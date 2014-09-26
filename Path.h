#ifndef PATH_H
#define PATH_H
#include <iostream>
using namespace std;
#include <vector>
#include "Link.h"

struct Path{

	std::vector<int> path;
	float totalLatency;

	Path();
	~Path();
	void display();
};

#endif