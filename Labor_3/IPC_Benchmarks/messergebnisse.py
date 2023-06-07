import matplotlib.pyplot as plt
import pandas as pd

df = pd.read_csv("messungen.txt", sep=";", decimal=".", names= ["Package size", "Bandwidth"])
df
x = df["Package size"]
y = df["Bandwidth"]
print("X: ", x, "Y:", y)
plt.plot(x, y)
plt.xlabel("Package size")
plt.ylabel("Bandwith (MB/s)")
plt.title("Measurements for pipes speed")
plt.savefig("diagramm.pdf")
