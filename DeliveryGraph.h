#ifndef DELIVERY_GRAPH_H
#define DELIVERY_GRAPH_H

#include <string>
#include <vector>
#include <map>


struct Edge {
    int to;
    double km;
};

class DeliveryGraph {
private:
    std::map<std::string, int> zoneIndex;
    std::vector<std::string> zoneNames;
    std::vector<std::vector<Edge>> adjList;

public:
    void addZone(const std::string& name);
    void addRoad(const std::string& zoneA, const std::string& zoneB, double km);
    bool hasZone(const std::string& zone) const;
    int zoneCount() const;
    const std::vector<std::string>& zones() const { return zoneNames; }

    
    bool shortestPath(const std::string& from, const std::string& to,
                       double& outDistanceKm, std::vector<std::string>& outPath) const;

    void printNetwork() const;
};

#endif
