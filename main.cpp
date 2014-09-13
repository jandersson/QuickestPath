#include <iostream>
#include "Link.h"
#include "Path.h"
using namespace std;
#include <string>
int main(){
	// Include boolean value in link vector to determine whether or not the algorithm should use the link
	// Need a function to compare all c values and find the lowest c and update the boolean value in the corresponding link vectors with a higher C
	// Write the shortest path algorithm with modification to include destination and ignoring links with boolean value set to false
	// When the algorithm computes the shortest path, store that path somewhere
	// Use the function to compare all C's again but of only links with true boolean values
	// If the algorithm can't reach the destination due to false boolean values perform the calculations on the found paths
	// 
	bool more = true;
	std::string link_start;
	std::string link_end;
	int capacity;
	int propogation_delay;

	while (more){
		std::cout << "Would you like to create a link? [1 = Yes/ 0 = No]";
		std::cin >> more;
		if (more){
			Link* link = new Link();
			std::cout << "Enter the start of the link: ";
			cin >> link_start;
			link->start_of_link = link_start;
			std::cout << "Enter the end of the link: ";
			cin >> link_end;
			link->end_of_link = link_end;
			std::cout << "Enter the propogation delay of the link: ";
			cin >> propogation_delay;
			link->propagation_delay = propogation_delay;
			std::cout << "Enter the capacity of the link: ";
			cin >> capacity;
			link->capacity = capacity;

		}

		[link1, link2, link3, link4]
	}
}