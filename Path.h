#ifndef PATH_H
#define PATH_H
#include <iostream>
using namespace std;
#include <vector>
#include "Link.h"

class Path{
private:
	std::vector<Link> linkList;
	int lowestCapacity;
	int totalPropogationDelay;
	float totalLatency;
public:	
	Path();
	~Path();
	int compareC();
	int totalPropogationDelay();
	float totalLatency();
	void display();

};

#endif PATH_H