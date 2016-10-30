#include <iostream>
#include <fstream>
#include <map>

/*
  Class WayPoint
  Store data for the WayPoinTFile Compiler WPTFC
*/

#include "waypoint.hpp"
#include "binutils.hpp"
#include <cstring>

// Some utils
// Assert whether or not id is convertible to a WPtId
bool assertWptId(std::string id) {
  for (uint8_t o = 0; o < id.size(); o++) {
    if (!isdigit(id[o])) {
      return false;
    }
  }
  return true;
}

// Waypoint
// Empty Waypoint
Waypoint::Waypoint() {
  this->id = 0;
  this->label = std::string("");
  this->distances = std::map<uint16_t, uint16_t>();
}

// Documented Waypoint
Waypoint::Waypoint(uint16_t id, std::string label) {
  this->id = id;
  this->label = label;
}

// Distance between two waypoints (coded on 16 bytes)
bool Waypoint::setDistance(uint16_t dest, uint16_t len) {
  this->distances[dest] = len;
  return true;
}

uint16_t Waypoint::getDistance(uint16_t dest) {
  return this->distances[dest];
}

uint16_t Waypoint::getId() {
  return this->id;
}

const std::string Waypoint::getLabel() {
  return this->label;
}

// Return an array of 16-bytes integers which all receive connections from
// the waypoint which Id is provided
std::vector<uint16_t> Waypoint::getOutgoingConnectionsIds() {
  std::vector<uint16_t> ret;
  for (std::map<uint16_t, uint16_t>::iterator it=this->distances.begin(); it!=this->distances.end(); it++) {
    ret.push_back(it->first);
  }

  return ret;
}

// Could also work with mywpt.getOutgoingConnectionsIds().size()
size_t Waypoint::getOutgoingConnectionsCount() {
  return this->distances.size();
}

// Blob data composer
void Waypoint::writeBlobData(std::ofstream * blob) {
  // u16 id
  writeU16(blob, this->id);

  // u8 label_len
  writeU8(blob, this->label.size());

  // foreach label_len
  for (uint8_t len = 0; len < this->label.size(); len++) {
    // u16 char
    writeU16(blob, this->label[len]);
  }

  // u8 connections
  writeU8(blob, this->getOutgoingConnectionsCount());

  // foreach connections
  for (std::map<uint16_t, uint16_t>::iterator it = this->distances.begin();
       it != this->distances.end(); it++) {
    writeU16(blob, it->first);
    writeU16(blob, it->second);
  }
}

// WaypointFile
// This represents a waypointfile, not a compressed blob
WaypointFile::WaypointFile() {
  this->start = 0; this->startset = false;
  this->end = 0; this->endset = false;
}

WaypointFile::~WaypointFile() {;}

// The WayPoint needs a beginning
bool WaypointFile::setStart(uint16_t id) {
  this->start = id;
  this->startset = true;
  return true;
}

bool WaypointFile::setEnd(uint16_t id) {
  this->end = id;
  this->endset = true;
  return true;
}

bool WaypointFile::addWaypoint(Waypoint wpt) {
  this->wpts[wpt.getId()] = wpt;
  return true;
}

Waypoint* WaypointFile::getWaypointPtr(uint16_t id) {
  return &(this->wpts[id]);
}

uint16_t WaypointFile::allocateId() {
  uint16_t i = 0;
  for (std::map<uint16_t,Waypoint>::iterator it = this->wpts.begin(); it != this->wpts.end(); ++it) {
    if (i < it->first)
      break;
    i++;
  }
  return i;
}

size_t WaypointFile::size() {
  return this->wpts.size();
}

uint16_t WaypointFile::getStart() {
  return this->start;
}

uint16_t WaypointFile::getEnd() {
  return this->end;
}

std::vector<uint16_t> WaypointFile::getWptIds() {
  std::vector<uint16_t> wpts;
  for (std::map<uint16_t, Waypoint>::iterator it=this->wpts.begin(); it!=this->wpts.end(); it++) {
    wpts.push_back(it->first);
  }

  return wpts;
}

std::map<uint16_t, uint16_t> WaypointFile::getConnectionsFor(uint16_t id) {
  std::map<uint16_t, uint16_t> ret;
  std::vector<uint16_t> ocids = this->wpts[id].getOutgoingConnectionsIds();
  for (std::vector<uint16_t>::iterator it = ocids.begin(); it != ocids.end(); it++) {
    ret[*it] = this->wpts[id].getDistance(*it);
  }
  return ret;
}


void WaypointFile::writeBlob(std::ofstream * blob) {
  // Signature and Version
  // u32 "WPTF"
  // u8 version
  blob->write("WPTF", 4*sizeof(char));
  blob->put(1);

  // u16 waypoint_nbr
  writeU16(blob, this->size());

  // u16 start_id
  writeU16(blob, this->getStart());

  // u16 end_id
  writeU16(blob, this->getEnd());

  // foreach waypoint_nbr
  for (std::map<uint16_t, Waypoint>::iterator it=this->wpts.begin(); it!=this->wpts.end(); it++)
    it->second.writeBlobData(blob);
}

