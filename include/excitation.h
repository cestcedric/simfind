#pragma once
#include <OpenSim/OpenSim.h>

class Excitation {

public:
	Excitation(int);
	void setTemplate(std::string);
	std::string excite(std::vector<double>);

private:
	int number;
	std::vector<double> excitations;
	//vorlage

	std::string print();
	

};