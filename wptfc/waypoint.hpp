#ifndef _WAYPOINT_HPP
#define _WAYPOINT_HPP 1

/*
  Class prototypes for WayPointFile Compiler
*/

#include <map>
#include <vector>

typedef std::vector<uint16_t> trajectory;

class Waypoint {
public:
  Waypoint();
  Waypoint(uint16_t id, std::string label);

  bool setDistance(uint16_t dest, uint16_t len);
  uint16_t getDistance(uint16_t dest);

  uint16_t getId();

  const std::string getLabel();

  size_t getOutgoingConnectionsCount();
  std::vector<uint16_t> getOutgoingConnectionsIds();

  void writeBlobData(std::ofstream * of);
private:
  std::map<uint16_t, uint16_t> distances;
  uint16_t id;
  std::string label;
};

class WaypointFile {
public:
  WaypointFile();
  //  WaypointFile(std::ifstream * ifs);
  ~WaypointFile();

  bool setStart(uint16_t id);
  bool setEnd(uint16_t id);
  // Set end and start

  bool addWaypoint(Waypoint wpt);

  Waypoint* getWaypointPtr(uint16_t id);
  std::map<uint16_t, uint16_t> getConnectionsFor(uint16_t id);

  uint16_t allocateId();

  size_t size();
  uint16_t getStart();
  uint16_t getEnd();
  std::vector<uint16_t> getWptIds();

  void writeBlob(std::ofstream * of);
  bool readFromSource(std::ifstream * ifs);

private:
  uint16_t start;
  uint16_t end;
  bool startset;
  bool endset;

  std::map<uint16_t, Waypoint> wpts;
};

#endif
