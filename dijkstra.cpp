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
//		std::cout << this->

	}
	return new trajectory;
}
