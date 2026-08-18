#ifndef DELIVERY_GRAPH_H
#define DELIVERY_GRAPH_H

#include <string>
#include <vector>
#include <map>

// Weighted, undirected graph of Noida delivery zones connected by roads.
// Edge weight = approximate road distance in km. Used to find the
// shortest delivery route (and hence delivery charge) between a
// restaurant's zone and the customer's zone.
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

    // Dijkstra's algorithm (simple O(V^2) array-scan version, well suited
    // to the small number of zones in this project). Returns false if
    // either zone is unknown or unreachable.
    bool shortestPath(const std::string& from, const std::string& to,
                       double& outDistanceKm, std::vector<std::string>& outPath) const;

    void printNetwork() const;
};

#endif
