# Branch-and-Bound e Aproximações: Desafios na Computação de Rotas para o Caixeiro Viajante
Este artigo avalia o desempenho de três implementações de algoritmos para computação de rotas no problema NP-difícil do Caixeiro Viajante. Serão analisadas duas soluções aproximadas e uma exata, avaliando o tempo e espaço usado por cada uma, além da qualidade da solução.

Os algoritmos aproximativos são "Twice around the tree" e "Christofides", e o código está presente no arquivo [aproximativos.py](algoritmos/aproximativos.py).

O algoritmo Branch and Bound está presente no arquivo [bnb.cc](algoritmos/bnb.cc)

O arquivo [primitivas.py](algoritmos/primitivas.py) contém funções auxiliares para os algoritmos, como criação de grafos a partir dos arquivos ```.tsp```.

O arquivo [avaliacao.py](algoritmos/avaliacao.py) contém um módulo de testes para avaliar métricas de tempo de execução, uso de memória e qualidade da solução para os algoritmos aproximativos

Por fim, o arquivo [lista-datasets.txt](lista-datasets.txt) contém todos os datasets da pasta nomeados e a solução exata de cada um, para análise do fator aproximativo.

Mais detalhes sobre a implementação e experimentos podem ser vistos no [relatório](artigo.pdf).