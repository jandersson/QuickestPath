#include "Link.h"

Link::Link(){
}

Link::Link(int prop, int cap, int start, int end){
	propagation_delay = prop;
	capacity = cap;
	start_of_link = start;
	end_of_link = end;
	capacityChecked = false;
}

Link::~Link(){

}

