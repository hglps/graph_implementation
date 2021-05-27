#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <fstream>
#include <limits.h>
#include <queue>
#include <string>
#include <string.h>
using namespace std;

class edge{
    public:
        int u;
        int v;
        int w;
        edge(int U, int V, int W){
            this->u = U;
            this->v = V;
            this->w = W;
        }

};
// Comparator : increasing order of weight
struct comp {
    bool operator() (edge wi, edge wj) 
    { 
        return wi.w < wj.w; 
    }
} min_comp;

class graph{
    public:
        vector<vector<int>> adj;
        vector<int> pred;
        vector<int> ranking;
        int n;
        graph(int N){
            this->n = N;
			this->adj = vector<vector<int>>(N, vector<int>(N));
            this->pred = vector<int>(N, -1);
            this->ranking = vector<int>(N, -1);

        }
		void add_edge(vector<vector<int>> adj_list, int u, int v, int w);
        void make_set(int u);
        int find(int u);
        void union_(int u, int v);
        int kruskal(int n);
};

void graph::add_edge(vector<vector<int>> adj_list, int u, int v, int w)
{
	// Grafo nao direcionado para encontrar a arvore geradora minima
	// Matrix de adjacencia
	this->adj[u][v] = w;
    this->adj[v][u] = w;
}

void graph::make_set(int u)
{
    // Inicialmente, esta definido como n componentes conexas totalmente desconectadas entre si
    this->pred[u] = u;
    this->ranking[u] = 0;
}

int graph::find(int u)
{
    // Index of set which u belongs
    int r = u;
    while(this->pred[r] != r){
        r = pred[r];
    }    
    return r;
}

void graph::union_(int u, int v)
{
    // Ao percorrer find(u) e find(v), quando ambos sao iguais,
    // significa que o caminho para seus predecessores seria igual, indicando
    // a existencia de um ciclo, nao adicionando na arvore geradora minima.
    if(this->ranking[u] >= this->ranking[v]){
        this->pred[v] = u;
        if(this->ranking[u] == this->ranking[v]){
            this->ranking[u] +=1;
        }
    }
    else this->pred[u] = v;
}

int checkExists(vector<edge> edges, int u, int v, int w){
    edge e(u,v,w);
    for(int i=0; i<edges.size(); i++){
        if((e.u == edges[i].v) && (e.v == edges[i].u) ){
            return 1;
        }
    }
    return 0;
}

int graph::kruskal(int n)
{
    vector<edge> mst;

    // Make-set: Conjunto com 1 item
    for(int v1=0; v1< n; v1++){
        this->make_set(v1);
    }

    // Getting edges of G

    vector<edge> edges;

    for(int i=0; i< n; i++){
        for(int j=0; j<n; j++){
            if(this->adj[i][j] != 0){
                edge e(i, j, adj[i][j]);
                if(!checkExists(edges, i, j, adj[i][j])){
                    edges.push_back(e);
                }
            }
        }
    }
    // Sort edges by weight
    sort(edges.begin(), edges.end(), min_comp);

    for(int i=0; i< edges.size(); i++){
        edge e = edges[i];
        if(this->find(e.u) != this->find(e.v)){
            mst.push_back(e);
            this->union_(this->find(e.u), this->find(e.v));
        }
    }

    int mst_cost = 0;
    for(int j=0; j<mst.size(); j++){
        cout << "Edge " << mst[j].u << "-" << mst[j].v << " : w= " << mst[j].w;
        cout << endl;
        mst_cost += mst[j].w;
    }
    cout << endl;


    return mst_cost;
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
	graph *gp = readInputFile("data_mst.txt");
	cout << "Arvore geradora minima em G tem custo = " << gp->kruskal(gp->n);

	return 0;
}
