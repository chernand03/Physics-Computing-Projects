import networkx as nx
import numpy as np
import os
import random
import time


# 1. Parámetros
N = 200  # número total de nodos
m = 3    # cada nuevo nodo se conecta a m nodos existentes

seed = random.seed(time.time())

# 2. Crear la red Barabási-Albert con NetworkX
G = nx.barabasi_albert_graph(n=N, m=m, seed=seed)

# 3. Obtener la matriz de adyacencia
A = nx.to_numpy_array(G, dtype=int)

ruta_directorio = "C:/Users/carme/OneDrive/Escritorio/FISICA/2024-2025/SEGUNDO_CUATRI/Caos_y_sistemas_dinamicos_no_lineales/Trabajos/Programas/"  
nombre_archivo = "matriz_BA.txt"
ruta_completa = os.path.join(ruta_directorio, nombre_archivo)

# Crear carpeta si no existe
os.makedirs(ruta_directorio, exist_ok=True)

# Guardar el archivo
np.savetxt(ruta_completa, A, fmt='%d')
print(f"Matriz guardada en: {ruta_completa}")

# Calcular el grado promedio
grado_promedio = sum(dict(G.degree()).values()) / N
print(f"Grado promedio real: {grado_promedio}")

grados = [grado for nodo, grado in G.degree()]  #guardamos el grado de cada nodo en un array
histograma = np.zeros(N, dtype=float) #inicializamos a 0 el histograma

for i in range(N):
    histograma[grados[i]] = histograma[grados[i]]+1
    
for i in range(N):
    histograma[i] = histograma[i]/N
    
# Nombre para el archivo del histograma
nombre_histograma = "histograma_conectividad_BA.txt"
ruta_histograma = os.path.join(ruta_directorio, nombre_histograma)

# Guardar histograma: columna 1 = k, columna 2 = P(k)
with open(ruta_histograma, 'w') as f:
    for k in range(N):
        f.write(f"{k}\t{histograma[k]}\n")

print(f"📊 Histograma guardado en: {ruta_histograma}")
    



