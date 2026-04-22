import numpy as np
import random
import time
#import matplotlib.pyplot as plt
import networkx as nx
import os

N = 200  # nodos
k_medio = 6 # <k>
p = k_medio / (N-1) # probabilidad de conexion
seed = random.seed(time.time())  # seed random number generators

# Generar la red ER
G = nx.erdos_renyi_graph(N, p, seed=seed, directed=False)
# Generar la matriz de adyacencia (ceros y unos)
A = nx.to_numpy_array(G, dtype=int)



#endTime = time.time()
#print("Tiempo de ejecucion (s): ", endTime - startTime)

ruta_directorio = "C:/Users/carme/OneDrive/Escritorio/FISICA/2024-2025/SEGUNDO_CUATRI/Caos_y_sistemas_dinamicos_no_lineales/Trabajos/Programas/"  
nombre_archivo = "matriz_ER.txt"
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
nombre_histograma = "histograma_conectividad_ER.txt"
ruta_histograma = os.path.join(ruta_directorio, nombre_histograma)

# Guardar histograma: columna 1 = k, columna 2 = P(k)
with open(ruta_histograma, 'w') as f:
    for k in range(N):
        f.write(f"{k}\t{histograma[k]}\n")
