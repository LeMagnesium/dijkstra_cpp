#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

/*
  WayPoinT File Compiler
  Creates Waypoint diagrams for Dijkstra's algorithm
  License: GPL3
  By Lymkwi, 2016
*/

#include <waypoint.hpp>
#include <binutils.hpp>

// Main Function
int main(int argc, char * argv[]) {
  if (argc == 1) {
    std::cout << "No file provided" << std::endl;
    return 0;
  }

  // This std::ifstream is the file input
  std::ifstream ifs;
  ifs.open(argv[1], std::ifstream::in);
  if (!ifs.good()) {
    std::cout << "Error in opening file. Maybe it doesn't exist.." << std::endl;
    ifs.close();
    return 1;
  }

  // We declare on the heap, because the data might grow gigantic
  WaypointFile * wptf = new WaypointFile();
    
  if (wptf->readFromSource(&ifs)) {
    std::cout << "Compilation succeeded \u2713" << std::endl;
    ifs.close();
  } else {
    std::cout << "Compilation failed \u2717" << std::endl;
    ifs.close();
    return 0;
  }

  if (!wptf->isWaypointFileReady()) {
  	std::cerr << "Compilation failed \u2717" << std::endl;
	return 1;
  }

  // Compilation over, now producting the blob
  std::string outname(argv[1]);
  outname += 'c'; // compiled wptC file

  std::ofstream blob;
  blob.open(outname.c_str(), std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);
  // Write the binary blob to our .wptc file
  wptf->writeBlob(&blob);
  delete wptf;

  if (blob.good()) {
    std::cout << "Data dumped into '" << outname << "' \u2713" << std::endl;
  } else {
    std::cout << "Error in writing data \u2717" << std::endl;
  }

  // Job. Finished.
  blob.close();

  return 0;
}
