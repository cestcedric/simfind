#include <OpenSim/OpenSim.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace OpenSim;
using namespace SimTK;

int main(int argc, char* argv[]) {
	//manager.integrate() crashing
	//try {
	//	//replace by input
	//	std::string path("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/ModelFiles/MoBL_ARMS_module6_7_CMC.osim");

	//	Model model(path);
	//	State &si = model.initSystem();
	//	std::cout << model.getName() << std::endl;
	//	int b = model.getNumBodies();
	//	int c = model.getNumControls();
	//	std::cout << b << std::endl;
	//	std::cout << c << std::endl;
	//	std::cout << "loaded" << std::endl;

	//	//----------------
	//	PrescribedController *controla = new PrescribedController;
	//	controla->setActuators(model.updActuators());

	//	for (int i = 0; i < c; i++) {
	//		controla->prescribeControlForActuator(i, new Constant(1.0));
	//	}

	//	std::cout << "excited" << std::endl;

	//	model.addController(controla);

	//	std::cout << "added" << std::endl;


	//	//----------------
	//	model.updMultibodySystem().realize(si, Stage::Dynamics);
	//	std::cout << "realized" << std::endl;
	//	RungeKuttaMersonIntegrator integrator(model.getMultibodySystem());
	//	std::cout << "integrate" << std::endl;
	//	integrator.setAccuracy(10e-6);
	//	std::cout << "accurate" << std::endl;

	//	std::cout << "integrator" << std::endl;

	//	//----------------
	//	Manager benson(model, integrator);
	//	double starttime = 0.0;
	//	double finaltime = 0.001;
	//	
	//	benson.setInitialTime(starttime);
	//	benson.setFinalTime(finaltime);

	//	std::cout << "managed" << std::endl;

	//	try {
	//		benson.integrate(si);
	//	}	catch (...) {
	//		std::cout << "bruh" << std::endl;
	//	}

	//	std::cout << "integrated" << std::endl;

	//	benson.getStateStorage().print("states.sto");
	//	////filter angles and return them

	//}
	//catch (OpenSim::Exception ex)
	//{
	//	std::cout << "opensim " << ex.getMessage() << std::endl;
	//	return 1;
	//}
	//catch (std::exception ex)
	//{
	//	std::cout << "std " << ex.what() << std::endl;
	//	return 1;
	//}
	//catch (...)
	//{
	//	std::cout << "UNRECOGNIZED EXCEPTION" << std::endl;
	//	return 1;
	//}
	

	//alternative method using the command line tools
	//xtina throwback tbh

	//navigate to setup for cmc forward simulation
	//run forward dynamics tool from command line
	//excitiations are changed using the control file
	//change this for your machine
	//system("cd E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation && forward -S MoBL_ARMS_module7_FDS_Setup.xml");
	
	//do something with the results
	//open states file
	std::ifstream results;
	std::string head;
	std::string line;
	line.reserve(4200);//length of one line, for better performance
	results.open("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/MoBL_ARMS_OpenSim33tutorial/ModelFiles/InputFiles_module7_CMCDrivenFowardSimulation/output/FDS_states.sto");

	//do stuff with the results

	//skip first two lines of header and read number of rows
	std::getline(results, head);
	std::getline(results, head);
	std::getline(results, head);//correct line -> line 3
	//get number
	int rows = std::stoi(head.substr(6, 8));

	//////////////////////////////////////////////////
	//get line with the names of the values -> line 7
	//might as well make this hardcoded, idc
	std::getline(results, head);
	std::getline(results, head);
	std::getline(results, head);
	std::getline(results, head); //line with column names

	//explode string -> make method out of this
	std::vector<std::string> names;
	std::istringstream iss(head);
	int k = 0;
	for (std::string token; std::getline(iss, token, '\t'); ) {
		names.push_back(std::move(token));
		k++;
	}

	//filter for values you actually want
	//-> we can set elv_angle, shoulder_elv, shoulder_rot, elbow_flexion, pro_sup, deviation, flexion
	//remember positions
	std::vector<int> positions(7); //in the order mentioned above

	for (int i = 0; i < k; i++) {
		//make a switch statement with string2int method for more sophistication and some kind of readability
		std::string s = names[i];
		if (s == "elv_angle") {
			positions[0] = i;
		}
		else
		{
			if (s == "shoulder_elv") {
				positions[1] = i;
			}
			else
			{
				if (s == "shoulder_rot") {
					positions[2] = i;
				}
				else
				{
					if (s == "elbow_flexion") {
						positions[3] = i;
					}
					else
					{
						if (s == "pro_sup") {
							positions[4] = i;
						}
						else
						{
							if (s == "deviation") {
								positions[5] = i;
							}
							else
							{
								if (s == "flexion") {
									positions[6] = i;
								}
							}
						}
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////

	//go to last line of file = final state
	//go down "rows", since we are at the beginning of the data now
	for (int l = 0; l < rows; l++) {
		std::getline(results, line);
	}
	//again, explode this string = data
	std::vector<std::string> data(7);
	std::istringstream issastring(line);
	for (std::string token; std::getline(issastring, token, '\t'); ) {
		data.push_back(std::move(token));
	}
	//now output only the relevant values = things where we know the positions from above
	for (int c = 0; c < 7; c++) {
		std::cout << names[positions[c]] << " : " << data[positions[c]] << std::endl;
	}


	//close file
	results.close();


	std::cout << "mic drop\n" << std::endl;

}