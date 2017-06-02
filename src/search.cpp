#include "../include/search.h"
#include "../include/excitation.h"
#include "../include/results.h"
#include "../include/states.h"

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
	b = g = e = rrt = rrtp = false;
	steps = 5;
	iterations = 2500;
}

void Search::setModel(std::string m) {
	pathModel = m;
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

std::vector<double> Search::getExcitations(int n) {
	std::vector<double> v(50);

	//http://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
	std::random_device rd;
	std::mt19937 gen(rd());
	//dis generates in [a,b), so no excitation of exactly 1 possible
	//might want to change that
	std::uniform_real_distribution<> dis(0, 1);

	if (!e) {
		for (int i = 0; i < 50; i++) {
			v[i] = dis(gen);
		}

		if (b) {//excitations 0 or 1
			for (int i = 0; i < 50; i++) {
				v[i] = (int)(v[i] + 0.5);
			}
		}
	}
	else
	{
		v[0] = n % 2;
		v[5] = (n / 2) % 2;
		v[8] = (n / 4) % 2;
		v[15] = (n / 8) % 2;
		v[20] = (n / 16) % 2;
		v[24] = (n / 32) % 2;
		v[27] = (n / 64) % 2;
		v[40] = (n / 128) % 2;
		v[19] = (n / 256) % 2;
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

		v[41] = v[42] = v[43] = v[45] = v[46] = v[47] = v[40];//group 8: EDCL, ...

		v[44] = v[26] = v[19];//group 9: ECU, SUP, EDM
	}

	return v;
}

void Search::run() {
	initResults();
	if (e) {
		enumerateSearch();
	}
	else
	{
		if (rrt) {
			standardRRT();
		}
		else
		{
			customRRT();
		}
	}
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

void Search::enumerateSearch() {
	b = true;
	g = true;
	for (int i = 0; i < 512; i++) {

		//std::clock_t startTime = std::clock();

		//reload model everytime because it slows down otherwise
		model = OpenSim::Model(pathModel);

		OpenSim::ForwardTool *fwd = new OpenSim::ForwardTool(pathSetup);

		//auto t1 = std::clock() - startTime;
		//startTime = std::clock();

		fwd->setModel(model);

		//auto t2 = std::clock() - startTime;
		//startTime = std::clock();

		fwd->setStatesFileName(pathInitialState);

		//auto t3 = std::clock() - startTime;
		//startTime = std::clock();

		//generate excitations
		std::vector<double> v = getExcitations(i);

		//auto t4 = std::clock() - startTime;
		//startTime = std::clock();

		//generate controls file
		ex->setNumber(i);

		//auto t5 = std::clock() - startTime;
		//startTime = std::clock();

		std::string controls = ex->excite(v);

		//auto t6 = std::clock() - startTime;
		//startTime = std::clock();

		//configure forward tool
		std::stringstream out;
		out << "output/output_" << i;

		//auto t7 = std::clock() - startTime;
		//startTime = std::clock();

		fwd->setResultsDir(out.str());

		//auto t8 = std::clock() - startTime;
		//startTime = std::clock();

		//fwd->setControlsFileName(controls);
		
		//auto t9 = std::clock() - startTime;
		//startTime = std::clock();

		fwd->run();
		
		//auto t10 = std::clock() - startTime;
		//startTime = std::clock();

		//get results from analysis
		addResults(i);
		
		//auto t11 = std::clock() - startTime;

		//std::ofstream times("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/output/times.txt", std::ios_base::app);
		//times << 1.e3*t1 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t2 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t3 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t4 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t5 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t6 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t7 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t8 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t9 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t10 / CLOCKS_PER_SEC << "\t\t";
		//times << 1.e3*t11 / CLOCKS_PER_SEC << "\n";
		//times.close();
	}
}

void Search::standardRRT() {
	b = true;
	g = false;
	e = false;

	int numOfStates = 1;
	States st;

	std::string directory = pathControls.substr(0, pathControls.size() - 12);
	std::string next;

	for (int i = 0; i < iterations; i++) {

		//reload model every iteration to avoid "contamination"
		model = OpenSim::Model(pathModel);

		OpenSim::ForwardTool *fwd = new OpenSim::ForwardTool(pathSetup);
		fwd->setModel(model);
		
		//get new initial state
		next = findNext(directory, numOfStates);
		fwd->setStatesFileName(next);

		//random excitations
		std::vector<double> v = getExcitations(i);
		ex->setNumber(i);
		std::string controls = ex->excite(v);

		//set output
		std::stringstream out;
		out << "output/output_" << i;
		fwd->setResultsDir(out.str());

		//set time intervall
		fwd->setInitialTime(0);
		fwd->setFinalTime(intervall / (double) steps);


		//run
		fwd->run();

		//add final marker positions
		addResults(i);

		//save final state in RRT
		std::stringstream newState;
		std::stringstream results;
		newState << directory << "output/states/initialState_" << i + 1 << ".sto";
		results << directory << "output/output_" << i << "/FDS_states.sto";
		st.save(results.str(), newState.str(), st.getRank(next) + 1);

		numOfStates++;
	}
}

void Search::customRRT() {

}

//best state = closest to random generated position AND time still smaller than the intervall
// => take state farther state if nearest is too late
std::string Search::findNext(std::string directory, int n) {
	std::string best;
	States st;
	
	//generate random state = angles
	std::vector<double> random = setAngles();

	std::stringstream init;
	init << directory << "output/states/initialState_0.sto";

	double minDist = distance(random, init.str()) + 1;

	std::cout << steps << std::endl;
	
	//find nearest old state nos
	for (int s = 0; s < n; s++) {
		std::stringstream nos;
		nos << directory << "output/states/initialState_" << s << ".sto";

		double d = distance(random, nos.str());

		if (st.getRank(nos.str()) <= steps && d < minDist) {//closest state with still time to go
			std::cout << "drin" << std::endl;
			minDist = d;
			best = nos.str();
		}
	}
	std::cout << "best : " << best << std::endl;
	return best;
}

std::vector<double> Search::setAngles() {
	std::vector<double> angles(7);

	std::random_device rd;
	std::mt19937 gen(rd());

	//because of the way this library function works, the outcoming value is [a, b)
	//it's really close though
	std::uniform_real_distribution<> elv_angle(-1.5708, 2.26893);
	std::uniform_real_distribution<> shoulder_elv(0, 3.14159);
	std::uniform_real_distribution<> shoulder_rot(-1.5708, 0.349066);
	std::uniform_real_distribution<> elbow_flexion(0, 2.26893);
	std::uniform_real_distribution<> pro_sup(-1.5708, 1.4708);
	std::uniform_real_distribution<> deviation(-0.174533, 0.436332);
	std::uniform_real_distribution<> flexion(-1.22173, 1.22173);

	angles[0] = elv_angle(gen);
	angles[1] = shoulder_elv(gen);
	angles[2] = shoulder_rot(gen);
	angles[3] = elbow_flexion(gen);
	angles[4] = pro_sup(gen);
	angles[5] = deviation(gen);
	angles[6] = flexion(gen);

	return angles;
}

//euclidean distance
double Search::distance(std::vector<double> r, std::string s) {
	States t;
	std::vector<double> a = t.getAngles(s);

	double d = 0;

	for (int i = 0; i < a.size(); i++) {
		d = d + pow((r[i] - a[i]), 2);
	}

	d = sqrt(d);

	return d;
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

void Search::enumerate(bool e) {
	this->e = e;
}

void Search::random(bool rrt) {
	this->rrt = rrt;
}

void Search::randomwithbenefits(bool rrtp) {
	this->rrtp = rrtp;
}

void Search::stepsRRT(int steps) {
	this->steps = steps;
}

void Search::iterationsRRT(int iterations) {
	this->iterations = iterations;
}