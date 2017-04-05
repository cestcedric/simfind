#include <OpenSim/OpenSim.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace OpenSim;
using namespace SimTK;

int main(int argc, char* argv[]) {
	try {
		std::string path("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/MoBL_ARMS_module6_7_CMC.osim");
		Model model(path);

		//simulation from 0.62 to 0.72

		//standard controls from download
		ForwardTool *tool = new ForwardTool("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/MoBL_ARMS_module7_FDS_Setup - Kopie.xml");
		tool->setModel(model);
		std::clock_t startTime = std::clock();
		tool->run();
		auto t1 = 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC;
		
		//with a different control file
		startTime = std::clock();
		tool->setControlsFileName("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/controls.xml");
		tool->addAnalysisSetToModel();
		tool->run();
		auto t2 = 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC;

		//with different first states file
		startTime = std::clock();
		tool->setStatesFileName("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/CMC_Reach8_states_abbrev - Kopie.sto");
		tool->run();
		auto t3 = 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC;
		//command line, standard controls
		startTime = std::clock();
		system("cd E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/MoBL_ARMS_tutorial_33/MoBL-ARMS OpenSim tutorial_33/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation && forward -S \"MoBL_ARMS_module7_FDS_Setup - Kopie - Kopie.xml\"");
		auto t4 = 1.e3*(std::clock() - startTime) / CLOCKS_PER_SEC;
		

		std::cout << "t1 : " << t1 << "\n" << "t2 : " << t2 << "\n" << "t3 : " << t3  << "\n" << "t4 : " << t4 << std::endl;
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
}