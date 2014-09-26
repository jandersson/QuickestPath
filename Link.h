#ifndef LINK_H
#define LINK_H
#include <iostream>

class Link{
public:
	int propagation_delay;
	int capacity;
	int start_of_link;
	int end_of_link;
	bool capacityChecked;
	Link();
	Link(int propagation_delay, int capacity, int start_of_link, int end_of_link);
	~Link();
	void calculateDelay();

};

#endif LINK_H