bool WaypointFile::isWaypointFileReady() {
	if (!this->endset) {
		std::cerr << "ERROR: No end waypoint set (missing 'END' line)" << std::endl;
		return false;
	} else if (!this->startset) {
		std::cerr << "ERROR: No start waypoint set (missing 'START' line)" << std::endl;
		return false;
	} else if (this->wpts.count(this->end) == 0) {
		std::cerr << "ERROR: Defined end waypoint is not declared : " << this->end << std::endl;
		return false;
	} else if (this->wpts.count(this->start) == 0) {
		std::cerr << "ERROR: Defined start waypoint is not declared : " << this->start << std::endl;
		return false;
	}
	return true;
}

bool WaypointFile::readFromSource(std::ifstream * ifs) {
  size_t lnc = 1;
  while (!ifs->eof()) {
    char * command = new char[300];
    ifs->getline(command, 300);
    std::string line(command);
    delete[] command;
    
    if (line.size() == 0 || line[0] == '#') {continue;}

    std::string cmd = line.substr(0, line.find("\t"));

    if (strcmp(cmd.c_str(), "start") == 0) {
      if (line.find("\t") > 65535) {
	std::cerr << "ERROR: No id for start (line " << lnc << ")" << std::endl;
	return false;
      }
 
      std::string id = line.substr(line.find("\t")+1);

      if (!assertWptId(id)) {
	std::cerr << "ERROR: Invalid id '" << id << "' (line " << lnc << ")" << std::endl;
	return false;
      }

      this->setStart(stoul(id));

    } else if (strcmp(cmd.c_str(), "end") == 0) {
      if (line.find("\t") > 65535) {
	std::cout  << "ERROR: No id for end (line " << lnc << ")" << std::endl;
	return false;
      }

      std::string id = line.substr(line.find("\t")+1);

      if (!assertWptId(id)) {
	std::cerr << "ERROR: Invalid id '" << id << "' (line " << lnc << ")" << std::endl;
	return false;
      }
      
      this->setEnd(stoul(id));

    } else if (strcmp(cmd.c_str(), "waypoint") == 0) {
      std::string params = line.substr(9);
      if (params.find("\t") > 65535) {
	std::cerr <<" ERROR: Missing tabulation to separate id and label (line " << lnc << ")"
		  << std::endl;
	return false;
      }

      std::string id = params.substr(0, params.find("\t"));
      if (!assertWptId(id)) {
	std::cerr << "ERROR: Invalid id '" << id << "' (line " << lnc << ")"
		  << std::endl;
	return false;
      }

      params = params.substr(params.find("\t")+1);
      if (params[0] != '"') {
	std::cerr << "ERROR: Missing quote to start waypoint label"
		  << std::endl;
	return false;
      }

      // Collect the label string
      std::string label;
      bool success = true;
      for (size_t i = 1; params[i] != '"'; ++i) {
	if (params[i] == '\\')
	  // Escape
	  i++;

	label += params[i];
	if (i == params.size()) {
	  success = false; break;
	}
      }
      if (!success) {
	std::cerr << "ERROR: Missing quote to end waypoint label (line " << lnc << ")"
		  << std::endl;
	return false;
      }
      if (label.size() > 256) {
	std::cerr << "ERROR: Label for waypoint is too long (256 char max) (line " << lnc << ")"
		  << std::endl;
	return false;
      }
      this->addWaypoint(Waypoint(stoul(id), label));

    } else if (strcmp(cmd.c_str(), "distance") == 0) {
      std::string params = line.substr(9);
      if (params.find("\t") > 65535) {
	std::cerr << "ERROR: Missing tabulation to introduce second waypoint id (line " << lnc << ")"
		  << std::endl;
	return false;
      }

      if (params.rfind("\t") == params.find("\t")) {
	std::cerr << "ERROR: Missing tabulation to introduce distance between waypoints (line " << lnc << ")"
		  << std::endl;
	return false;
      }

      std::string id1 = params.substr(0, params.find("\t"));
      std::string id2 = params.substr(params.find('\t')+1, params.rfind('\t')-params.find('\t')-1);
      std::string dist = params.substr(params.rfind("\t")+1);

      if (!(assertWptId(id1) && assertWptId(id2) && assertWptId(dist))) {
	std::cerr << "ERROR: Invalid value in distance declaration (line " << lnc << ")"
		  << std::endl
		  << "Source ID: " << id1 << ", Dest ID: " << id2 << ", Distance: " << dist
		  << std::endl;
	return false;
      }

      this->getWaypointPtr(stoul(id1))->setDistance(stoul(id2), stoul(dist));
      
    } else {
      std::cerr << "ERROR: No command " << cmd << " (line " << lnc << ")" << std::endl;
      return false;
    }
    lnc++;
  }
  return true;
}

