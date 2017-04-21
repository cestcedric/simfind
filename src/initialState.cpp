#include "../include/initialState.h"

InitialState::InitialState(int n) {
	number = n;
}

std::string InitialState::create(std::vector<double> data) {
	this->data = data;
	//calculate something
	return print();
}

std::string InitialState::print() {
	std::string path;
	//path = outputstream
	return path;
}

void InitialState::setTemplate(std::string path) {
	//vorlage = inputstream
}