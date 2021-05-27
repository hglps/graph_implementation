#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <limits.h>
#include <queue>
#include <string>
#include <string.h>
using namespace std;

#define inf 1e9

class graph{
    public:
        vector<vector<int>> adj;
        int n;
        graph(int N){
            this->n = N;
			this->adj = vector<vector<int>>(N, vector<int>(N));

        }
		void add_edge(vector<vector<int>> adj_list, int u, int v, int w);
};

void graph::add_edge(vector<vector<int>> adj_list, int u, int v, int w)
{
	// Grafo capacitado direcionado por definicao
	// Matrix de adjacencia
	this->adj[u][v] = w;
}

bool bfs_ff(vector<vector<int>> residual_gp,  int path_n[], int s, int t, int N)
{
	// Codigo basico de busca por largura BFS. Vai marcar como visitado os vertices da fila
	// e vai percorrendo, salvando o caminho em path_n se houver caminho de s para t

	bool visitado[N];
	visitado[s] = true;

	for(int i=0; i<N; i++)
	{
		visitado[i] = false;
	}

	queue<int> q;

	q.push(s);
	path_n[s] = -1; // Indicando que qualquer caminho que chegue em s seja terminado

	// Executa o BFS ja conhecido
	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int v = 0; v < N; v++) {
			if (visitado[v] == false && residual_gp[u][v] > 0) {
				
				if (v == t) {
					// Se existe o caminho de s ate t, retorna os caminhos ja existentes.
					// Nao precisa percorrer necessariamente todos os caminhos do grafo se o caminho s-> ... -> t ja foi encontrado
					path_n[v] = u;
					return true; // Se existe caminho st
				}
				q.push(v);
				path_n[v] = u;
				visitado[v] = true;
			}
		}
	}

	// Se nao existe caminho st. Caso de parada do algoritmo de Ford-Fulkerson
	return false;
}

int fordFulkerson(graph* gp, int s, int t)
{
	// s : vertice fonte / source vertex
	// t : vertice sumidouro / sink vertex
	// Ford-Fulkerson vai calcular o fluxo maximo da fonte ate o sumidouro

	int v1, v2; //, residual_graph[gp->n][gp->n];
	
	// Obtem toda a matrix de adjacencia do grafo, com todas as arestas e capacidades de cada aresta
	vector<vector<int>> adj_matrix = gp->adj;
	vector<vector<int>> residual_graph(gp->n, vector<int>(gp->n));
	
	// Inicializa todo o grafo residual igualmente ao grafo original
	for (v1=0; v1<gp->n; v1++)
	{
		for (v2=0; v2<gp->n; v2++)
		{
			residual_graph[v1][v2] = adj_matrix[v1][v2];
		}
	}

	// Inicializa todo os caminhos do grafo, que partindo de s chega em t.
	int path_st[gp->n], fluxo;

	// Antes de percorrer os outros vertices, o fluxo maximo de G vai ser zero
	fluxo = 0; 

    int count = 0;
	while (bfs_ff(residual_graph, path_st, s, t, gp->n)) {
		
		int fluxo_atual = inf;
		// Analisa o caminho obtido e percorre cada aresta e obtem a menor
		for (v2 = t; v2 != s; v2 = path_st[v2]) {
			v1 = path_st[v2];
			fluxo_atual =  (fluxo_atual < residual_graph[v1][v2]) ? fluxo_atual : residual_graph[v1][v2];
		}

		// Analisa o caminho obtido novamente, mas agora reduzindo as arestas pela menor e gerando as arestas inversas com capacidade complementar
		for (v2 = t; v2 != s; v2 = path_st[v2]) {
			v1 = path_st[v2];
			residual_graph[v1][v2] -= fluxo_atual; // Gera-se a reducao das arestas a partir da menor
			residual_graph[v2][v1] += fluxo_atual; // Gera-se a aresta inversa do grafo capacitado. Ao fim, a soma das capacidades que chegam em s sera o fluxo maximo tambem
		}

		fluxo += fluxo_atual; // Menor capacidade compoe o fluxo maximo

        count +=1;
	}

	return fluxo;
}

graph* readInputFile(string inputPath)
{
    // Reading input file
    // line 1: n_vertices m_edges
    // line 2: v_1 v_2 w_1
    // line 3: v_3 v_4 w_2
    // ...
    // line m+1
    int m, n;
    int u, v, w;
    string line;
    ifstream file(inputPath);

    graph *gp;
    if(file.is_open()){
        // Save n vertices and m edges
		
        file >> n >> m;
        gp = new graph(n); // Initialize graph G with n vertices
		cout << n << "," << m << endl;
        while(getline(file, line))
        {
            // Read and save each vertice u,v and weighted edge uv in G
            file >> u >> v >> w;
            cout << u << " "<< v << " " << w << endl;
            gp->add_edge(gp->adj, u-1, v-1, w);

        }
        file.close();
    }

    return gp;

}

int main()
{
	graph *gp = readInputFile("data_wd.txt");

	cout << "Fluxo maximo de G igual a= " << fordFulkerson(gp, 0, gp->n - 1);

	return 0;
}
