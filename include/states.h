#pragma once
#include <fstream>
#include <sstream>
#include <vector>

class States {
	
public:
	States(std::string, std::string, int);

	int getRank(std::string);
	std::vector<double> getAngles(std::string);

};