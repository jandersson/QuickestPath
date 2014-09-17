#include <iostream>
#include "Link.h"
#include "Path.h"
#include <string>
#include <vector>
#include "limits.h"
#define ring_size  3
#define torus_size  2

using namespace std;

//Parent array keeps track of the best neighboring node which is used to find the shortest path.
//Parent is global so it does not need to be passed and returned from several functions as a pointer
int parent[64];


//Dijkstra functions
void Dijkstra(vector<vector<int> > graph, int source, int num_nodes);
int calculateMinDistance(int dist[], bool visit[], int num_nodes);

vector<int> compareCapacaties(vector<Link> listOfLinks);
vector<vector<int> > generateRingGraph(vector<Link> listOfLinks);
vector<vector<int> > generateTorusGraph(vector<Link> listOfLinks);
void print_path(vector<int> path);
vector<int> calculatePath(int dest, int src, int parent[], vector<int> path, int num_nodes);

int main(){

    // User will specify which topology they want (ring or torus)
    // We will set size of each topology to ring = 9 nodes, torus = 16 nodes

    //[X] Create graph from link objects
    //	[X] Two seperate functions for creating a ring or a torus
    //	[X] Loop through vector of links looking for "end_of_link" that is equal to "start_of_link"
    //[] Update algorithm to use vectors
    //[X] Store user input in vector


    // Need a function to compare all c values and find the lowest c and update the boolean value in the corresponding link vectors with a higher C
    // Use the function to compare all C's again but of only links with true boolean values
    // If the algorithm can't reach the destination due to false boolean values perform the calculations on the found paths

    bool more = true;
    int num_nodes;
    vector<int> path;
    vector<Link> listOfLinks;
    int destination;
    int source;
    long packetSize;
    int topo_choice;

    while (more){
        std::cout << "Which topology would you like to use? [1: Ring(9 nodes) / 2: Torus(16 nodes) / 3: exit]" << endl;
        cin >> topo_choice;

        switch (topo_choice){
            case 1:{
                for (int i = 0; i < ring_size; i++){
                    Link link = Link();
                    //When entering the first start_of_link, it must be 0 for memory allocation purposes.
                    std::cout << "Enter the start of the link " << i << ": ";
                    cin >> link.start_of_link;
                    std::cout << "Enter the end of the link " << i << ": ";
                    cin >> link.end_of_link;
                    std::cout << "Enter the propogation delay of the link: ";
                    cin >> link.propagation_delay;
                    std::cout << "Enter the capacity of the link: ";
                    cin >> link.capacity;
                    cout << endl;
                    listOfLinks.push_back(link);
                    link.~Link();
                }

                //DEBUGGING BELOW
                /*for (std::vector<Link>::const_iterator it = listOfLinks.begin(); it != listOfLinks.end(); it++){
                    std::cout << "TOPOLOGY" << endl << it->start_of_link << endl;*/

                bool new_topo = false;
                while (!new_topo) {
                    cout << endl << "Please enter the source: " << endl;
                    cin >> source;
                    cout << "Please enter the destination: " << endl;
                    cin >> destination;
                    cout << "Please enter the packet size in bytes: " << endl;
                    cin >> packetSize;

                    num_nodes = ring_size;

                    //FUNCTION CALLS HERE
                    vector<vector<int> > ring_dist_graph = generateRingGraph(listOfLinks);

                    Dijkstra(ring_dist_graph, source, num_nodes);
                    vector<int> new_path = calculatePath(destination, source, parent, path, num_nodes);
                    print_path(new_path);

                    cout << "Do you want to make a new topology[1] or use the previous topology[0] or quit[2]? [0: Previous / 1: New / 2: Quit]" << endl;
                    int selection;

                    cin >> selection;
                    switch (selection){
                        case 0:
                            break;
                        case 1:
                            new_topo = true;
                            break;
                        case 2:
                            new_topo = true;
                            more = false;
                            break;
                        default:
                            cout << "Not a valid input; Assuming Previous" << endl;
                            break;
                    }
                }
            }
            case 2:{
                cout << "\nWARNING: The amount inputs required for a torus is 4 times the number of nodes.\n\n";
                for (int i = 0; i < (torus_size*4); i++){
                    Link link = Link();
                    //When entering the first start_of_link, it must be 0 for memory allocation purposes.
                    std::cout << "Enter the start of the link " << i << ": ";
                    cin >> link.start_of_link;
                    std::cout << "Enter the end of the link " << i << ": ";
                    cin >> link.end_of_link;
                    std::cout << "Enter the propogation delay of the link: ";
                    cin >> link.propagation_delay;
                    std::cout << "Enter the capacity of the link: ";
                    cin >> link.capacity;
                    cout << endl;
                    listOfLinks.push_back(link);
                    link.~Link();
                }

                //DEBUGGING
                /*for (std::vector<Link>::const_iterator it = listOfLinks.begin(); it != listOfLinks.end(); it++){
                    std::cout << "TOPOLOGY" << endl << it->start_of_link << endl;
                }*/
                // END DEBUGGING


                bool new_topo = false;
                while (!new_topo) {
                    cout << "Please enter the source: " << endl;
                    cin >> source;
                    cout << "Please enter the destination: " << endl;
                    cin >> destination;
                    cout << "Please enter the packet size in bytes: " << endl;
                    cin >> packetSize;

                    num_nodes = torus_size;

                    // FUNCTION CALLS HERE
                    vector<vector<int> > torus_dist_graph = generateTorusGraph(listOfLinks);

                    Dijkstra(torus_dist_graph, source, num_nodes);
                    vector<int> new_path = calculatePath(destination, source, parent, path, num_nodes);
                    print_path(new_path);

                    cout << "Do you want to make a new topology[1] or use the previous topology[0] or quit[2]? [0: Previous / 1: New / 2: Quit]" << endl;
                    int selection;

                    cin >> selection;
                    switch (selection){
                        case 0:
                            break;
                        case 1:
                            new_topo = true;
                            break;
                        case 2:
                            new_topo = true;
                            more = false;
                            break;
                        default:
                            cout << "Not a valid input; Assuming Previous" << endl;
                            break;
                    }
                }

            }
            case 3:
                more = false;
            default:
                cout << endl << "Not a valid choice" << endl;
                break;
        }

    }


    cout << endl << endl << "Program ended" << endl;
    cin.get();
}

