#include <iostream>

#include "wptfc/waypoint.hpp"
#include "dijkstra.hpp"

// A Dijkstra
//Dijkstra::Dijkstra() {
//  this->wptf = NULL;
//}

Dijkstra::Dijkstra(WaypointFile * wptf) {
  this->loadFromWptFile(wptf);
}

Dijkstra::~Dijkstra() {;}

void Dijkstra::updateAround(uint16_t cursor) {
	Waypoint * wpt = this->waypointfile->getWaypointPtr(cursor);
	std::vector<uint16_t> around = wpt->getOutgoingConnectionsIds();
	for (std::vector<uint16_t>::iterator it = around.begin(); it!=around.end(); it++) {
		Waypoint * connection = this->waypointfile->getWaypointPtr(*it);
		//std::cout << wpt->getDistance(*it) << std::endl;
		this->grid[*it] = wpt->getDistance(*it);
	}
}

void Dijkstra::holdTrial(uint16_t one, uint16_t two) {
	;
}

void Dijkstra::addRoute(trajectory traj, uint16_t dist) {
	this->discoveries[traj] = dist;
}

void Dijkstra::loadFromWptFile(WaypointFile * wptf) {
  this->waypointfile = wptf;
}

trajectory * Dijkstra::solve() {
	// Add starting point as current path
	uint16_t startpoint = this->waypointfile->getStart();
	std::vector<uint16_t> d;
	d.push_back(startpoint);
	this->addRoute(d, 0);

	uint16_t selected = 0; // Selected path 0

	// Main loop
	while (true) {
		// Explore surroundings
		this->updateAround(selected);

		// Select
		Waypoint * wpt = this->waypointfile->getWaypointPtr(selected);
		std::vector<uint16_t> around = wpt->getOutgoingConnectionsIds();
		long min = -1;
		long newid = -1;
		for (std::vector<uint16_t>::iterator it = around.begin(); it!=around.end(); it++) {
			//std::cout << *it << std::endl;
			//std::cout << this->grid[*it] << std::endl;
			if (min == -1 || this->grid[*it] < min) {
				newid = *it;
				min = this->grid[*it];
			}
		}
		//std::cout << min << std::endl;
		std::cout << "Getting to " << newid << "..." << std::endl;
		selected = newid;
		if (newid == this->waypointfile->getEnd()) {break;}
	}
	return new trajectory;
}
