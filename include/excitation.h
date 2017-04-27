#pragma once
#include <OpenSim/OpenSim.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

class Excitation {

public:
	Excitation(int);
	void setNumber(int);
	void setTemplate(std::string);
	std::string excite(std::vector<double>);

private:
	int number;
	int numberOfLines;
	std::string path;
	std::vector<double> excitations;
	std::vector<std::string> controls;

	std::string print();
	

};