//Quickest Path Algorithm
//For ECE427
//By Jonas Andersson
#include <iostream>
#include <fstream>
#include "Link.h"
#include "Path.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include "limits.h"
#define ring_size 4
#define mesh_size 8
using namespace std;
//TODO: The number of nodes is hardcoded into dijkstras function, allow the user to input the #
//Parent array keeps track of the best neighboring node which is used to find the shortest path.
//Parent is global so it does not need to be passed and returned from several functions as a pointer
int parent[64];

//Function Prototypes
int Dijkstra(vector<vector<int> > graph, int source, int num_nodes, int dest);
int calculateMinDistance(int dist[], bool visit[], int num_nodes);
vector<int> getCapacities(vector<int> path, vector<Link>);
int compareCapacaties(vector<Link> new_listOfLinks);
vector<vector<int> > generateGraph(vector<Link> listOfLinks, int num_nodes);
void print_path(vector<int> path);
vector<int> calculatePath(int dest, int src, int parent[], int num_nodes);
float calculateTotalLatency(float packetSize, int propDelay, int capacity);
void quickestPath(vector<Link> listOfLinks, int source, int dest, int num_nodes, float packetSize);
void displayQuickestPath(vector<Path> listOfPaths);

int main(){

	bool more = true;
	int num_nodes;
	vector<Link> listOfLinks;
	int destination;
	int source;
	float packetSize;
	int topo_choice;

	while (more){
		std::cout << "Which topology would you like to use? \n[1: Graph1 (" << ring_size << " nodes) | 2: Graph2 ("
			<< mesh_size << " nodes) | 3: Provide CSV File | 0: exit]" << endl;
		cin >> topo_choice;

		switch (topo_choice){
		case 1:{

				   //Prop, Cap, Start, End
				   Link link1 = Link(12, 10, 2, 3);
				   Link link2 = Link(4, 25, 0, 2);
				   Link link3 = Link(13, 50, 1, 3);
				   Link link4 = Link(12, 100, 0, 1);
				   Link link5 = Link(1, 90, 1, 2);
				   listOfLinks.push_back(link1);
				   listOfLinks.push_back(link2);
				   listOfLinks.push_back(link3);
				   listOfLinks.push_back(link4);
				   listOfLinks.push_back(link5);

				   cout << endl << "Please enter the source: " << endl;
				   cin >> source;
				   cout << "Please enter the destination: " << endl;
				   cin >> destination;
				   cout << "Please enter the packet size in bytes: " << endl;
				   cin >> packetSize;
				   num_nodes = ring_size;
				   quickestPath(listOfLinks, source, destination, num_nodes, packetSize);

				   continue;
		}
		case 2:{

				   Link link1 = Link(14, 10, 6, 7);
				   Link link2 = Link(18, 8, 0, 1);
				   Link link3 = Link(18, 5, 1, 3);
				   Link link5 = Link(12, 40, 2, 1);
				   Link link4 = Link(17, 18, 5, 6);
				   Link link6 = Link(18, 15, 0, 4);
				   Link link7 = Link(40, 9, 4, 3);
				   Link link8 = Link(18, 4, 3, 7);
				   Link link9 = Link(17, 20, 5, 7);
				   Link link10 = Link(18, 25, 0, 5);
				   Link link11 = Link(17, 19, 6, 7);
				   Link link12 = Link(12, 41, 0, 2);
				   listOfLinks.push_back(link1);
				   listOfLinks.push_back(link2);
				   listOfLinks.push_back(link3);
				   listOfLinks.push_back(link5);
				   listOfLinks.push_back(link4);
				   listOfLinks.push_back(link6);
				   listOfLinks.push_back(link7);
				   listOfLinks.push_back(link8);
				   listOfLinks.push_back(link9);
				   listOfLinks.push_back(link10);
				   listOfLinks.push_back(link11);
				   listOfLinks.push_back(link12);
				   num_nodes = mesh_size;

				   cout << "Please enter the source: " << endl;
				   cin >> source;
				   cout << "Please enter the destination: " << endl;
				   cin >> destination;
				   cout << "Please enter the packet size in bytes: " << endl;
				   cin >> packetSize;
				   quickestPath(listOfLinks, source, destination, num_nodes, packetSize);

				   continue;
		}
		case 0:{
				   more = false;
		}
		case 3: {
					cout << "Type the name of the file including extension: ";
					string fileName = "";  //User inputs filename
					cin >> fileName;
					ifstream graphFile; //Open user input graph file
					string line = "";
					vector<Link> listOfLinks;
					graphFile.open(fileName);
					if (graphFile.is_open()){
						while (getline(graphFile, line, ',')){
							int start = atoi(line.c_str());
							getline(graphFile, line, ',');
							int end = atoi(line.c_str());
							getline(graphFile, line, ',');
							int delay = atoi(line.c_str());
							getline(graphFile, line, ',');
							int capacity = atoi(line.c_str());
							Link new_link = Link(delay, capacity, start - 1, end - 1);
							listOfLinks.push_back(new_link);
						}
						graphFile.close();
						cout << "Please enter the source: " << endl;
						cin >> source;
						cout << "Please enter the destination: " << endl;
						cin >> destination;
						cout << "Please enter the packet size in bytes: " << endl;
						cin >> packetSize;
						int num_nodes = mesh_size;
						quickestPath(listOfLinks, source, destination, num_nodes, packetSize);
					}
					else{
						cout << "Unable to open that file...";
					}
					continue;
		}
		default: {
					cout << endl << "Not a valid choice" << endl;
					continue;
		}
		}
	}
}

