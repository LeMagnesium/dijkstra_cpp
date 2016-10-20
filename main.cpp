#include <iostream>
#include <string>
#include <fstream>

#include "wptfc/waypoint.hpp"
#include "dijkstra.hpp"

int main(int argc, char * argv[]) {
	if (argc == 1) {
		std::cout << "No file argument"
			<< std::endl;
		return 0;
	}

	std::ifstream fptr;
	std::string validate("\u2713"), error("\u2717");
	fptr.open(argv[1], std::ifstream::in);

	WaypointFile * wptf = new WaypointFile();
	if (wptf->readFromSource(&fptr)) {
		std::cout << "Reading done " << validate << std::endl;
	} else {
		fptr.close();
		std::cerr << "Reading error " << error << std::endl;
		return 1;
	}

	Dijkstra dij = Dijkstra(wptf);
	std::cout << "Data loaded " << validate << std::endl
		<< "Solving begins..." << std::endl;
	trajectory * tj = dij.solve();

	if (tj->size() == 0) {
		std::cout << "The exit waypoint could not be reached " << error << std::endl;
		return 0;
	}

	std::cout << "Solved " << validate << std::endl;
	std::cout << "The shortest route goes through : " << std::endl << wptf->getWaypointPtr(wptf->getStart())->getLabel();

	for (trajectory::iterator it = tj->begin()+1; it!=tj->end(); it++) {
		Waypoint * wpt = wptf->getWaypointPtr(*it);
		std::cout << " -> " << wpt->getLabel();
	}
	std::cout << std::endl;

	delete wptf;
	delete tj;

	return 0;
}
