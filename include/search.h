#pragma once
#include <OpenSim/OpenSim.h>

class Search {

public:
	Search();
	void setModel(std::string);
	void setIntervall(double);
	void setControls(std::string);
	void setInitialState(std::string);
	void run();

private:
	OpenSim::ForwardTool *fwd;
	OpenSim::Model model;
	std::string pathControls;
	std::string pathInitialState;
	//struktur
};