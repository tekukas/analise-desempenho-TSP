import networkx as nx
from typing import Tuple, List

def twice(grafo) -> Tuple[List[int], float]:
	"""Algoritmo Twice around the tree para o Problema do Caixeiro Viajante
		Entrada: grafo ponderado e completo
		Saida: custo do caminho mínimo"""
	
	# gera a arvore geradora minima Kruskal
	AGM = nx.minimum_spanning_tree(grafo)

	# percorre a AGM em DFS, gerando uma lista de vertices visitados (com repeticao)
	caminho = list(nx.dfs_preorder_nodes(AGM))

	# adiciona o primeiro vertice no final do caminho para formar um ciclo
	caminho.append(caminho[0])

	# calcula o custo do caminho
	custo = sum([grafo[caminho[i]][caminho[i+1]]['weight'] for i in range(len(caminho)-1)])

	return custo

def christofides(grafo) -> Tuple[List[int], float]:
	"""Algoritmo de Christofides para o Problema do Caixeiro Viajante
		Entrada: grafo ponderado e completo
		Saida: custo do caminho mínimo"""

	# gera a arvore geradora minima
	agm = nx.minimum_spanning_tree(grafo)

	# seleciona o conjunto de vertices de grau impar
	impares = [vertice for vertice, grau in agm.degree() if grau % 2 == 1]

	# gera o perfect matching de custo minimo com esse conjunto
	perfectMatching = nx.min_weight_matching(grafo.subgraph(impares))

	# gera o grafo final para percorrer
	multiAGM = nx.MultiGraph(agm)
	multiAGM.add_edges_from([(u, v, {'weight': grafo[u][v]['weight']}) for u, v in perfectMatching])

	caminho = [u for u, _ in nx.eulerian_circuit(multiAGM)]

	caminho = list(dict.fromkeys(caminho))

	# adiciona o primeiro vertice no final do caminho para formar um ciclo
	caminho.append(caminho[0])

	# calcula o custo do caminho
	custo = sum([grafo[caminho[i]][caminho[i+1]]['weight'] for i in range(len(caminho)-1)])

	return custo