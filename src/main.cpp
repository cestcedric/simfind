#include <iostream>
#include <OpenSim/OpenSim.h>

using namespace OpenSim;
using namespace SimTK;

int main(int argc, char* argv[]) {
	try {
		//replace by input
		std::string path("E:/Dokumente/Schule/tum/Informatik/6/Bachelor-Arbeit/Original Files - UpperExtremityModel/Stanford VA upper limb model_0.osim");
		
		Model model(path);
		Set<Muscle> muscles = model.getMuscles();

		//----------------
		PrescribedController *controla = new PrescribedController;
		controla->setActuators(model.updActuators());

		for (int i = 0; i < muscles.getSize(); i++) {
			controla->prescribeControlForActuator(muscles.get(i).getName(), new Constant(1.0));
		}

		model.addController(controla);

		//----------------
		State si = model.initSystem();
		model.updMultibodySystem().realize(si, Stage::Dynamics);
		RungeKutta2Integrator integrator(model.getMultibodySystem());
		integrator.setAccuracy(10e-6);

		//----------------
		Manager benson(model, integrator);
		double starttime = 0.0;
		double finaltime = 0.1;
		
		benson.setInitialTime(starttime);
		benson.setFinalTime(finaltime);

		benson.integrate(si);

		benson.getStateStorage().print("states.sto");
		//filter angles and return them

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