#include "DeliveryGraph.h"
#include <iostream>
#include <limits>
#include <algorithm>

void DeliveryGraph::addZone(const std::string& name) {
    if (zoneIndex.count(name)) return; // already present
    zoneIndex[name] = static_cast<int>(zoneNames.size());
    zoneNames.push_back(name);
    adjList.push_back({});
}

void DeliveryGraph::addRoad(const std::string& zoneA, const std::string& zoneB, double km) {
    if (!zoneIndex.count(zoneA) || !zoneIndex.count(zoneB)) {
        std::cerr << "  [Graph] Cannot add road, unknown zone(s): " << zoneA << " / " << zoneB << "\n";
        return;
    }
    int a = zoneIndex[zoneA];
    int b = zoneIndex[zoneB];
    adjList[a].push_back({b, km});
    adjList[b].push_back({a, km}); // undirected road
}

bool DeliveryGraph::hasZone(const std::string& zone) const {
    return zoneIndex.count(zone) > 0;
}

int DeliveryGraph::zoneCount() const { return static_cast<int>(zoneNames.size()); }

bool DeliveryGraph::shortestPath(const std::string& from, const std::string& to,
                                  double& outDistanceKm, std::vector<std::string>& outPath) const {
    if (!hasZone(from) || !hasZone(to)) return false;

    int n = zoneCount();
    int src = zoneIndex.at(from);
    int dst = zoneIndex.at(to);

    const double INF = std::numeric_limits<double>::infinity();
    std::vector<double> dist(n, INF);
    std::vector<int> prevNode(n, -1);
    std::vector<bool> visited(n, false);
    dist[src] = 0.0;

    for (int iter = 0; iter < n; ++iter) {
        // pick the unvisited node with the smallest known distance
        int u = -1;
        double best = INF;
        for (int i = 0; i < n; ++i) {
            if (!visited[i] && dist[i] < best) { best = dist[i]; u = i; }
        }
        if (u == -1) break; // remaining nodes are unreachable
        visited[u] = true;
        if (u == dst) break;

        for (const Edge& e : adjList[u]) {
            if (!visited[e.to] && dist[u] + e.km < dist[e.to]) {
                dist[e.to] = dist[u] + e.km;
                prevNode[e.to] = u;
            }
        }
    }

    if (dist[dst] == INF) return false;

    outDistanceKm = dist[dst];
    outPath.clear();
    for (int at = dst; at != -1; at = prevNode[at]) outPath.push_back(zoneNames[at]);
    std::reverse(outPath.begin(), outPath.end());
    return true;
}

void DeliveryGraph::printNetwork() const {
    std::cout << "  Delivery zones and connecting roads:\n";
    for (int i = 0; i < zoneCount(); ++i) {
        std::cout << "   " << zoneNames[i] << " -> ";
        if (adjList[i].empty()) { std::cout << "(no direct roads)\n"; continue; }
        for (size_t j = 0; j < adjList[i].size(); ++j) {
            std::cout << zoneNames[adjList[i][j].to] << " (" << adjList[i][j].km << " km)";
            if (j + 1 < adjList[i].size()) std::cout << ", ";
        }
        std::cout << "\n";
    }
}
