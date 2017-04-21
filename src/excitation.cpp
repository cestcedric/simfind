#include "../include/excitation.h"

Excitation::Excitation(int n) {
	number = n;
}

std::string Excitation::excite(std::vector<double> v) {
	excitations = v;
	return print();
}

std::string Excitation::print() {
	int entry = 0;
	int limit = 10;// = number of lines vorlage
	for (int i = 0; i < limit; i++) {
		if (i == entry * 40 + 14) {
			//array a = split at >
			//write  a[0] + excitations[entry] + a[1]
		}
		else
		{
			if (i == entry * 40 + 18) {
				//array a = split at >
				//write  a[0] + excitations[entry] + a[1]
			}
		}
	}
	std::string path;
	//path = outputstream
	return path;
}

void Excitation::setTemplate(std::string path) {
	//vorlage = inputstream(path)
}