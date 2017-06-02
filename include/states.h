#pragma once
#include <fstream>
#include <sstream>
#include <vector>

class States {
	
public:
	States();

	int getRank(std::string);
	std::vector<double> getAngles(std::string);
	void save(std::string, std::string, int);

};