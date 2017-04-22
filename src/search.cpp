#include "../include/search.h"
#include "../include/excitation.h"
#include "../include/initialState.h"


Search::Search() {
	fwd = new OpenSim::ForwardTool();
}

void Search::setModel(std::string m) {
	model = OpenSim::Model(m);
}

void Search::setIntervall(double t) {

}

void Search::setControls(std::string path) {
	pathControls = path;
}

void Search::setInitialState(std::string path) {
	pathInitialState = path;
}

void Search::run() {
	
	/*
	for (Bedingung) {
		create new InitialState
		for (Bedingung 2) {//l‰ngere Zeit kein neuer ‰uﬂerster Punkt
			create new Excitation
			run forward tool
		}
	}
	*/
	// 6 different initial states
	//for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 10; j++) {

		}
	//}
}
