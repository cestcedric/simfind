#include "../include/Results.h"

Results::Results() {
}

Results::Results(std::string path, int number) {
	this->path = path;
	this->number = number;
}

void Results::add(double x, double y, double z) {
	if (!isItIn(x, y, z)) {
		std::vector<double> p(3);
		p[0] = x;
		p[1] = y;
		p[2] = z;

		points.push_back(p);
	}
}

bool Results::isItIn(double x, double y, double z) {
	//use google thing for linear programming
	return false;
}

std::string Results::print() {
	//print points in a way so that matlab can draw the fancy pictures
	std::stringstream s;
	s << path << "/marker_" << number << ".txt";
	std::ofstream out(s.str());
	for (auto point : points) {
		out << point[0] << "\t" << point[1] << "\t" << point[2] << std::endl;
	}

	out.close();

	return s.str();
}