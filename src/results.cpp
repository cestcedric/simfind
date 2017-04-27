#include "../include/Results.h"

Results::Results(int n) {

}

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

}

std::string Results::print() {

}