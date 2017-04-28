#pragma once
#include <OpenSim/OpenSim.h>
#include "results.h"
#include "excitation.h"

class Search {

public:
	Search();
	Search(std::string);
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
	std::vector<Results> results;
	//to save already used excitations, not sure if necessary
	std::vector<std::vector<double>> done;

	std::vector<double> getExcitations();
	void addResults(int n);
	std::vector<double> readFile(std::string);

	void initResults();
};