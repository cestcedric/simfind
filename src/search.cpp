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
	//add initial position to results, or make first simulation with 0 excitation
	fwd->setStatesFileName(pathInitialState);
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
		addResults(i);



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

//add new final points to results
void Search::addResults(int n) {
	std::string directory = fwd->getResultsDir();
	std::string fds = "/FDS_PointKinematics";
	std::vector<double> tmp;

	std::cout << directory << std::endl;
	std::vector<std::stringstream> streams;
	std::stringstream ana1;
	std::stringstream ana2;
	std::stringstream ana3;
	std::stringstream ana4;
	std::stringstream ana5;
	std::stringstream ana6;
	std::stringstream ana7;
	std::stringstream ana8;
	std::stringstream ana9;
	std::stringstream ana10;

	ana1 << directory << "/FDS_PointKinematics1_R.Clavicle_pos.sto";
	tmp = readFile(ana1.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "/FDS_PointKinematics2_C7_pos.sto";
	tmp = readFile(ana2.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "/FDS_PointKinematics3_R.Shoulder_pos.sto";
	tmp = readFile(ana3.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "/FDS_PointKinematics4_R.Biceps_pos.sto";
	tmp = readFile(ana4.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "FDS_PointKinematics5_R.Elbow.Lateral_pos.sto";
	tmp = readFile(ana5.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "/FDS_PointKinematics6_R.Forearm_pos.sto";
	tmp = readFile(ana6.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "/FDS_PointKinematics7_R.Radius_pos.sto";
	tmp = readFile(ana7.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "/FDS_PointKinematics8_Handle_pos.sto";
	tmp = readFile(ana8.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "/FDS_PointKinematics9_R.Elbow.Medial_pos.sto";
	tmp = readFile(ana9.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "/FDS_PointKinematics10_R.Ulna_pos.sto";
	tmp = readFile(ana10.str());
	results[n].add(tmp[0], tmp[1], tmp[2]);
}

std::vector<double>  Search::readFile(std::string path) {
	std::string line;
	std::ifstream tmp;
	tmp.open(path);
	std::getline(tmp, line);
	std::getline(tmp, line);
	std::getline(tmp, line);
}
