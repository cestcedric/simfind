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
		Search s(setup);
		s.setModel(model);
		s.setControls(controls);
		s.setInitialState(initialState);
		s.setIntervall(0.1);

		s.run();
	}
	else
	{
		
		//////////////////////////////////////////////
		//////////////////////////////////////////////
		std::string model = "E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/model.osim";
		std::string setup = "E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/setup.xml";
		std::string controls = "E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/controls.xml";
		//std::string initialState = "E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/CMC_Reach8_states_abbrev.sto";
		std::string initialState = "E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/output/states/initialState_0.sto";
		//std::string model = "C:/files/model.osim";
		//std::string setup = "C:/files/setup.xml";
		//std::string controls = "C:/files/controls.xml";
		//std::string initialState = "C:/files/CMC_Reach8_states_abbrev.sto";
		
		Search s(setup);
		
		s.setModel(model);
		s.setControls(controls);
		s.setInitialState(initialState);
		//s.enumerate(true);
		s.random(true);//RRT
		s.stepsRRT(10);
		s.setIntervall(0.1);
		s.iterationsRRT(50000);

		s.run();
		
		DeleteFile("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/output/test/file.txt");
		RemoveDirectory("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/output/test");
		

		//States t;
		//t.getAngles(initialState);
		
		//std::string from = "E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/output_Clustering/output_16/FDS_states.sto";
		//std::string to = "E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/output/states/test.sto";



		//////////////////////////////////////////////
		//////////////////////////////////////////////
		std::cout << "please call the application like this: 'app path/to/model path/to/controlsTemplate path/to/initialStateTemplate'" << std::endl;
		
	}
}