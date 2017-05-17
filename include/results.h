#pragma once
#include <OpenSim/OpenSim.h>
#include <fstream>
#include <vector>
#include <list>
//include the google thingy

class Results {

public:
	Results();
	Results(std::string, int);
	void add(double, double, double);
	std::string print();

private:
	int number;
	std::string path;
};