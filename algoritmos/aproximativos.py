import networkx as nx
from typing import Tuple, List

def twice(grafo) -> Tuple[List[int], float]:
	# gera a arvore geradora minima Kruskal
	AGM = nx.minimum_spanning_tree(grafo)

	# percorre a AGM em DFS, gerando uma lista de vertices visitados (com repeticao)
	caminho = list(nx.dfs_preorder_nodes(AGM))

	# adiciona o primeiro vertice no final do caminho
	caminho.append(caminho[0])

	# calcula o custo do caminho
	custo = 0
	for i in range(len(caminho)-1):
		custo += grafo[caminho[i]][caminho[i+1]]['weight']

	return custo

def christofides(grafo) -> Tuple[List[int], float]:
	# gera a arvore geradora minima
	AGM = nx.minimum_spanning_tree(grafo)

	# gera o grafo com o conjunto de vertices de grau impar
	impares = [vertice for vertice, grau in AGM.degree() if grau % 2 == 1]

	# gera o perfect matching de custo minimo com o conjunto de vertices de grau impar
	PM = nx.min_weight_matching(grafo.subgraph(impares))

	# gera o grafo final para percorrer
	multiAGM = nx.MultiGraph(AGM)
	multiAGM.add_edges_from([(u, v, {'weight': grafo[u][v]['weight']}) for u, v in PM])

	caminho = [u for u, _ in nx.eulerian_circuit(multiAGM)]

	caminho = list(dict.fromkeys(caminho))

	# adiciona o primeiro vertice no final do caminho
	caminho.append(caminho[0])

	# calcula o custo do caminho
	custo = sum([grafo[caminho[i]][caminho[i+1]]['weight'] for i in range(len(caminho)-1)])

	return custo