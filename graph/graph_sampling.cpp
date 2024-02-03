#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <algorithm>

// Define a class to represent lattice nodes
class LatticeNode {
public:
    int id;
    std::vector<int> lattice4id;

    LatticeNode(int _id, std::vector<int> _lattice4id) : id(_id), lattice4id(_lattice4id) {}
};

// Define a class for the growth subgraph
class GrowthSubgraph {
private:
    int g0 = -10;
    int g1 = 15;
    std::unordered_map<int, LatticeNode> latticeNodes;
    std::vector<std::pair<int, int>> edges;
    std::vector<int> subgraphNodes;

public:
    GrowthSubgraph(std::vector<int> seed) {
        for (int i = 0; i < 50; ++i) {
            for (int j = 0; j < 50; ++j) {
                for (int k = 0; k < 50; ++k) {
                    for (int l = 0; l < 2; ++l) {
                        int latticeId = 2 * (i + 50 * (j + 50 * (k + 50 * l))) + l;
                        latticeNodes[latticeId] = LatticeNode(latticeId, {i, j, k, l});
                    }
                }
            }
        }

        // Define edges and create the initial subgraph
        std::vector<std::vector<int>> neighborlist = {{0, 0, 0, 1}, {0, 0, -1, 1}, {-1, 0, 0, 1},
                                                       {-1, 0, -1, 1}, {-1, 1, 0, 1}, {-1, 1, -1, 1}};
        for (int i = 0; i < 50; ++i) {
            for (int j = 0; j < 50; ++j) {
                for (int k = 0; k < 50; ++k) {
                    for (int l = 0; l < 6; ++l) {
                        int i_n = i + neighborlist[l][0];
                        int j_n = j + neighborlist[l][1];
                        int k_n = k + neighborlist[l][2];
                        if (i_n >= 0 && i_n < 50 && j_n >= 0 && j_n < 50 && k_n >= 0 && k_n < 50) {
                            int particle1Index = lattice4id2id({i, j, k, 0});
                            int particle2Index = lattice4id2id({i_n, j_n, k_n, 1});
                            edges.emplace_back(particle1Index, particle2Index);
                        }
                    }
                }
            }
        }

        // Initialize the subgraph with the seed
        for (int nodeId : seed) {
            subgraphNodes.push_back(nodeId);
        }
    }

    int lattice4id2id(std::vector<int> lattice4id) {
        return 2 * (lattice4id[0] + 50 * (lattice4id[1] + 50 * (lattice4id[2] + 50 * lattice4id[3]))) +
               lattice4id[3];
    }

    std::vector<int> latticeid24id(int latticeid) {
        return {((latticeid / 2) % 50), ((latticeid / 100) % 50), ((latticeid / 5000) % 50), latticeid % 2};
    }

    void addNode(int i) {
        subgraphNodes.push_back(i);
    }

    void removeNodes(std::vector<std::pair<int, int>> &cut) {
        std::vector<int> nodesToRemove;
        for (int i = 0; i < cut.size(); ++i) {
            nodesToRemove.push_back(cut[i].first);
            nodesToRemove.push_back(cut[i].second);
        }

        // Remove duplicate nodes
        std::sort(nodesToRemove.begin(), nodesToRemove.end());
        nodesToRemove.erase(std::unique(nodesToRemove.begin(), nodesToRemove.end()), nodesToRemove.end());

        // Create a new subgraph without the removed nodes
        std::vector<int> newSubgraphNodes;
        for (int nodeId : subgraphNodes) {
            if (std::find(nodesToRemove.begin(), nodesToRemove.end(), nodeId) == nodesToRemove.end()) {
                newSubgraphNodes.push_back(nodeId);
            }
        }
        subgraphNodes = newSubgraphNodes;
    }

    std::vector<std::pair<int, int>> neighbor(std::pair<int, int> edge) {
        int m1 = edge.first;
        int m2 = edge.second;
        std::vector<int> id1 = latticeNodes[m1].lattice4id;
        std::vector<int> id2 = latticeNodes[m2].lattice4id;

        if (id1[3] == 1) {
            std::swap(id1, id2);
        }

        std::vector<int> neighbor14id = {id2[0], id2[1], -id2[2] + 2 * id1[2] - 1, 1};
        int neighbor1id = lattice4id2id(neighbor14id);

        std::vector<int> neighbor24id = {id1[0], id1[1], -id1[2] + 2 * id2[2] + 1, 0};
        int neighbor2id = lattice4id2id(neighbor24id);

        std::pair<int, int> neighbor1Edge = {lattice4id2id(id1), neighbor1id};
        std::pair<int, int> neighbor2Edge = {neighbor2id, lattice4id2id(id2)};

        return {neighbor1Edge, neighbor2Edge};
    }

