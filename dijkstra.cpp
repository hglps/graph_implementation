#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <functional>
#include <utility>

#define inf 1e9 //search for better reference of 'infinity'
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
        int dijkstra(int source, int target, int n);
        void showPathDijkstra(int pred[], int target);
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

void graph::showPathDijkstra(int pred[], int target)
{
    std::vector<int> predecessors;
    predecessors.push_back(target);
    for(int i= target; i != 0; i= pred[i]){
        predecessors.push_back(pred[i]);
    }
    std::cout << "Final path= ";
    for(int i=predecessors.size()-1; i>= 0; i--)
    {
        std::cout << predecessors[i];
        if(i != 0) std::cout << " - ";
    }
    std::cout << std::endl;
}

int graph::dijkstra(int source, int target, int n){
    // Initialize cost to each vertice as infinity
    int cost[n];
    for(int i=0; i<n; i++)
        cost[i] = inf;
    std::priority_queue<pairINT, std::vector<pairINT> , increasingPQ > pqueue;
    
    // Create predecessors vector to show path through source to target in the end
    int pred[n];
    pred[0] = -1;
    // Cost from source to source is 0
    cost[source] = 0;
    pqueue.push({source, 0});
    
    while(!pqueue.empty()){
        // Analyse the shortest distance from source to u so far, 
        // i.e., the head of the priority queue
        int u = pqueue.top().first;
        pqueue.pop();
        
        // Get all of the adjacent nodes of u
        adj_list *auxU = this->vertices[u];
        while(auxU != NULL){

            // Get adjacent node id and weight uv
            int v = auxU->item;
            int weightUV = auxU->w;
            // If the cost through u + the new weight uv is shorter
            // than the current cost to v, switch the minimum cost to v
            // and put v and its new shorter cost in the priority queue. 
            if(cost[u] + weightUV < cost[v]){
                cost[v] = cost[u] + weightUV;
                pqueue.push({v, cost[v]});
                pred[v] = u;
            }
            auxU = auxU->next;
        }

    }

    showPathDijkstra(pred, target);

    return cost[target];
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
            gp->add_edge(u-1, v-1, w, false); // false = undirected graph

        }
        file.close();
    }

    return gp;

}


int main(int argc, const char** argv) {

    int m, n, u, v, w;
    // Let n vertices be 1,2, ..., n
    graph *gp = readInputFile("data_w.txt");

    int dist = gp->dijkstra(0,gp->n - 1,gp->n);
    std::cout << "Nearest path has total weight: " << dist << std::endl;

    return 0;
}
