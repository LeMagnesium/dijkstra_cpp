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

Dijkstra::~Dijkstra() {
	delete this->waypointfile;
}

void Dijkstra::updateAround(uint16_t cursor, trajectory current, long curlength) {
	Waypoint * wpt = this->waypointfile->getWaypointPtr(cursor);
	std::vector<uint16_t> around = wpt->getOutgoingConnectionsIds();
	for (std::vector<uint16_t>::iterator it = around.begin(); it!=around.end(); it++) {
		Waypoint * connection = this->waypointfile->getWaypointPtr(*it);
		this->grid[*it] = wpt->getDistance(*it);
		trajectory attempt(current);
		attempt.push_back(*it);
		this->addRoute(attempt, curlength + wpt->getDistance(*it));
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

	uint16_t selected = 0; // Selected path 0

	// Main loop
	long length = 0;
	trajectory tested;
	tested.push_back(startpoint);
	while (true) {
		// Explore surroundings
		this->updateAround(selected, tested, length);

		// Select
		Waypoint * wpt = this->waypointfile->getWaypointPtr(selected);
		std::vector<uint16_t> around = wpt->getOutgoingConnectionsIds();

		long min = -1;
		long newid = -1;
		bool traject_change = false;
		for (std::vector<uint16_t>::iterator it = around.begin(); it!=around.end(); it++) {
			if (min == -1 || this->grid[*it] < min) {
				newid = *it;
				min = this->grid[*it];
			}
		}
		for (std::map<trajectory, uint16_t>::iterator prev_iter = this->discoveries.begin(); prev_iter!=this->discoveries.end(); prev_iter++) {
			if (min == -1 || prev_iter->second < min) {
				/*
				 * 		This part assigns saved values to the operated variables as to simulate
				 * 	movement on another branch. The min variable is still changed for further computing.
				 * 	tested, our trajectory, is changed to the trajectory to the smallest-in-distance
				 * 	non-exhausted and visited point.
				 * 	traject_change is toggled so that we don't apply normal calculus to length, and
				 * 	adds the discovered trajectories in our registers afterwards, since we are moving
				 * 	branch
				 *
				 */
				tested = prev_iter->first;
				min = prev_iter->second;
				traject_change = true;
			}
		}
		//std::cout << min << std::endl;
		if (!traject_change) {
			length += min;
			tested.push_back(newid);
			this->discoveries[tested] = length;
		} else {
			// length is changed to the distance from the start to our new point
			// the new id is set from our new trajectory
			newid = tested.back();
			length = this->discoveries[tested];
		}
		this->exhausted.push_back(selected);
		selected = newid;
		// Remove the exhausted and do anticipation
		for (std::vector<uint16_t>::iterator exhaust_iter = this->exhausted.begin(); exhaust_iter!=this->exhausted.end(); exhaust_iter++) {
			for (std::map<trajectory, uint16_t>::iterator it = this->discoveries.begin(); it!=this->discoveries.end(); it++) {
				uint16_t wptindex = it->first.back(); // Last element of the trajectory, which could be the exhausted
				// Compare value currently pointed by the iterator to that one waypoint ID
				if (*exhaust_iter == wptindex) {
					this->discoveries.erase(it);
					it = this->discoveries.begin();
				}
				if (this->exhausted.size() == 0) {break;}
			}
		}

		if (selected == this->waypointfile->getEnd() || this->exhausted.size() == this->waypointfile->getWptIds().size()) {break;}
	}
	
	// Now, the aftermath
	if (this->exhausted.size() == this->waypointfile->getWptIds().size()) {
		// There is no exit
		return new trajectory();
	} else {
		return new trajectory(tested);
	}
}
