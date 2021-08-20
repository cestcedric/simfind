#include <OpenSim/OpenSim.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../include/search.h"
#include "../include/excitation.h"
#include "../include/results.h"
#include "../include/states.h"

using namespace OpenSim;
using namespace SimTK;

int main(int argc, char* argv[]) {
	if (argc > 4) {
		std::string model = argv[1];
		std::string setup = argv[2];
		std::string controls = argv[3];
		std::string initialState = argv[4];
		Search s(setup, 0, 0);
		s.setModel(model);
		s.setControls(controls);
		s.setInitialState(initialState);
		s.setIntervall(0.05);

		s.run();
	}
	else
	{
		int start = std::stoi(argv[1]);
		int end = std::stoi(argv[2]);

		//int start = 0;
		//int end = 10;

		std::cout << "Bereich: " << start << " -> " << end << std::endl;
		
		//////////////////////////////////////////////
		/////set the location of your files here//////
		//////////////////////////////////////////////


		std::string model = "path/to/repository/SimFind/files/model.osim";
		std::string setup = "path/to/repository/SimFind/files/setup.xml";
		std::string controls = "path/to/repository/SimFind/files/controls.xml";
		std::string initialState = "path/to/repository/SimFind/files/output/states/initialState_0.sto";
		
		Search s(setup, start, end);

		s.setModel(model);
		s.setControls(controls);
		s.setInitialState(initialState);
		s.enumerate();
		//s.randomTree();
		//s.totallyrandom();
		s.stepsRRT(10);
		s.triesRRT(1);
		s.setIntervall(0.05);
		s.group();
		s.cleanup();

		s.run();
	}
}
