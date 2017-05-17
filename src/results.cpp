#include "../include/Results.h"

Results::Results() {
}

Results::Results(std::string path, int number) {
	this->path = path;
	this->number = number;
}

void Results::add(double x, double y, double z) {
	std::stringstream s;
	s << path << "/marker_" << number << ".txt";
	std::ofstream out(s.str(), std::ios_base::app);
	out << x << "\t" << y << "\t" << z << std::endl;
	out.close();
}