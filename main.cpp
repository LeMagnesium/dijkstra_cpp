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
  fptr.open(argv[1], std::ifstream::in);

  WaypointFile * wptf = new WaypointFile();
  if (wptf->readFromSource(&fptr)) {
    std::cout << "Reading done \u2713" << std::endl;
  } else {
    fptr.close();
    std::cerr << "Reading error \u2717" << std::endl;
    return 1;
  }

  Dijkstra dij = Dijkstra(wptf);
  std::cout << "Data loaded \u2713" << std::endl
	    << "Solving begins..." << std::endl;
  trajectory * tj = dij.solve();

  delete wptf;
  delete tj;

  return 0;
}
