#include <OpenSim/OpenSim.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../include/search.h"
#include "../include/excitation.h"
#include "../include/results.h"

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
		std::string initialState = "E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Code/SimFind/files/CMC_Reach8_states_abbrev.sto";
		//std::string model = "C:/files/model.osim";
		//std::string setup = "C:/files/setup.xml";
		//std::string controls = "C:/files/controls.xml";
		//std::string initialState = "C:/files/CMC_Reach8_states_abbrev.sto";

		Search s(setup);
		s.setModel(model);
		s.setControls(controls);
		s.setInitialState(initialState);
		std::cout << "now run" << std::endl;
		s.setIntervall(0.1);

		s.run();
		//////////////////////////////////////////////
		//////////////////////////////////////////////
		std::cout << "please call the application like this: 'app path/to/model path/to/controlsTemplate path/to/initialStateTemplate'" << std::endl;
		
	}
	
	/*
	try {
		std::string path("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/MoBL_ARMS_module6_7_CMC.osim");
		//std::string path("C:/OpenSim Models/MoBL-ARMS OpenSim tutorial_33/ModelFiles/MoBL_ARMS_module6_7_CMC.osim");
		Model model(path);

		//simulation from 0.62 to 0.72

		//standard controls from download
		ForwardTool *tool = new ForwardTool("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/MoBL_ARMS_module7_FDS_Setup - Kopie.xml");
		//ForwardTool *tool = new ForwardTool("C:/OpenSim Models/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/MoBL_ARMS_module7_FDS_Setup - Kopie.xml");
		tool->setModel(model);
		std::clock_t startTime = std::clock();
		tool->run();
		auto t1 = 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC;
		
		//with a different control file
		startTime = std::clock();
		//tool->setControlsFileName("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/controls.xml");
		//tool->setControlsFileName("C:/OpenSim Models/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/controls.xml");
		//tool->addAnalysisSetToModel();
		//tool->run();
		auto t2 = 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC;

		//with different first states file
		startTime = std::clock();
		//tool->setStatesFileName("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/CMC_Reach8_states_abbrev - Kopie.sto");
		//tool->setStatesFileName("C:/OpenSim Models/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/CMC_Reach8_states_abbrev - Kopie.sto");
		//tool->run();
		auto t3 = 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC;
		//command line, standard controls
		startTime = std::clock();
		//system("cd E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation && forward -S \"MoBL_ARMS_module7_FDS_Setup - Kopie - Kopie.xml\"");
		//system("cd C:/OpenSim Models/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation && forward -S \"MoBL_ARMS_module7_FDS_Setup - Kopie - Kopie.xml\"");
		auto t4 = 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC;
		

		std::cout << "original controls : " << t1 << "\n" << "only biceps excited : " << t2 << "\n" << "different initial state : " << t3  << "\n" << "command line : " << t4 << std::endl;
	}
	catch (OpenSim::Exception ex)
	{
		std::cout << "opensim " << ex.getMessage() << std::endl;
		return 1;
	}
	catch (std::exception ex)
	{
		std::cout << "std " << ex.what() << std::endl;
		return 1;
	}
	catch (...)
	{
		std::cout << "UNRECOGNIZED EXCEPTION" << std::endl;
		return 1;
	}
	*/
}