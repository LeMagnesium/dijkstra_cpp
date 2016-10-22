#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP "loaded"

/*\
 * 	C++ Implementation of Dijkstra's Algorithm
 * 	ßÿ Lymkwi/LeMagnesium
 * 	License : WTFPL
 * 	Last modification : 22/10/2016
 *
\*/

#include "wptfc/waypoint.hpp"

class Dijkstra {
public:
  Dijkstra(WaypointFile*);
  ~Dijkstra();

  void addRoute(trajectory, uint16_t);

  void loadFromWptFile(WaypointFile*);
  trajectory * solve();
private:
  WaypointFile * waypointfile = NULL;
  std::map<uint16_t, uint16_t> grid;
  std::map<trajectory, uint16_t> discoveries;
  std::vector<uint16_t> exhausted;
};

#endif