    bool isNeighbor(std::pair<int, int> edge1, std::pair<int, int> edge2) {
        std::vector<std::pair<int, int>> neighbor1 = neighbor(edge1);
        std::vector<std::pair<int, int>> neighbor2 = neighbor(edge2);
        return (neighbor1 == edge2 || neighbor2 == edge2);
    }

    void createAddEvent(std::vector<int> &eventList) {
        for (auto &entry : latticeNodes) {
            int nodeId = entry.first;
            if (std::find(subgraphNodes.begin(), subgraphNodes.end(), nodeId) == subgraphNodes.end()) {
                int value = neighbors_count(nodeId);
                double rate = pow(0.25, value);
                eventList.push_back(nodeId);
                eventList.push_back(rate);
            }
        }
    }

    void createRemoveEvent(std::vector<std::pair<int, int>> &eventList) {
        std::vector<std::pair<int, int>> allEdgeCombinations1;
        std::vector<std::pair<int, int>> allEdgeCombinations2;

        for (int i = 0; i < subgraphNodes.size(); ++i) {
            for (int j = i + 1; j < subgraphNodes.size(); ++j) {
                allEdgeCombinations1.push_back({subgraphNodes[i], subgraphNodes[j]});
            }
        }

        for (int i = 0; i < subgraphNodes.size(); ++i) {
            for (int j = i + 1; j < subgraphNodes.size(); ++j) {
                for (int k = j + 1; k < subgraphNodes.size(); ++k) {
                    allEdgeCombinations2.push_back({subgraphNodes[i], subgraphNodes[j]});
                    allEdgeCombinations2.push_back({subgraphNodes[j], subgraphNodes[k]});
                    allEdgeCombinations2.push_back({subgraphNodes[i], subgraphNodes[k]});
                }
            }
        }

        std::vector<std::pair<int, int>> allEdgeCombinations = allEdgeCombinations1;
        allEdgeCombinations.insert(allEdgeCombinations.end(), allEdgeCombinations2.begin(), allEdgeCombinations2.end());

        std::vector<std::pair<int, int>> edgeCuts;
        for (auto &cut : allEdgeCombinations) {
            if (isEdgeCut(cut)) {
                edgeCuts.push_back(cut);
            }
        }

        for (auto &cut : edgeCuts) {
            int freeBonds = 0;
            std::vector<std::pair<int, int>> neighborEdges = neighbor(cut[0]);
            for (auto &edge : cut) {
                for (auto &neighborEdge : neighborEdges) {
                    if (!subgraphHasEdge(neighborEdge)) {
                        freeBonds += 2;
                    }
                }
            }
            if (cut.size() == 2 && isNeighbor(cut[0], cut[1])) {
                freeBonds += 2;
            }
            double rate = pow(0.25, cut.size()) * exp(g0 * cut.size() + g1 + freeBonds);
            eventList.push_back(cut);
            eventList.push_back(rate);
        }
    }

    bool subgraphHasEdge(std::pair<int, int> edge) {
        return std::find(subgraphEdges.begin(), subgraphEdges.end(), edge) != subgraphEdges.end();
    }

    void executeEvent(std::pair<int, double> event) {
        if (event.first >= 0) {
            addNode(event.first);
        } else {
            std::pair<int, int> cut = event.first;
            removeNodes(cut);
        }
    }

    void sample() {
        std::vector<std::pair<int, double>> eventList;
        createAddEvent(eventList);
        createRemoveEvent(eventList);

        int eventChosen;
        double totalRate = 0.0;
        for (const auto &event : eventList) {
            totalRate += event.second;
        }
        double r = (rand() / (RAND_MAX + 1.0)) * totalRate;
        double prefixSum = 0.0;

        for (int i = 0; i < eventList.size(); ++i) {
            prefixSum += eventList[i].second;
            if (prefixSum >= r) {
                eventChosen = i;
                break;
            }
        }

        executeEvent(eventList[eventChosen]);
    }
};

int main() {
    std::vector<int> seed;

    // Construct seed particles
    std::vector<std::vector<int>> layer4id = {{25, 25, 25, 0}, {25, 25, 25, 1}, {26, 25, 25, 0},
                                               {25, 26, 25, 1}, {25, 26, 25, 0}, {24, 26, 25, 1}};

    for (int i = 0; i < 6; ++i) {
        for (auto &atom : layer4id) {
            seed.push_back(lattice4id2id(atom));
        }
    }
    seed.push_back(lattice4id2id({26, 25, 25, 1}));

    GrowthSubgraph sample(seed);

    for (int i = 0; i < 500; ++i) {
        sample.sample();
    }

    return 0;
}
