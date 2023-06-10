import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

bandwidth_mmap = pd.read_csv("messungen_mmap.txt", sep=";", decimal=".", names=['package size','bandwidth'])['bandwidth']
bandwidth_process_vm_readv = pd.read_csv("messungen_process_vm_readv.txt", sep=";", decimal=".", names=['package size','bandwidth'])['bandwidth']
bandwidth_pipes = pd.read_csv("messungen_pipes.txt", sep=";", decimal=".", names=['package size','bandwidth'])['bandwidth']

package_size = ('64 B','256 B','1 KB','4 KB','16 KB','64 KB','256 KB','1 MB','4 MB','16 MB')

x = np.arange(len(package_size))
bar_width = 0.3
plt.bar(x+bar_width, bandwidth_mmap, color="green", width=bar_width, label='mmap')
plt.bar(x+bar_width*2, bandwidth_process_vm_readv, color="orange", width=bar_width, label='readv')
plt.bar(x+bar_width*3, bandwidth_pipes, color="red", width=bar_width, label='pipe')

plt.xlabel(package_size)
plt.xticks(x+bar_width, package_size)
plt.grid(axis='y')
plt.legend()
plt.xlabel("Package size")
plt.ylabel("Bandwidth (MB/s)")
plt.title("Measurements of pipes speed")
plt.savefig("diagramm.pdf")
