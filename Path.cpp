#include "Path.h"
#include <vector>

Path::Path(){
}
Path::~Path(){
}

void Path::display(){
	cout << "Total Latency: " << totalLatency << " ns" << endl;
	cout << "Path is: ";
	for (std::vector<int>::reverse_iterator it = path.rbegin(); it != path.rend(); it++){
		cout << *it + 1;
		if (it != path.rend() - 1){
			cout << "->";
		}
	}
	cout << endl;
}
