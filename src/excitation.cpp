#include "../include/excitation.h"

Excitation::Excitation(int n) {
	this->setNumber(n);
}

void Excitation::setNumber(int n) {
	number = n;
}

std::string Excitation::excite(std::vector<double> v) {
	excitations = v;
	return print();
}

std::string Excitation::print() {
	int entry = 0;
	std::stringstream out;
	//new controls file is put into Controls folder to seem organized
	out << path << "Controls/controls_" << number << ".xml";
	std::ofstream tmp(out.str());

	for (int i = 0; i < numberOfLines; i++) {
		if (i == (entry * 40 + 14)) {
			tmp << "\t\t\t\t\t\t<value>" << excitations[entry] << "</value>\n";
		}
		else
		{
			if (i == (entry * 40 + 18)) {
				tmp << "\t\t\t\t\t\t<value>" << excitations[entry] << "</value>\n>";
				entry++;
			}
			else
			{
				tmp << controls[i] << std::endl;
			}
		}
	}
	tmp.close();
	return out.str();
}

void Excitation::setTemplate(std::string path) {
	int size = path.size() - 12;
	this->path = path.substr(0, size);//remove .xml at the end
	std::cout << this->path << std::endl;
	controls = std::vector<std::string>(2008);//should always stay the same
	numberOfLines = 0;

	std::ifstream tmp;
	tmp.open(path);

	for (int i = 0; i < 2008; i++) {
		std::string line;
		std::getline(tmp, line);
		controls[i] = line;
		numberOfLines++;
	}

	tmp.close();
}