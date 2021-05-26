#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <queue>
#include <functional>
#include <utility>

#define MAX_SIZE 100
#define MAX_INT 100000

class adj_list{
	public:
		int vertex;
		int weight;
		int cost;
		adj_list *next;
		adj_list(int vertex, int weight){
			this->vertex = vertex;
			this->weight = weight;
			this->next = NULL;
			this->cost = 0;
		}
		
};

class graph{
	public:
		adj_list *vertices[MAX_SIZE];
		short visited[MAX_SIZE];
		int n;
		graph(int n){
			this->n = n;
            for(int i=0; i< n; i++){
                this->vertices[i] = NULL;
                this->visited[i] = 0;

            }
        }
        std::vector<int> MST;
        std::vector<int> franja;
		int cost;
		
        void add_edge(int vertex1, int vertex2, int weight, int directed);
		void display_nodes(int n);
		int prim();
};

void graph::add_edge(int vertex1, int vertex2, int weight, int directed){
	adj_list *v1 = new adj_list(vertex2, weight);
	if(this->vertices[vertex1] == NULL){
		this->vertices[vertex1] = v1;
	}
	else{
		adj_list *aux1 = this->vertices[vertex1];
		while(aux1->next != NULL){
			aux1 = aux1->next;
		}
		aux1->next = v1;
	}

	if (!directed){
		adj_list *v2 = new adj_list(vertex1, weight);
	
		if(this->vertices[vertex2] == NULL){
			this->vertices[vertex2] = v2;
		}
		else{
			adj_list *aux2 = this->vertices[vertex2];
			while(aux2->next != NULL){
				aux2 = aux2->next;
			}
			aux2->next = v2;
		}
	}
};

void graph::display_nodes(int n){
	// n: vertices
	for(int i=0; i<n; i++){
		if(this->vertices[i] != NULL){
			adj_list *aux = this->vertices[i];
			std::cout << i << " -> ";
			while(aux != NULL){
				if (aux->next != NULL){
					std::cout << aux->vertex<<", "<< aux->weight<<" -> ";
				}
				else{
					std::cout << aux->vertex<<", "<< aux->weight;
				}

				aux = aux->next;
			}
			std::cout << std::endl;
		}
	}
};

adj_list *remove_item(adj_list *head, int item){
	
	adj_list *previous = NULL;
	adj_list *current = head;
	while(current -> vertex != item && current != NULL)
	{
		previous = current;
		current = current -> next;
	}
	if(current == NULL) return head;
	else if(previous == NULL)
		head = current -> next;
	else
		previous -> next = current -> next;
	free(current);
	return head;
}

adj_list* delete_item(adj_list* curr, int x) {
    adj_list* next;
    if (curr == NULL) { // Found the tail
        std::cout << "not found\n";
        return NULL;
    } else if (curr->vertex == x) { // Found one to delete
        next = curr->next;
        free(curr);
        return next;
    } else { // Just keep going
        curr->next = delete_item(curr->next, x);
        return curr;
    }
}

adj_list* remove_edge(adj_list* G, int v){
	G = delete_item(G,v);	
	return G;
}



int graph::prim(){
	
	int min = MAX_INT;
	int index = 0;
	int aux1, aux2, i;
	std::vector<std::vector<int>> tree;
	std::vector<std::vector<int>> subtree;
	// OS N VERTICES SAO ENUMERADOS DE 0 A N-1.
	this->franja.push_back(0);
	
while(1){
	if(this->franja.size() == this->n){
		for(int ii = 0; ii<this->franja.size(); ii++){
			std::cout<<this->franja[ii]<<",";
		}
		break;
	}
	for( i=0; i<this->franja.size(); i++){
	
		adj_list *aux = this->vertices[this->franja[i]];
		
		aux1 = this->franja[i];
		while (aux->next != NULL){
			aux2 = aux->vertex;
			int flag = 1;
			for(int iii=0; iii<this->franja.size(); iii++){
					if(aux2 == this->franja[iii] ){
						flag = 0;
					}
				}
			if(flag){
				if (aux->weight < min){				
				
				
					aux1 = this->franja[i];
					min = aux->weight;
					aux2 = aux->vertex;
				
				// else ?		
				}
			}

			
			aux = aux->next;
		}
	}
		std::vector<int> vect{aux1,aux2,min};
		tree.push_back(vect);
		//for (int j=0; j<this->franja.size();j++){
			for(int k=0; k<2; k++){
				std::cout << vect[k] << ","; 
			}
		//}
			std::cout<<std::endl;
		//int u = this->franja[i];
		//int v = aux2;
		this->vertices[aux1] = remove_edge(this->vertices[aux1],aux2);
		//std::cout << aux1 << std::endl;
		//std::cout << aux2 << std::endl;
		this->vertices[aux2] = remove_edge(this->vertices[aux2],aux1);
		
		
		this->franja.push_back(aux2);
		//custo += min;
		//aux2->cost = custo;
		
		//this->MST.push_back(aux2->vertex);
		
		//index ++;
		// cost = 16
		// MST = [0,7]
		// go to next vertex

		//aux = this->vertices[aux2];
		
	min = MAX_INT;
}

}

int main(int argc, const char** argv){
	int m,n,u,v,w;
	std::cout << "Enter n vertices and m arestas:" << std::endl;
	std::cin >> n >> m;

	graph *G = new graph(n);

	 std::cout <<"Enter " << m << " edges and weights:\n";
	  for(int i=0; i<m; i++){
        std::cin >> u >> v >> w;
        G->add_edge(u, v, w, false); // false : undirected

    }
    std::cout << std::endl;
    G->prim();
    //G->display_nodes(n);
    
    return 0;
}