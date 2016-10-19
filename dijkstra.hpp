#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP "loaded"


#include "wptfc/waypoint.hpp"

class Dijkstra {
public:
  //  Dijkstra();
  Dijkstra(WaypointFile*);
  ~Dijkstra();

  void updateAround(uint16_t, trajectory, long);
  void addRoute(trajectory, uint16_t);
  void holdTrial(uint16_t, uint16_t);

  void loadFromWptFile(WaypointFile*);
  trajectory * solve();
private:
  WaypointFile * waypointfile = NULL;
  std::map<uint16_t, uint16_t> grid;
  std::map<trajectory, uint16_t> discoveries;
  std::vector<uint16_t> exhausted;
};

#endif
