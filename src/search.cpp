#include "../include/search.h"
#include "../include/excitation.h"
#include "../include/results.h"

#include <math.h>
#include <random>


Search::Search() {
	fwd = new OpenSim::ForwardTool();
	ex = new Excitation(0);
}

void Search::setModel(std::string m) {
	model = OpenSim::Model(m);
}

void Search::setIntervall(double t) {

}

void Search::setControls(std::string path) {
	pathControls = path;
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
	for (int i = 0; i < 50; i++) {
		//generate excitations
		std::vector<double> v = getExcitations();
		done.push_back(v);//check if already excited like this?

		//generate controls file
		ex->setNumber(i);
		ex->excite(v);

		//configure forward tool

		//get results from analysis
	}
}
