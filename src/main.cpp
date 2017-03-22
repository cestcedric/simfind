#include <OpenSim/OpenSim.h>
#include <iostream>

using namespace OpenSim;
using namespace SimTK;

int main(int argc, char* argv[]) {
	try {
		//replace by input
		std::string path("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/model_works/MoBL_ARMS_Upper_Limb_Model_OpenSim.osim");

		Model model(path);
		State &si = model.initSystem();
		std::cout << model.getName() << std::endl;
		int b = model.getNumBodies();
		int c = model.getNumControls();
		std::cout << b << std::endl;
		std::cout << c << std::endl;
		std::cout << "loaded" << std::endl;

		//----------------
		PrescribedController *controla = new PrescribedController;
		controla->setActuators(model.updActuators());

		for (int i = 0; i < c; i++) {
			//controla->prescribeControlForActuator(i, new Constant(1.0));
		}

		std::cout << "excited" << std::endl;

		model.addController(controla);

		std::cout << "added" << std::endl;


		//----------------
		//model.updMultibodySystem().realize(si, Stage::Dynamics);
		std::cout << "realized" << std::endl;
		RungeKuttaMersonIntegrator integrator(model.getMultibodySystem());
		std::cout << "integrate" << std::endl;
		integrator.setAccuracy(10e-6);
		std::cout << "accurate" << std::endl;

		std::cout << "integrator" << std::endl;

		//----------------
		Manager benson(model, integrator);
		double starttime = 0.0;
		double finaltime = 0.1;
		
		benson.setInitialTime(starttime);
		benson.setFinalTime(finaltime);

		std::cout << "managed" << std::endl;

		try {
			benson.integrate(si);
		}	catch (...) {
			std::cout << "bruh" << std::endl;
		}

		std::cout << "integrated" << std::endl;

		benson.getStateStorage().print("states.sto");
		////filter angles and return them

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

	std::cout << "mic drop\n" << std::endl;

}