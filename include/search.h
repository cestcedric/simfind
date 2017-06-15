#pragma once
#include <OpenSim/OpenSim.h>
#include <list>
#include "results.h"
#include "excitation.h"

class Search {

public:
	Search();
	Search(std::string, int, int);
	void setModel(std::string);
	void setIntervall(double);
	void setControls(std::string);
	void setInitialState(std::string);
	void run();
	void group();
	void binary();
	void enumerate();
	void randomTree();
	void randomwithbenefits();
	void totallyrandom();

	void stepsRRT(int);
	void triesRRT(int);

	void cleanup();

private:
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
	bool r;
	int steps;
	int iterations;
	int tries;

	int start;
	int end;

	bool c;

	void iterationsRRT(int);

	std::vector<double> getExcitations(int n);
	void addResults(int n);
	std::vector<double> readFile(std::string);

	std::vector<double> setAngles();

	std::string findNext(std::string, int, std::vector<double>);
	
	double distance(std::vector<double>, std::string);
	void initResults();
	void cleanuponaisle(int);
	void cleanSteps(int, int);

	//search methods
	void enumerateSearch();
	void standardRRT();
	void customRRT();
	void randomSearch();
};