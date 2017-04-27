#pragma once
#include <OpenSim/OpenSim.h>
#include "../soplex/src/soplex.h"

struct point {
	double x;
	double y;
	double z;
};

class Results {

public:
	Results(int);

private:
	int number;
	std::vector<point> points;//contains outer points
	bool isItIn(int, int, int);
	void add(int, int, int);

	std::string print();


};