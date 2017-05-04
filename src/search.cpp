#include "../include/search.h"
#include "../include/excitation.h"
#include "../include/results.h"

#include <math.h>
#include <random>


Search::Search() {
	Search("");
}

Search::Search(std::string setup) {
	pathSetup = setup;
	ex = new Excitation(0);
	std::vector<Results> v(10);
	results = v;
}

void Search::setModel(std::string m) {
	model = OpenSim::Model(m);
}

void Search::setIntervall(double t) {
	intervall = t;
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

	if (b) {//excitations 0 or 1
		for (int i = 0; i < 50; i++) {
			v[i] = (int)(v[i] + 0.5);
		}
	}

	if (g && b) {//groups of muscles, only grouping when also binary excitation
		v[1] = v[2] = v[3] = v[4] = v[0]; //group 1: DELT1, ...
		
		v[7] = v[11] = v[12] = v[13] = v[5]; //group 2: SUBSC, LAT, ...
		
		v[9] = v[10] = v[14] = v[8]; //group 3: PECM, ...
		
		v[16] = v[17] = v[18] = v[15]; //group 4: TRI, ...
		
		v[21] = v[22] = v[23] = v[20]; //group 5: BIC, ...
		
		v[25] = v[24]; //group 6: ECRL, ECRB
		
		v[28] = v[29] = v[30] = v[31] = v[32] = v[33] = v[27];//group 7: FCR, FCU, ...
		v[34] = v[35] = v[36] = v[37] = v[38] = v[39] = v[27];
		v[48] = v[49] = v[27];
		
		v[41] = v[42] = v[43] = v[44] = v[45] = v[46] = v[47] = v[40];//group 8: EDCL, ...
	}

	return v;
}

void Search::run() {
	//for (every initial state) {
	initResults();
	//add initial position to results, or make first simulation with 0 excitation
	
	
	for (int i = 0; i < 1; i++) {
		
		std::clock_t startTime = std::clock();

		fwd = new OpenSim::ForwardTool(pathSetup);
		fwd->setModel(model);
		fwd->setStatesFileName(pathInitialState);

		//generate excitations
		std::vector<double> v = getExcitations();
		excited.push_back(v);//check if already excited like this?

		//generate controls file
		ex->setNumber(i);
		std::string controls = ex->excite(v);

		//configure forward tool
		std::stringstream out;
		out << "output/output_" << i;
		fwd->setResultsDir(out.str());
		fwd->setControlsFileName(controls);

		fwd->run();

		//get results from analysis
		addResults(i);

		std::cout << 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC << std::endl;

	}
	//}
}

void Search::initResults() {
	std::string directory = pathControls.substr(0, pathControls.size() - 12);
	std::stringstream s;
	s << directory << "output/markers";
	for (int i = 0; i < 10; i++) {
		Results r(s.str(), i);
		results[i] = r;
	}
}

//add new final points to results
void Search::addResults(int n) {
	std::string directory = pathControls.substr(0, pathControls.size() - 12);
	std::vector<double> tmp;
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

	ana1 << directory << "output/output_" << n << "/FDS_PointKinematics1_R.Clavicle_pos.sto";
	tmp = readFile(ana1.str());
	results[0].add(tmp[0], tmp[1], tmp[2]);

	ana2 << directory << "output/output_" << n << "/FDS_PointKinematics2_C7_pos.sto";
	tmp = readFile(ana2.str());
	results[1].add(tmp[0], tmp[1], tmp[2]);

	ana3 << directory << "output/output_" << n << "/FDS_PointKinematics3_R.Shoulder_pos.sto";
	tmp = readFile(ana3.str());
	results[2].add(tmp[0], tmp[1], tmp[2]);

	ana4 << directory << "output/output_" << n << "/FDS_PointKinematics4_R.Biceps_pos.sto";
	tmp = readFile(ana4.str());
	results[3].add(tmp[0], tmp[1], tmp[2]);

	ana5 << directory << "output/output_" << n << "/FDS_PointKinematics5_R.Elbow.Lateral_pos.sto";
	tmp = readFile(ana5.str());
	results[4].add(tmp[0], tmp[1], tmp[2]);

	ana6 << directory << "output/output_" << n << "/FDS_PointKinematics6_R.Forearm_pos.sto";
	tmp = readFile(ana6.str());
	results[5].add(tmp[0], tmp[1], tmp[2]);

	ana7 << directory << "output/output_" << n << "/FDS_PointKinematics7_R.Radius_pos.sto";
	tmp = readFile(ana7.str());
	results[6].add(tmp[0], tmp[1], tmp[2]);

	ana8 << directory << "output/output_" << n << "/FDS_PointKinematics8_Handle_pos.sto";
	tmp = readFile(ana8.str());
	results[7].add(tmp[0], tmp[1], tmp[2]);

	ana9 << directory << "output/output_" << n << "/FDS_PointKinematics9_R.Elbow.Medial_pos.sto";
	tmp = readFile(ana9.str());
	results[8].add(tmp[0], tmp[1], tmp[2]);

	ana10 << directory << "output/output_" << n << "/FDS_PointKinematics10_R.Ulna_pos.sto";
	tmp = readFile(ana10.str());
	results[9].add(tmp[0], tmp[1], tmp[2]);
}

std::vector<double>  Search::readFile(std::string path) {
	std::string line;
	std::ifstream tmp;
	tmp.open(path);
	//get number of lines
	std::getline(tmp, line);
	std::getline(tmp, line);
	std::getline(tmp, line);
	int rows = atoi(line.substr(6, line.length()).c_str());
	//go to last line
	for (int i = 3; i < rows + 8; i++) {
		std::getline(tmp, line);
	}
	//split last line to get coordinates
	std::vector<double> position(3);
	//find the decimal dots -> one digit before and 20 after
	int p = line.find(".");
	//remove time
	line.erase(0, p + 21);
	//state_0
	p = line.find(".");
	std::string x = line.substr(p - 2, p + 21);
	line.erase(0, p + 21);
	//state_1
	p = line.find(".");
	std::string y = line.substr(p - 2, p + 21);
	line.erase(0, p + 21);
	//state_2
	p = line.find(".");
	std::string z = line.substr(p - 2, p + 21);

	//save positions in vector
	position[0] = std::stod(x);
	position[1] = std::stod(y);
	position[2] = std::stod(z);

	return position;
}

void Search::group(bool g) {
	this->g = g;
}

void Search::binary(bool b) {
	this->b = b;
}
