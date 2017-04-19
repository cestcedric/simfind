#pragma once
#include <OpenSim/OpenSim.h>

class Search {

public:
	Search();
	void setModel(std::string);
	void setIntervall(double);

private:
	OpenSim::ForwardTool fwd;
	OpenSim::Model model;
	std::string pathControls;
	std::string pathInitialState;


};