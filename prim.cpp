#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <utility>

#define inf 1e9
typedef std::string string;
typedef std::pair<int,int> pairINT;

// Comparator to set increasing order to priority queue
struct increasingPQ {
    constexpr bool operator()(
        std::pair<int,int> const& a,
        std::pair<int,int> const& b)
        const noexcept
    {
        return a.second > b.second;
    }
};

class adj_list{
    public:
        int item;
        int w;
        adj_list *next;
        adj_list(int item, int weight){
            this->item = item;
            this->w = weight;
            this->next = NULL;
        }
};

class graph{
    public:
        int n;
        std::vector<adj_list*> vertices;
        std::vector<short> *visited;

        graph(int N){
            this->n = N;
            this->visited = new std::vector<short>(N,0);

            for(int i=0; i< N;i++){
                this->vertices.push_back(NULL);
            }

        }
        
        void add_edge(int item1, int item2, int weight, int directed);
        void display_nodes(int n);
        int prim(int source, int n);
        int getPathPrim(int n, std::vector<int> pred);

};

void graph::add_edge(int item1, int item2, int weight, int directed){
    adj_list *vex = new adj_list(item2, weight);
    if(this->vertices[item1] == NULL)
        this->vertices[item1] = vex;
    else{
        adj_list *aux1 = this->vertices[item1];
        while(aux1->next != NULL)
            aux1 = aux1->next;
        aux1->next = vex;
    }

    if(!directed){
        adj_list *vex2 = new adj_list(item1, weight);
    if(this->vertices[item2] == NULL)
        this->vertices[item2] = vex2;
        else{
            adj_list *aux2 = this->vertices[item2];
            while(aux2->next != NULL)
                aux2 = aux2->next;
            aux2->next = vex2;
        }
    }
}

int graph::prim(int source, int n)
{
    std::vector<int> cost(n, inf);

    std::priority_queue<pairINT, std::vector<pairINT> , increasingPQ > pqueue;
    std::vector<int> pred(n,-1);
    int visited = 0;
    
    std::vector<int> inQueue(n,1);


    cost[source] = 0;
    pqueue.push({source, 0});

    // Make sure that MST has n vertices
    while(!pqueue.empty() && visited < n){
        
        // Analyse the shortest distance from source to u so far, 
        // i.e., the head of the priority queue
        int u = pqueue.top().first;

        pqueue.pop();
        // Analyze adjacents of u if u is not in queue:
        if(inQueue[u] == 1){

            // Vertice is in MST and was visited already
            inQueue[u] = 0;
            visited += 1;


            // Get all of the adjacent nodes of u
            adj_list *auxU = this->vertices[u];
            while(auxU != NULL){

                // Get adjacent node id and weight uv
                int v = auxU->item;
                int weightUV = auxU->w;
                // If the cost of the new weight uv is shorter
                // than the current cost to v, and vertex v is not in queue, 
                // switch the minimum cost to v and put v and its new 
                // shorter cost in the priority queue. 

                    if(inQueue[v] == 1 && weightUV < cost[v]){
                        cost[v] = weightUV;
                        pqueue.push({v, cost[v]});
                        pred[v] = u;

                    }

                auxU = auxU->next;
            }

        }
    }

    return this->getPathPrim(n, pred);

}

int graph::getPathPrim(int n, std::vector<int> pred){

    int mst_cost = 0;
    std::cout << std::endl << "MST of G:" << std::endl;
    for(int i=1; i< n; i++){
        for(adj_list *j= this->vertices[i]; j != NULL ; j = j->next){
            if(pred[i] == j->item){
                std::cout << "Edge " << i+1 << "-" << j->item + 1 << ", w= " << j->w << "  ";
                mst_cost += j->w;
            }
        }
        std::cout << std::endl;
    }

    return mst_cost;

}


void graph::display_nodes(int n){
    // Print n vertices
    for(int i=0; i< n; i++){
        if(this->vertices[i] != NULL){
            adj_list *aux = this->vertices[i];
            std::cout << i << " -> ";
            while(aux != NULL){
                std::cout << aux->item << " -> ";
                aux = aux->next;
            }
            std::cout << std::endl;
        }
    }
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
    std::ifstream file(inputPath);

    graph *gp;
    
    if(file.is_open()){
        // Save n vertices and m edges
        file >> n >> m;
        gp = NULL;
        gp = new graph(n); // Initialize graph G with n vertices
        while(std::getline(file, line))
        {
            // Read and save each vertice u,v and weighted edge uv in G
            file >> u >> v >> w;
            std::cout << u << " "<< v << " " << w << std::endl;
            gp->add_edge(u-1, v-1, w, false); // false => undirected graph

        }
        file.close();
    }

    return gp;

}


int main(int argc, const char** argv) {

    int m, n, u, v, w;
    // Let n vertices be 1,2, ..., n
    graph *gp = readInputFile("data_mst.txt");

    int min_cost_mst = gp->prim(0, gp->n);
    std::cout << "Minimum spanning tree of G has cost: " << min_cost_mst << std::endl;

    return 0;
}