int Dijkstra(vector<vector<int> > graph, int source, int num_nodes, int dest){

	int dist[mesh_size]; // Array to hold distances to nodes from source
	bool visit[mesh_size]; // Array to determine if a node is visited

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
	return dist[dest];
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

vector<int> calculatePath(int dest, int src, int parent[], int num_nodes){
	// Calculate the best path from a source node to a destination node
	vector<int> path;
	if (src == dest) {
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

vector< vector< int > > generateGraph(vector<Link> listOfLinks, int num_nodes){
	// The following steps describe the functions "generateGraph"
	//1. Iterate throght the vector to get to a Link.
	//2. Dimension 1 of dist_graph is the start_of_link node.
	//3. Dimension 2 of dist_graph is the end_of_link node.
	//4. Find the end_of_link spot in array and enter the propogation_delay
	//5. Reverse the array index so that the end_of_link is the first index and the start_of_link is the second index (fliping steps 1-3) to complete the graph
	vector<vector<int> > ring_dist_graph;
	ring_dist_graph.resize(num_nodes, vector<int>(num_nodes));
	for (std::vector<Link>::const_iterator it = listOfLinks.begin(); it != listOfLinks.end(); it++){


		ring_dist_graph[it->start_of_link][it->end_of_link] = it->propagation_delay;

		ring_dist_graph[it->end_of_link][it->start_of_link] = it->propagation_delay;
	}
	//The above loop leaves empty spaces in the graph for distances from all nodes to themselves.  Since that value is always 0, this loop populates the graph with 0s for those distances.
	for (int i = 0; i < num_nodes; i++){
		ring_dist_graph[i][i] = 0;
	}
	return ring_dist_graph;
}


//Function to compare and find the lowest capacity
int compareCapacaties(vector<Link> new_listOfLinks){
	int capacity = INT_MAX;
	for (std::vector<Link>::iterator it2 = new_listOfLinks.begin(); it2 != new_listOfLinks.end(); it2++){

		if ((capacity > it2->capacity)&&(it2->capacityChecked == false)){
			capacity = it2->capacity;
		}
	}
	return capacity;
}

vector<int> getCapacities(vector<int> path, vector<Link> listOfLinks){
	int currentNode = path.back();
	vector<int> capacityList;
	int nextNode;
	for (int i = path.size(); i > 0; i--){
		//Setting the next node
		nextNode = path.at(i - 1);
		for (std::vector<Link>::const_iterator it = listOfLinks.begin(); it != listOfLinks.end(); it++){
			//Looking at the selected link to see if it matches the current node and next node
			if (currentNode == it->start_of_link && nextNode == it->end_of_link){
				capacityList.push_back(it->capacity);
				currentNode = nextNode;
				break;
			}
		}
	}
	return capacityList;
}

float calculateTotalLatency(float packetSize, int propDelay, int capacity){
	float totalLatency = (packetSize / float(capacity)) + propDelay;
	return totalLatency;
}

void quickestPath(vector<Link> listOfLinks, int source, int dest, int num_nodes, float packetSize){

	vector<Link> new_listOfLinks = listOfLinks;
	vector<int> allCapacities;
	vector<Path> listOfPaths;
	int propDelay;
	float totalLatency;
	bool done = false;

	while (!done){

		int lowestCapacity = compareCapacaties(new_listOfLinks);

		if (lowestCapacity == INT_MAX){
			done = true;
			break;
		}
		//Take lowest capacity and eliminate Links with capacity values less than the lowest
		for (std::vector<Link>::iterator it3 = new_listOfLinks.begin(); it3 != new_listOfLinks.end(); /*it3++*/){
			if (lowestCapacity > it3->capacity){
				//it3->propagation_delay = INT_MAX;
				it3 = new_listOfLinks.erase(it3);
			}
			else{
				it3++;
			}
		}
		for (std::vector<Link>::iterator it1 = new_listOfLinks.begin(); it1 != new_listOfLinks.end(); it1++){
			if (it1->capacity == lowestCapacity){
				it1->capacityChecked = true;
			}
		}

		//Running the Dijkstra functions to find the best path
		vector<vector<int> > new_graph = generateGraph(new_listOfLinks, num_nodes);
		propDelay = Dijkstra(new_graph, source, num_nodes, dest);
		vector<int> new_path = calculatePath(dest, source, parent, num_nodes);

		//Calculating total latency for later later comparision
		totalLatency = calculateTotalLatency(packetSize, propDelay, lowestCapacity);

		//Saving the total latency and updated quickest path in a vector for later comparision
		Path path1 = Path();
		path1.totalLatency = totalLatency;
		path1.path = new_path;
		listOfPaths.push_back(path1);
		allCapacities.resize(0);
	}
	displayQuickestPath(listOfPaths);
}

void displayQuickestPath(vector<Path> listOfPaths){
	float lowestLatency = listOfPaths.front().totalLatency;
	Path bestPath = listOfPaths.front();
	for (std::vector<Path>::iterator it = listOfPaths.begin(); it != listOfPaths.end(); it++){
		if (bestPath.totalLatency > it->totalLatency){
			bestPath = *it;
		}
	}
	//Print total latency and the path
	bestPath.display();
	cin.get();
}