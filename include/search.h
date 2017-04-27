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
	Excitation *ex;
	std::string pathControls;
	std::string pathInitialState;
	//to save already used excitations, not sure if necessary
	std::vector<std::vector<double>> done;

	std::vector<double> getExcitations();
};