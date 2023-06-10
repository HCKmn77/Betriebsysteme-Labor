import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("messungen.txt", sep=";", decimal=".", names= ["Package size", "Bandwidth"])
df
x = df["Package size"]
y = df["Bandwidth"]
print("X: ", x, "Y:", y)

plt.bar(['64 B','256 B','1 KB','4 KB','16 KB','64 KB','256 KB','1 MB','4 MB','16 MB'], y, color="green")

for i,j in zip(x,y):
    plt.annotate('  x: ' + str(i) +'\n  y: ' + str(j),xy=(i,j))
plt.grid(axis='y')

plt.xlabel("Package size")
plt.ylabel("Bandwith (MB/s)")
plt.title("Measurements of pipes speed")
plt.savefig("diagramm.pdf")
