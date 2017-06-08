#pragma once
#include <OpenSim/OpenSim.h>
#include <list>
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
	void group(bool);
	void binary(bool);
	void enumerate(bool);
	void random(bool);
	void randomwithbenefits(bool);

	void stepsRRT(int);
	void iterationsRRT(int);

private:
	//OpenSim::ForwardTool *fwd;
	OpenSim::Model model;
	Excitation *ex;
	std::string pathModel;
	std::string pathControls;
	std::string pathInitialState;
	std::string pathSetup;
	std::vector<Results> results;
	
	double intervall;

	bool b;
	bool g;
	bool e;
	bool rrt;
	bool rrtp;
	int steps;
	int iterations;

	std::vector<double> getExcitations(int n);
	void addResults(int n);
	std::vector<double> readFile(std::string);

	std::vector<double> setAngles();

	std::string findNext(std::string, int);
	
	double distance(std::vector<double>, std::string);
	void initResults();
	void cleanuponaisle(int);

	//search methods
	void enumerateSearch();
	void standardRRT();
	void customRRT();
};