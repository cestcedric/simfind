#pragma once
//#include <OpenSim/OpenSim.h>
#include <fstream>
#include <sstream>
//#include <string>
//#include <vector>
//#include <list>

class Results {

public:
	Results();
	Results(std::string, int);
	void add(double, double, double);

private:
	int number;
	std::string path;
};