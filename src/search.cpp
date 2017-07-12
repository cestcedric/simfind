#include "../include/search.h"
#include "../include/excitation.h"
#include "../include/results.h"
#include "../include/states.h"

#include <math.h>
#include <random>
#include <Windows.h>


Search::Search() {
	Search("",0,0);
}

Search::Search(std::string setup, int start, int end) {
	pathSetup = setup;
	ex = new Excitation(0);
	std::vector<Results> v(10);
	results = v;
	b = c = g = e = r = rrt = rrtp = false;
	steps = 5;
	iterations = end - start;
	this->start = start;
	this->end = end;
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
		v[6] = (n / 512) % 2;
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
			if (r) {
				randomSearch();
			}
			else
			{
				customRRT();
			}
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
	for (int i = 0; i < 1024; i++) {

		//reload model everytime because it slows down otherwise
		OpenSim::Model model = OpenSim::Model(pathModel);
		OpenSim::ForwardTool *fwd = new OpenSim::ForwardTool(pathSetup);
		fwd->setModel(model);
		fwd->setStatesFileName(pathInitialState);
		fwd->setInitialTime(0);
		fwd->setFinalTime(intervall);

		//generate excitations
		std::vector<double> v = getExcitations(i);

		//generate controls file
		ex->setNumber(i);
		std::string controls = ex->excite(v);

		//configure forward tool
		std::stringstream out;
		out << "output/output_" << i;
		fwd->setResultsDir(out.str());
		//fwd->setControlsFileName(controls);

		fwd->run();

		//get results from analysis
		addResults(i);

		//delete unneeded files
		if (c) {
			cleanuponaisle(i);
		}
	}
}

