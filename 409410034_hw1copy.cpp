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
int dijkstra(map<unsigned int, unsigned int>& table, unsigned int dest, vector<Node>& nodes, int nodeCnt, bool newWeight, map<unsigned int, unsigned int>& cost) {
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
    vector<Node> nodes; // nodes list and their neighbor
    map<unsigned int, unsigned int>destATable; // route table
    map<unsigned int, unsigned int>destBTable; // route table
    map<unsigned int, unsigned int>destANewTable; // route table
    map<unsigned int, unsigned int>destBNewTable; // route table
    map<unsigned int, unsigned int>destACost; // route cost
    map<unsigned int, unsigned int>destBCost; // route cost
    map<unsigned int, unsigned int>destANewCost; // route cost
    map<unsigned int, unsigned int>destBNewCost; // route cost
    int nodeCnt, destCnt, linkCnt;
    unsigned int destA, destB;

    // ifstream in("in.txt");
    // streambuf* oldBuf = cin.rdbuf(in.rdbuf());

    cin >> nodeCnt >> destCnt >> linkCnt;
    cin >> destA >> destB;
    for (int i = 0; i < linkCnt; i++) {
        Link link;
        cin >> link.linkId >> link.nodeA >> link.nodeB >> link.oldWeight >> link.newWeight;
        links.push_back(link);
    }
    nodes.resize(nodeCnt);
    for (auto i : links) {
        nodes[i.nodeA].id = i.nodeA;
        nodes[i.nodeA].neighbors.push_back({ i.nodeB, i.oldWeight, i.newWeight });
        nodes[i.nodeB].id = i.nodeB;
        nodes[i.nodeB].neighbors.push_back({ i.nodeA, i.oldWeight, i.newWeight });
    }
    dijkstra(destATable, destA, nodes, nodeCnt, false, destACost);
    dijkstra(destBTable, destB, nodes, nodeCnt, false, destBCost);
    dijkstra(destANewTable, destA, nodes, nodeCnt, true, destANewCost);
    dijkstra(destBNewTable, destB, nodes, nodeCnt, true, destBNewCost);
    for (unsigned int i = 0; i < nodeCnt; i++) {
        cout << i << endl;
        if (destA != i)
            cout << destA << " " << destATable[i] << endl;
        if (destB != i)
            cout << destB << " " << destBTable[i] << endl;
    }
    for (unsigned int i = 0; i < nodeCnt; i++) {
        if (destANewTable[i] != destATable[i] || destBNewTable[i] != destBTable[i]) {
            cout << i << endl;
            if (destANewTable[i] != destATable[i]) {
                cout << destA << " " << destANewTable[i] << endl;
            }
            if (destBNewTable[i] != destBTable[i]) {
                cout << destB << " " << destBNewTable[i] << endl;
            }
        }
    }


    // cin.rdbuf(oldBuf);
    // in.close();
    return 0;
}