void Dijkstra(vector<vector<int> > graph, int source, int num_nodes){


    int dist[9]; // Array to hold distances to nodes from source
    bool visit[9]; // Array to determine if a node is visited

    for (int i = 0; i < num_nodes; i++){
        dist[i] = INT_MAX; // Set all distances to infinite (INT_MAX will effectively work as infinity)
        visit[i] = false; // set all nodes to unvisited
    }

    dist[source] = 0; // Set the distance from the source to itself to 0

    for (int count = 0; count < num_nodes - 1; count++){
        int u = calculateMinDistance(dist, visit, num_nodes);
        visit[u] = true;
        for (int i = 0; i < num_nodes; i++){
            if (!visit[i] && graph[u][i] && dist[u] != INT_MAX && dist[u] + graph[u][i] < dist[i]){
                dist[i] = dist[u] + graph[u][i];
                parent[i] = u;
            }
        }
    }
}

int calculateMinDistance(int dist[], bool visit[], int num_nodes){
    int min_index;
    int min = INT_MAX;

    for (int i = 0; i < num_nodes; i++){
        if (dist[i] <= min && visit[i] == false){
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

void print_path(vector<int> path){

    cout << "\nPath is:\t " << endl;
    for (std::vector<int>::reverse_iterator it = path.rbegin(); it != path.rend(); it++){
        cout << *it;
    }
}

vector<int> calculatePath(int dest, int src, int parent[], vector<int> path, int num_nodes){

    if (src == dest){
        cout << "\nSource is equal to destination!\n";
        return path;
    }
    path.push_back(dest);
    bool found = false;
    while (found == false){
        for (int i = 0; i < num_nodes; i++){
            if (i == dest){

                path.push_back(parent[i]);
                dest = parent[i];
                if (parent[i] == src){
                    found = true;
                    return path;
                }
            }
        }
    }

    return path;

}

// The following steps describe the functions "generateRingGraph" and "generateTorusGraph"
//1. Iterate throght the vector to get to a Link.
//2. Dimension 1 of dist_graph is the start_of_link node.
//3. Dimension 2 of dist_graph is the end_of_link node.
//4. Find the end_of_link spot in array and enter the propogation_delay
//5. Reverse the array index so that the end_of_link is the first index and the start_of_link is the second index (fliping steps 1-3) to complete the graph
vector<vector<int> > generateRingGraph(vector<Link> listOfLinks){
    vector<vector<int> > ring_dist_graph;
    ring_dist_graph.resize(ring_size, vector<int>(ring_size));
    //This loop does steps 1-5 above.
    for (std::vector<Link>::const_iterator it = listOfLinks.begin(); it != listOfLinks.end(); it++){

        for (int i = 0; i < ring_size; i++){
            ring_dist_graph[it->start_of_link][it->end_of_link] = it->propagation_delay;
            ring_dist_graph[it->end_of_link][it->start_of_link] = it->propagation_delay;
        }
    }

    //The above loop leaves empty spaces in the graph for distances from all nodes to themselves.  Since that value is always 0, this loop populates the graph with 0s for those distances.
    for (int i = 0; i < ring_size; i++){
        ring_dist_graph[i][i] = 0;
    }

    return ring_dist_graph;

}
vector<vector<int> > generateTorusGraph(vector<Link> listOfLinks){
    vector<vector<int> > torus_dist_graph;
    torus_dist_graph.resize((torus_size*4), vector<int>(torus_size*4));
    //This loop does steps 1-5 above.
    for (std::vector<Link>::const_iterator it = listOfLinks.begin(); it != listOfLinks.end(); it++){

        for (int i = 0; i < torus_size; i++){
            torus_dist_graph[it->start_of_link][it->end_of_link] = it->propagation_delay;
            torus_dist_graph[it->end_of_link][it->start_of_link] = it->propagation_delay;
        }
    }

    //The above loop leaves empty spaces in the graph for distances from all nodes to themselves.  Since that value is always 0, this loop populates the graph with 0s for those distances.
    for (int i = 0; i < torus_size; i++){

        torus_dist_graph[i][i] = 0;
    }

    return torus_dist_graph;
}
//
//vector<int> compareCapacaties(vector<Link> listOfLinks){
//
//	return 
//}