void Search::standardRRT() {
	iterations = end - start;
	b = true;

	int numOfStates = start + 1;
	States st;

	std::string directory = pathControls.substr(0, pathControls.size() - 12);
	std::string next;


	//////////////////////////////////////////////////
	//////////////////////////////////////////////////
	//debugging
	std::stringstream controlOutput;
	controlOutput << directory << "output/states.txt";
	//////////////////////////////////////////////////
	//////////////////////////////////////////////////

	for (int i = start; i < end; i++) {
		
		//find target point
		std::vector<double> random = setAngles();
		std::cout << "random" << std::endl;

		//get new initial state
		next = findNext(directory, numOfStates, random);

		for (int k = 0; k < tries; k++) {
			//reload model every iteration to avoid "contamination"
			OpenSim::Model model = OpenSim::Model(pathModel);

			OpenSim::ForwardTool *fwd = new OpenSim::ForwardTool(pathSetup);
			fwd->setModel(model);

			//new initital state
			fwd->setStatesFileName(next);

			//////////////////////////////////////////////////
			//////////////////////////////////////////////////
			if (k == 0) {
				std::ofstream states(controlOutput.str(), std::ios_base::app);
				states << fwd->getStatesFileName() << std::endl;
			}
			//////////////////////////////////////////////////
			//////////////////////////////////////////////////

			//random excitations
			std::vector<double> v = getExcitations(i);
			ex->setNumber(i);
			std::string controls = ex->excite(v);

			//set output
			std::stringstream out;
			out << "output/output_" << i << "_" << k;
			fwd->setResultsDir(out.str());

			//set time intervall
			fwd->setInitialTime(0);
			fwd->setFinalTime(intervall / (double)steps);

			//run
			fwd->run();			

			//temporarily save final state
			std::stringstream newState;
			std::stringstream results;
			newState << directory << "output/states/initialState_" << i + 1 << "_" << k << ".sto";
			results << directory << "output/output_" << i << "_" << k << "/FDS_states.sto";
			st.save(results.str(), newState.str(), st.getRank(next) + 1);
		}

		//find best final state
		//////////////////////////////////////////////////
		//////////////////////////////////////////////////
		std::ofstream states(controlOutput.str(), std::ios_base::app);
		//////////////////////////////////////////////////
		//////////////////////////////////////////////////
		int bestState = 0;
		std::stringstream b;
		b << directory << "output/states/initialState_" << i + 1 << "_" << 0 << ".sto";
		std::string bestStateName = b.str();
		double d = distance(random, b.str());
		double min = d;
		for (int k = 0; k < tries; k++) {
			std::stringstream best;
			best << directory << "output/states/initialState_" << i + 1 << "_" << k << ".sto";

			d = distance(random, best.str());

			//////////////////////////////////////////////////
			//////////////////////////////////////////////////
			states << d << " : " << best.str() << std::endl;
			//////////////////////////////////////////////////
			//////////////////////////////////////////////////

			if (d < min) {
				min = d;
				bestState = k;
				bestStateName = best.str();
			}
		}

		//////////////////////////////////////////////////
		//////////////////////////////////////////////////
		states << "\t -> " << bestState << std::endl;
		//////////////////////////////////////////////////
		//////////////////////////////////////////////////

		////reload model every iteration to avoid "contamination"
		//OpenSim::Model model = OpenSim::Model(pathModel);

		//OpenSim::ForwardTool *fwd = new OpenSim::ForwardTool(pathSetup);
		//fwd->setModel(model);
		//
		////find target point
		//std::vector<double> target = setAngles();

		////get new initial state
		//next = findNext(directory, numOfStates, target);
		//fwd->setStatesFileName(next);

		////random excitations
		//std::vector<double> v = getExcitations(i);
		//ex->setNumber(i);
		//std::string controls = ex->excite(v);

		////set output
		//std::stringstream out;
		//out << "output/output_" << i;
		//fwd->setResultsDir(out.str());

		////set time intervall
		//fwd->setInitialTime(0);
		//fwd->setFinalTime(intervall / (double) steps);


		////run
		//fwd->run()

		//move best state to folder without subscript, to use other methods
		std::stringstream sub;
		std::stringstream fin;
		sub << directory << "output/output_" << i << "_" << bestState;
		fin << directory << "output/output_" << i;

		MoveFile(sub.str().c_str(), fin.str().c_str());
		
		//delete old stuff
		for (int k = 0; k < tries; k++) {
			cleanSteps(i, k);
		}

		//add final marker positions
		addResults(i);

		//save final state in RRT
		std::stringstream newState;
		std::stringstream results;
		newState << directory << "output/states/initialState_" << i + 1 << ".sto";
		results << directory << "output/output_" << i << "/FDS_states.sto";
		st.save(results.str(), newState.str(), st.getRank(next) + 1);

		numOfStates++;

		if (c) {
			cleanuponaisle(i);
		}
	}
}

void Search::cleanup() {
	c = true;
}

