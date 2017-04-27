#include "../include/search.h"
#include "../include/excitation.h"
#include "../include/results.h"

#include <math.h>
#include <random>


Search::Search() {
	Search("");
}

Search::Search(std::string setup) {
	fwd = new OpenSim::ForwardTool(setup);
	ex = new Excitation(0);
}

void Search::setModel(std::string m) {
	model = OpenSim::Model(m);
	fwd->setModel(model);
}

void Search::setIntervall(double t) {
	fwd->setInitialTime(0);
	fwd->setFinalTime(t);
}

void Search::setControls(std::string path) {
	pathControls = path;
	ex->setTemplate(path);
}

void Search::setInitialState(std::string path) {
	pathInitialState = path;
}

std::vector<double> Search::getExcitations() {
	std::vector<double> v(50);

	//http://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	std::random_device rd;
	std::mt19937 gen(rd());
	//dis generates in [a,b), so no excitation of exactly 1 possible
	//might want to change that
	std::uniform_real_distribution<> dis(0, 1);

	for (int i = 0; i < 50; i++) {
		v[i] = dis(gen);
	}
	return v;
}

void Search::run() {
	//for (every initial state) {
	//fwd->setStatesFileName(pathInitialState);
	for (int i = 0; i < 3; i++) {
		std::clock_t startTime = std::clock();

		//generate excitations
		std::vector<double> v = getExcitations();
		done.push_back(v);//check if already excited like this?

		//generate controls file
		ex->setNumber(i);
		std::string controls = ex->excite(v);

		//configure forward tool
		std::stringstream out;
		out << "output/output_" << i;
		fwd->setResultsDir(out.str());
		fwd->setControlsFileName(controls);
		//setIntervall(0.1);

		fwd->run();

		//get results from analysis
		std::cout << 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC << std::endl;
	}
	//}
}

void Search::initResults() {
	for (int i = 0; i < 10; i++) {
		Results r;
		results.push_back(r);
	}
}

void Search::addResults(int n) {
	//if n = -1 add the inital position to result
	std::ifstream tmp;
	if (n == -1) {

	}
	else
	{
		
	}
}
