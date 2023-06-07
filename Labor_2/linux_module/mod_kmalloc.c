/*
 *
 * Author:  Fabian Schneider
 *          Jeremia Haackmann
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h> // For kmalloc()
#include <linux/types.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Fabian Schneider, Jeremia Haackmann");
MODULE_DESCRIPTION("kmalloc time measurement");

static int **ptr;
static unsigned int loop_cnt = 1024;
static unsigned int alloc_size = 64;

module_param(loop_cnt, int, S_IRUGO);
MODULE_PARM_DESC(loop_cnt, "Number of loop iterations (Default=1024)");
module_param(alloc_size, int, S_IRUGO);
MODULE_PARM_DESC(alloc_size, "Size of allocated storage (Default=64)");


static inline uint64_t setRDTSC(void)
{
  unsigned int hi, lo;
  __asm__ volatile("rdtsc"
                   : "=a"(lo), "=d"(hi));
  return ((uint64_t)hi << 32) | lo;
}
__init int init_module(void)
{
  printk(KERN_INFO "Init time measurement module\n");

  ptr = kmalloc(loop_cnt * alloc_size, GFP_KERNEL);
  if (ptr != NULL)
  {
    uint64_t start = setRDTSC();
    for (int i = 0; i < loop_cnt; i++)
    {
      ptr[i] = kmalloc(alloc_size, GFP_KERNEL);
    }
    uint64_t stop = setRDTSC();

    uint64_t avgTime = (stop - start) / loop_cnt;
    printk(KERN_INFO "Average Malloc-Time for %d iterations, each with an allocated storage size of %d bytes: %d\n", loop_cnt, alloc_size, avgTime);
  }
  else
  {
    printk(KERN_INFO "Storage allocation not successful!\n");
    return 1;
  }
  return 0;
}
__exit void cleanup_module(void)
{
  printk(KERN_INFO "Cleanup of time measurement module\n");

  for (unsigned int i = 0; i < loop_cnt; i++)
  {
    kfree(ptr[i]);
  }
  kfree(ptr);
}