void Search::cleanSteps(int i, int j) {
	std::stringstream step;
	step << pathControls.substr(0, pathControls.size() - 12);
	step << "output/output_" << i << "_" << j;
	std::vector<std::string> filenames(33);
	filenames[0] = "FDS_controls.sto";
	filenames[1] = "FDS_PointKinematics1_R.Clavicle_acc.sto";
	filenames[2] = "FDS_PointKinematics1_R.Clavicle_pos.sto";
	filenames[3] = "FDS_PointKinematics1_R.Clavicle_vel.sto";
	filenames[4] = "FDS_PointKinematics2_C7_acc.sto";
	filenames[5] = "FDS_PointKinematics2_C7_pos.sto";
	filenames[6] = "FDS_PointKinematics2_C7_vel.sto";
	filenames[7] = "FDS_PointKinematics3_R.Shoulder_acc.sto";
	filenames[8] = "FDS_PointKinematics3_R.Shoulder_pos.sto";
	filenames[9] = "FDS_PointKinematics3_R.Shoulder_vel.sto";
	filenames[10] = "FDS_PointKinematics4_R.Biceps_acc.sto";
	filenames[11] = "FDS_PointKinematics4_R.Biceps_pos.sto";
	filenames[12] = "FDS_PointKinematics4_R.Biceps_vel.sto";
	filenames[13] = "FDS_PointKinematics5_R.Elbow.Lateral_acc.sto";
	filenames[14] = "FDS_PointKinematics5_R.Elbow.Lateral_pos.sto";
	filenames[15] = "FDS_PointKinematics5_R.Elbow.Lateral_vel.sto";
	filenames[16] = "FDS_PointKinematics6_R.Forearm_acc.sto";
	filenames[17] = "FDS_PointKinematics6_R.Forearm_pos.sto";
	filenames[18] = "FDS_PointKinematics6_R.Forearm_vel.sto";
	filenames[19] = "FDS_PointKinematics7_R.Radius_acc.sto";
	filenames[20] = "FDS_PointKinematics7_R.Radius_pos.sto";
	filenames[21] = "FDS_PointKinematics7_R.Radius_vel.sto";
	filenames[22] = "FDS_PointKinematics8_Handle_acc.sto";
	filenames[23] = "FDS_PointKinematics8_Handle_pos.sto";
	filenames[24] = "FDS_PointKinematics8_Handle_vel.sto";
	filenames[25] = "FDS_PointKinematics9_R.Elbow.Medial_acc.sto";
	filenames[26] = "FDS_PointKinematics9_R.Elbow.Medial_pos.sto";
	filenames[27] = "FDS_PointKinematics9_R.Elbow.Medial_vel.sto";
	filenames[28] = "FDS_PointKinematics10_R.Ulna_acc.sto";
	filenames[29] = "FDS_PointKinematics10_R.Ulna_pos.sto";
	filenames[30] = "FDS_PointKinematics10_R.Ulna_vel.sto";
	filenames[31] = "FDS_states.sto";
	filenames[32] = "FDS_states_degrees.mot";

	for (int k = 0; k < 33; k++) {
		std::stringstream here;
		here << step.str() << "/" << filenames[k];
		DeleteFile(here.str().c_str());
	}
	RemoveDirectory(step.str().c_str());

	std::stringstream statefile;
	statefile << pathControls.substr(0, pathControls.size() - 12) << "output/states/initialState_" << i + 1 << "_" << j << ".sto";
	DeleteFile(statefile.str().c_str());
}

void Search::cleanuponaisle(int i) {
	std::stringstream aisle;
	aisle << pathControls.substr(0, pathControls.size() - 12);
	aisle << "output/output_" << i;
	std::vector<std::string> filenames(33);
	filenames[0] = "FDS_controls.sto";
	filenames[1] = "FDS_PointKinematics1_R.Clavicle_acc.sto";
	filenames[2] = "FDS_PointKinematics1_R.Clavicle_pos.sto";
	filenames[3] = "FDS_PointKinematics1_R.Clavicle_vel.sto";
	filenames[4] = "FDS_PointKinematics2_C7_acc.sto";
	filenames[5] = "FDS_PointKinematics2_C7_pos.sto";
	filenames[6] = "FDS_PointKinematics2_C7_vel.sto";
	filenames[7] = "FDS_PointKinematics3_R.Shoulder_acc.sto";
	filenames[8] = "FDS_PointKinematics3_R.Shoulder_pos.sto";
	filenames[9] = "FDS_PointKinematics3_R.Shoulder_vel.sto";
	filenames[10] = "FDS_PointKinematics4_R.Biceps_acc.sto";
	filenames[11] = "FDS_PointKinematics4_R.Biceps_pos.sto";
	filenames[12] = "FDS_PointKinematics4_R.Biceps_vel.sto";
	filenames[13] = "FDS_PointKinematics5_R.Elbow.Lateral_acc.sto";
	filenames[14] = "FDS_PointKinematics5_R.Elbow.Lateral_pos.sto";
	filenames[15] = "FDS_PointKinematics5_R.Elbow.Lateral_vel.sto";
	filenames[16] = "FDS_PointKinematics6_R.Forearm_acc.sto";
	filenames[17] = "FDS_PointKinematics6_R.Forearm_pos.sto";
	filenames[18] = "FDS_PointKinematics6_R.Forearm_vel.sto";
	filenames[19] = "FDS_PointKinematics7_R.Radius_acc.sto";
	filenames[20] = "FDS_PointKinematics7_R.Radius_pos.sto";
	filenames[21] = "FDS_PointKinematics7_R.Radius_vel.sto";
	filenames[22] = "FDS_PointKinematics8_Handle_acc.sto";
	filenames[23] = "FDS_PointKinematics8_Handle_pos.sto";
	filenames[24] = "FDS_PointKinematics8_Handle_vel.sto";
	filenames[25] = "FDS_PointKinematics9_R.Elbow.Medial_acc.sto";
	filenames[26] = "FDS_PointKinematics9_R.Elbow.Medial_pos.sto";
	filenames[27] = "FDS_PointKinematics9_R.Elbow.Medial_vel.sto";
	filenames[28] = "FDS_PointKinematics10_R.Ulna_acc.sto";
	filenames[29] = "FDS_PointKinematics10_R.Ulna_pos.sto";
	filenames[30] = "FDS_PointKinematics10_R.Ulna_vel.sto";
	filenames[31] = "FDS_states.sto";
	filenames[32] = "FDS_states_degrees.mot";

	for (int k = 0; k < 33; k++) {
		std::stringstream here;
		here << aisle.str() << "/" << filenames[k];
		DeleteFile(here.str().c_str());
	}
	RemoveDirectory(aisle.str().c_str());

	std::stringstream controls;
	controls << pathControls.substr(0, pathControls.size() - 12) << "Controls/controls_" << i << ".xml";
	DeleteFile(controls.str().c_str());
}

