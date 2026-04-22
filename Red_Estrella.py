import matplotlib.pyplot as plt
import networkx as nx

# Datos del histograma
k_values = [1, 10]
counts = [10, 1]

# Crear el histograma
fig, ax = plt.subplots()
ax.bar(k_values, counts, width=0.5)
ax.set_xlabel("k")
ax.set_ylabel("P(k)")
ax.set_title("número de nodos con conectividad k en una red estrella con 10 hojas y un Hub")
ax.set_xticks([1, 10])

# Crear el símbolo de red estrella
G = nx.star_graph(10)  # 10 hojas + 1 hub
pos = nx.spring_layout(G, seed=42)

# Ejes nuevos para la red en la zona blanca
inset_ax = fig.add_axes([0.65, 0.6, 0.25, 0.25])  # [left, bottom, width, height]
inset_ax.axis("off")
nx.draw(G, pos, ax=inset_ax, node_size=100, with_labels=False)

plt.show()