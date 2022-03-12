#include <iostream>
#include <vector>
#include <queue>
#include <map>

using namespace std;

class Link { // input links
public:
    int linkId;
    unsigned int nodeA;
    unsigned int nodeB;
    int oldWeight;
    int newWeight;
};

class Neighbor { // neighbor and weight between
public:
    unsigned int id;
    int oldWeight;
    int newWeight;
};

class Node { // node and it's neighbor list
public:
    unsigned int id;
    vector<Neighbor>neighbors;
};

class pqLink { // structure for dijkstra priority queue
public:
    unsigned int from;
    unsigned int to;
    int oldWeight;
    int newWeight;
};

class CompareOld { // compare function using old weight
public:
    bool operator()(const pqLink& a, const pqLink& b) {
        if (a.oldWeight == b.oldWeight) return b.from < a.from; // use smaller id when weight is equal
        return b.oldWeight < a.oldWeight;
    }
};
class CompareNew { // new weight
public:
    bool operator()(const pqLink& a, const pqLink& b) {
        if (a.newWeight == b.newWeight) return b.from < a.from;
        return b.newWeight < a.newWeight;
    }
};
int dijkstra(map<unsigned int, unsigned int>& table, unsigned int dest, map<unsigned int, Node>& nodes, int nodeCnt, bool newWeight) {
    map<unsigned int, unsigned int> cost; // path cost for each point
    priority_queue<pqLink, vector<pqLink>, CompareOld> oldPq; // two priority for using different compare function
    priority_queue<pqLink, vector<pqLink>, CompareNew> newPq;
    vector<bool> selected; // node selected already
    pqLink current;
    selected.resize(nodeCnt);
    if (!newWeight) { // if comparing old weight
        selected[dest] = true;
        for (auto i : nodes[dest].neighbors) {
            oldPq.push({ dest, i.id, i.oldWeight, i.newWeight }); // push all neighbors of dest
        }
        while (!oldPq.empty()) {
            current = oldPq.top(); // get top of pq and pop
            oldPq.pop();
            if (selected[current.to])continue; // continue when selected already
            table[current.to] = current.from; // update route table by the smallest cost path
            cost[current.to] = current.oldWeight; // upsate cost

            selected[current.to] = true; // update selected
            for (auto i : nodes[current.to].neighbors) { // push all neighbors of the popped node
                if (!selected[i.id]) // check if not selected
                    oldPq.push({ current.to, i.id, i.oldWeight + (int)cost[current.to] , i.newWeight });
            }

        }
    }
    else {
        selected[dest] = true;
        for (auto i : nodes[dest].neighbors) {
            newPq.push({ dest, i.id, i.oldWeight, i.newWeight });
        }
        while (!newPq.empty()) {
            current = newPq.top();
            newPq.pop();
            if (selected[current.to]) continue;
            table[current.to] = current.from;
            cost[current.to] = current.newWeight;

            selected[current.to] = true;
            for (auto i : nodes[current.to].neighbors) {
                if (!selected[i.id])
                    newPq.push({ current.to, i.id, i.oldWeight  , i.newWeight + (int)cost[current.to] });
            }

        }
    }
    return 1;
}

int main(int argc, char** argv) {
    vector<Link> links; // save input
    map<unsigned int, Node> nodes; // nodes list and their neighbor
    vector<map<unsigned int, unsigned int>>tableList; // route table list for different dest
    vector<map<unsigned int, unsigned int>>newTableList; // route table using new weight
    unsigned int nodeCnt, destCnt, linkCnt;
    vector<unsigned int> dests; // destination list


    cin >> nodeCnt >> destCnt >> linkCnt; // read counts
    for (unsigned int i = 0; i < destCnt; i++) { // push destinations to list
        unsigned int dest;
        cin >> dest;
        dests.push_back(dest);
    }
    for (unsigned int i = 0; i < linkCnt; i++) { // save links to list
        Link link;
        cin >> link.linkId >> link.nodeA >> link.nodeB >> link.oldWeight >> link.newWeight;
        links.push_back(link);
    }
    for (auto i : links) { // build node list and their neighbors
        nodes[i.nodeA].neighbors.push_back({ i.nodeB, i.oldWeight, i.newWeight });
        nodes[i.nodeB].neighbors.push_back({ i.nodeA, i.oldWeight, i.newWeight });
    }
    tableList.resize(destCnt); // resize the table
    newTableList.resize(destCnt);
    for (unsigned int i = 0; i < destCnt; i++) {
        dijkstra(tableList[i], dests[i], nodes, nodeCnt, false); // shortest path algorithm, using old weight
    }
    for (unsigned int i = 0; i < destCnt; i++) {
        dijkstra(newTableList[i], dests[i], nodes, nodeCnt, true); // new weight
    }
    for (unsigned int i = 0; i < nodeCnt; i++) { // print old route table
        cout << i << endl;
        for (unsigned int j = 0; j < destCnt; j++) {
            if (dests[j] != i)
                cout << dests[j] << " " << tableList[j][i] << endl;
        }
    }
    for (unsigned int i = 0; i < nodeCnt; i++) { // print changed route
        int first = 1;
        for (unsigned int j = 0; j < destCnt; j++) {
            if (tableList[j][i] != newTableList[j][i]) {
                if (first == 1) {   // print node id only at the first time
                    cout << i << endl;
                    first = 0;
                }
                cout << dests[j] << " " << newTableList[j][i] << endl;
            }
        }
    }


    return 0;
}