#pragma once
#include <OpenSim/OpenSim.h>
//include the google thingy

struct point {
	double x;
	double y;
	double z;
};

class Results {

public:
	Results();
	void add(double, double, double);

private:
	int number;
	std::vector<point> points;//contains outer points
	bool isItIn(double, double, double);

	std::string print(int);


};