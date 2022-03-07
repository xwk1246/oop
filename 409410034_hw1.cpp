#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <utility>
#include <fstream>

using namespace std;

class Link {
public:
    int linkId;
    unsigned int nodeA;
    unsigned int nodeB;
    int oldWeight;
    int newWeight;
};

class Neighbor {
public:
    unsigned int id;
    int oldWeight;
    int newWeight;
};

class Node {
public:
    unsigned int id;
    vector<Neighbor>neighbors;
};

class pqLink {
public:
    unsigned int from;
    unsigned int to;
    int oldWeight;
    int newWeight;
};

class CompareOld {
public:
    bool operator()(const pqLink& a, const pqLink& b) {
        if (a.oldWeight == b.oldWeight) return b.to < a.to;
        return b.oldWeight < a.oldWeight;
    }
};
class CompareNew {
public:
    bool operator()(const pqLink& a, const pqLink& b) {
        if (a.newWeight == b.newWeight) return b.to < a.to;
        return b.newWeight < a.newWeight;
    }
};
int dijkstra(map<unsigned int, unsigned int>& table, unsigned int dest, map<unsigned int, Node>& nodes, int nodeCnt, bool newWeight) {
    map<unsigned int, unsigned int> cost;
    priority_queue<pqLink, vector<pqLink>, CompareOld> oldPq;
    priority_queue<pqLink, vector<pqLink>, CompareNew> newPq;
    vector<bool> selected; // node selected
    pqLink current;
    selected.resize(nodeCnt);
    if (!newWeight) {
        selected[dest] = true;
        for (auto i : nodes[dest].neighbors) {
            oldPq.push({ dest, i.id, i.oldWeight, i.newWeight });
        }
        while (!oldPq.empty()) {
            current = oldPq.top();
            oldPq.pop();
            if (selected[current.to])continue;
            table[current.to] = current.from;
            cost[current.to] = current.oldWeight;

            selected[current.to] = true;
            for (auto i : nodes[current.to].neighbors) {
                if (!selected[i.id])
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
    vector<map<unsigned int, unsigned int>>tableList;
    vector<map<unsigned int, unsigned int>>newTableList;
    unsigned int nodeCnt, destCnt, linkCnt;
    vector<unsigned int> dests;


    cin >> nodeCnt >> destCnt >> linkCnt;
    for (unsigned int i = 0; i < destCnt; i++) {
        unsigned int dest;
        cin >> dest;
        dests.push_back(dest);
    }
    for (unsigned int i = 0; i < linkCnt; i++) {
        Link link;
        cin >> link.linkId >> link.nodeA >> link.nodeB >> link.oldWeight >> link.newWeight;
        links.push_back(link);
    }
    for (auto i : links) {
        nodes[i.nodeA].neighbors.push_back({ i.nodeB, i.oldWeight, i.newWeight });
        nodes[i.nodeB].neighbors.push_back({ i.nodeA, i.oldWeight, i.newWeight });
    }
    tableList.resize(destCnt);
    newTableList.resize(destCnt);
    for (unsigned int i = 0; i < destCnt; i++) {
        dijkstra(tableList[i], dests[i], nodes, nodeCnt, false);
    }
    for (unsigned int i = 0; i < destCnt; i++) {
        dijkstra(newTableList[i], dests[i], nodes, nodeCnt, true);
    }
    for (unsigned int i = 0; i < nodeCnt; i++) {
        cout << i << endl;
        for (unsigned int j = 0; j < destCnt; j++) {
            if (dests[j] != i)
                cout << dests[j] << " " << tableList[j][i] << endl;
        }
    }
    for (unsigned int i = 0; i < nodeCnt; i++) {
        int first = 1;
        for (unsigned int j = 0; j < destCnt; j++) {
            if (tableList[j][i] != newTableList[j][i]) {
                if (first == 1) {
                    cout << i << endl;
                    first = 0;
                }
                cout << dests[j] << " " << newTableList[j][i] << endl;
            }
        }
    }


    return 0;
}