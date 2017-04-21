#pragma once
#include <OpenSim/OpenSim.h>

class InitialState {

public:
	InitialState(int);
	void setTemplate(std::string);
	std::string create(std::vector<double>);

private:
	int number;
	std::vector<double> data;
	//vorlage

	std::string print();


};