void Search::customRRT() {

}

void Search::randomSearch() {
	b = true;
	g = false;

	std::string directory = pathControls.substr(0, pathControls.size() - 12);
	std::string next;

	for (int i = start; i < end; i++) {
			//reload model every iteration to avoid "contamination"
			OpenSim::Model model = OpenSim::Model(pathModel);

			OpenSim::ForwardTool *fwd = new OpenSim::ForwardTool(pathSetup);
			fwd->setModel(model);

			//new initital state
			fwd->setStatesFileName(pathInitialState);

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
			fwd->setFinalTime(intervall);

			//run
			fwd->run();

			//get results from analysis
			addResults(i);

			//delete unneeded files
			if (c) {
				cleanuponaisle(i);
			}
		}
}

//best state = closest to random generated position AND time still smaller than the intervall
// => take state farther state if nearest is too late
std::string Search::findNext(std::string directory, int n, std::vector<double> random) {
	std::string best;
	States st;

	std::stringstream init;
	init << directory << "output/states/initialState_0.sto";

	double minDist = distance(random, init.str()) + 1;
	
	//find nearest old state nos
	for (int s = 0; s < n; s++) {
		std::stringstream nos;
		nos << directory << "output/states/initialState_" << s << ".sto";

		double d = distance(random, nos.str());

		if (st.getRank(nos.str()) <= steps && d < minDist) {//closest state with still time to go
			minDist = d;
			best = nos.str();
		}
	}
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

	std::ofstream angle("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/output/angles.txt", std::ios_base::app);
	//std::ofstream angle("C:/files/output/angles.txt", std::ios_base::app);
	angle << angles[0] << "\t" << angles[1] << "\t" << angles[2] << "\t" << angles[3] << "\t" << angles[4] << "\t" << angles[5] << "\t" << angles[6] << "\t" << std::endl;

	return angles;
}

//euclidean distance
//manhattan distance
double Search::distance(std::vector<double> r, std::string s) {
	States t;
	std::vector<double> a = t.getAngles(s);

	double d = 0;

	/*
	for (int i = 0; i < a.size(); i++) {
		d = d + pow((r[i] - a[i]), 2);
	}

	d = sqrt(d);
	*/
	
	for (int i = 0; i < a.size(); i++) {
		d = d + abs(r[i] - a[i]);
	}
	
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

void Search::group() {
	this->g = true;
}

void Search::binary() {
	this->b = true;
}

void Search::enumerate() {
	this->e = true;
}

void Search::randomTree() {
	this->rrt = true;
}

void Search::randomwithbenefits() {
	this->rrtp = true;
}

void Search::stepsRRT(int steps) {
	this->steps = steps;
}

void Search::iterationsRRT(int iterations) {
	this->iterations = iterations;
}

void Search::totallyrandom() {
	this->r = true;
}

void Search::triesRRT(int tries) {
	this->tries = tries;
}