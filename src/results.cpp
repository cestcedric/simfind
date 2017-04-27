#include "../include/Results.h"

Results::Results() {}

void Results::add(int x, int y, int z) {
	if (!isItIn(x, y, z)) {
		point p;
		p.x = x;
		p.y = y;
		p.z = z;
		points.push_back(p);
	}
}

bool Results::isItIn(int x, int y, int z) {
	//use google thing for linear programming
	return false;
}

std::string Results::print(int n) {
	//print points in a way so that matlab can draw the fancy pictures
	return "";
}