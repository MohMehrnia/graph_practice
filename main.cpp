#include <iostream>
#include <queue>
#include <stack>
#include "rapidcsv.h"
#include <algorithm>
#include <regex>
#include<list>
#include<set>

using namespace std;

typedef pair<int, int> vertex;
typedef pair<int, int> iPair;


struct countryInfo {
    string country;
    string adjacency_List;
};

struct Graph {
    int V, E;
    vector<pair<int, iPair> > edges;
    vector<countryInfo> countries;
    Graph(int V, int E) {
        this->V = V;
        this->E = E;
    }
    string ltrim(std::string str) {
        return std::regex_replace(str, std::regex("^\\s+"), std::string(""));
    }

    string rtrim(std::string str) {
        return std::regex_replace(str, std::regex("\\s+$"), std::string(""));
    }

    string trim(std::string str) {
        return ltrim(rtrim(str));
    }
    void addEdge(int u, int v, int w) {
        edges.push_back({w, {u, v}});
    }

    int getCountryIndex(string key) {
        for (size_t i = 0; i < this->countries.size(); ++i)
            if (this->countries[i].country == trim(key))
                return i;
        return -1;
    }
    vector<string> tokenize(const string &s, char c) {
        auto end = s.cend();
        auto start = end;
        std::vector<std::string> v;
        for (auto it = s.cbegin(); it != end; ++it) {
            if (*it != c) {
                if (start == end)
                    start = it;
                continue;
            }
            if (start != end) {
                v.emplace_back(start, it);
                start = end;
            }
        }
        if (start != end)
            v.emplace_back(start, end);
        return v;
    }

    string removeAll(string str, char c) {
        size_t offset = 0;
        size_t size = str.size();

        size_t i = 0;
        while (i < size - offset) {
            if (str[i + offset] == c) {
                offset++;
            }

            if (offset != 0) {
                str[i] = str[i + offset];
            }

            i++;
        }

        str.resize(size - offset);
        return str;
    }
    void fillGraphFromFile() {
        int index = 0;

        rapidcsv::Document doc("/home/mohammad/MySourceCodes/c++/graph_Practice/texas_counties_adjacency.csv");
        vector<string> results;

        while (index <= doc.GetRowCount() - 1) {
            results = doc.GetRow<string>(index);
            this->countries.push_back({results[0].c_str(), results[1].c_str()});
            index++;
        }

        index = 0;
        while (index <= this->countries.size() - 1) {
            string adList = removeAll(this->countries[index].adjacency_List, '(');
            adList = removeAll(adList, ')');
            vector<string> connectedList = tokenize(adList, ';');
            int cityListIndex = 0;
            while (cityListIndex <= connectedList.size() - 1) {
                vector<string> connectedCity = tokenize(connectedList[cityListIndex], '-');
                int des = getCountryIndex(connectedCity[0].c_str());
                int weight = atoi(connectedCity[1].c_str());
                addEdge(index, des, weight);
                cityListIndex++;
            }
            index++;
        }
    };

    int kruskalMST();
};


struct DisjointSets {
    int *parent, *rnk;
    int n;
    DisjointSets(int n) {
        this->n = n;
        parent = new int[n + 1];
        rnk = new int[n + 1];
        for (int i = 0; i <= n; i++) {
            rnk[i] = 0;
            parent[i] = i;
        }
    }
    int find(int u)
    {
        if (u != parent[u])
            parent[u] = find(parent[u]);
        return parent[u];
    }
    void merge(int x, int y)
    {
        x = find(x), y = find(y);
        if (rnk[x] > rnk[y])
            parent[y] = x;
        else
            parent[x] = y;

        if (rnk[x] == rnk[y])
            rnk[y]++;
    }
};

int Graph::kruskalMST()
{
    int mst_wt = 0;

    sort(edges.begin(), edges.end());
    DisjointSets ds(V);
    vector< pair<int, iPair> >::iterator it;
    for (it=edges.begin(); it!=edges.end(); it++)
    {
        int u = it->second.first;
        int v = it->second.second;

        int set_u = ds.find(u);
        int set_v = ds.find(v);

        if (set_u != set_v)
        {
            cout << u << " - " << v << endl;

            mst_wt += it->first;

            ds.merge(set_u, set_v);
        }
    }

    return mst_wt;
}

class Graph_ {
private:
    inline void printEdgeVertex(vertex j) {
        cout << " (V = " << this->countries[j.first].country << " , " << "W = " << j.second << " ),";
    }

    string ltrim(std::string str) {
        return std::regex_replace(str, std::regex("^\\s+"), std::string(""));
    }

    string rtrim(std::string str) {
        return std::regex_replace(str, std::regex("\\s+$"), std::string(""));
    }

    string trim(std::string str) {
        return ltrim(rtrim(str));
    }

    void printAsBFS(int start) {
        cout << endl;
        cout << "BFS : " << endl;
        queue<int> bfsQueue;
        vector<bool> visited(graphSize);
        visited[start] = true;
        bfsQueue.push(start);
        while (!bfsQueue.empty()) {
            int front = bfsQueue.front();
            cout << countries[front].country << " ";
            bfsQueue.pop();
            for (vector<vertex>::iterator i = this->adjList[front].begin();
                 i != this->adjList[front].end();
                 ++i) {
                if (!visited[i->first]) {
                    visited[i->first] = true;
                    bfsQueue.push(i->first);
                }
            }
        }
    }

    void printAsDFS(int start) {
        cout << endl;
        cout << "DFS :" << endl;
        stack<int> dfsStack;
        vector<bool> visited(graphSize);
        dfsStack.push(0);
        while (!dfsStack.empty()) {
            int top = dfsStack.top();
            dfsStack.pop();
            if (!visited[top]) {
                visited[top] = true;
                cout << countries[top].country << " ";
            }
            for (vector<vertex>::iterator i = this->adjList[top].begin();
                 i != this->adjList[top].end();
                 ++i) {
                if (!visited[i->first]) {
                    dfsStack.push(i->first);
                }
            }

        }
    }

public:
    vector<vertex> *adjList;
    size_t graphSize;
    vector<countryInfo> countries;

    Graph_(size_t graphSize) {
        this->graphSize = graphSize;
        adjList = new vector<vertex>[graphSize];
    }

    void addVertex(int source, int destination, int weight) {
        adjList[source].push_back(make_pair(destination, weight));
    }

    vector<string> tokenize(const string &s, char c) {
        auto end = s.cend();
        auto start = end;
        std::vector<std::string> v;
        for (auto it = s.cbegin(); it != end; ++it) {
            if (*it != c) {
                if (start == end)
                    start = it;
                continue;
            }
            if (start != end) {
                v.emplace_back(start, it);
                start = end;
            }
        }
        if (start != end)
            v.emplace_back(start, end);
        return v;
    }

    void fillGraphFromFile() {
        int index = 0;

        rapidcsv::Document doc("/home/mohammad/MySourceCodes/c++/graph_Practice/texas_counties_adjacency.csv");
        this->graphSize = doc.GetRowCount();
        adjList = new vector<vertex>[graphSize];
        vector<string> results;

        while (index <= doc.GetRowCount() - 1) {
            results = doc.GetRow<string>(index);
            this->countries.push_back({results[0].c_str(), results[1].c_str()});
            index++;
        }

        index = 0;
        while (index <= this->countries.size() - 1) {
            string adList = removeAll(this->countries[index].adjacency_List, '(');
            adList = removeAll(adList, ')');
            vector<string> connectedList = tokenize(adList, ';');
            int cityListIndex = 0;
            while (cityListIndex <= connectedList.size() - 1) {
                vector<string> connectedCity = tokenize(connectedList[cityListIndex], '-');
                int des = getCountryIndex(connectedCity[0].c_str());
                int weight = atoi(connectedCity[1].c_str());
                addVertex(index, des, weight);
                cityListIndex++;
            }
            index++;
        }
    };

    int getCountryIndex(string key) {
        for (size_t i = 0; i < this->countries.size(); ++i)
            if (this->countries[i].country == trim(key))
                return i;
        return -1;
    }

    string removeAll(string str, char c) {
        size_t offset = 0;
        size_t size = str.size();

        size_t i = 0;
        while (i < size - offset) {
            if (str[i + offset] == c) {
                offset++;
            }

            if (offset != 0) {
                str[i] = str[i + offset];
            }

            i++;
        }

        str.resize(size - offset);
        return str;
    }

    void printAsList() {
        for (size_t i = 0; i < this->graphSize; ++i) {
            cout << this->countries[i].country << " -> ";
            for (vector<pair<int, int>>::iterator
                         j = this->adjList[i].begin();
                 j != this->adjList[i].end();
                 ++j) {
                printEdgeVertex(*j);
            }
            cout << " X" << endl;
        }
    }

    void printAsBFS() {
        printAsBFS(0);
    }

    void printAsDFS() {
        printAsDFS(0);
    }

    set<int> difference(set<int> first, set<int> second) {
        set<int>::iterator it;
        set<int> res;

        for (it = first.begin(); it != first.end(); it++) {
            if (second.find(*it) == second.end())
                res.insert(*it);
        }

        return res;
    }

    Graph_ primsMST(int start) {
        int n = this->graphSize;
        Graph_ tree = Graph_(10);
        tree.adjList->clear();
        set<int> B, N, diff;
        B.insert(start);

        for (int u = 0; u < n; u++) {
            N.insert(u);
        }

        while (B != N) {
            int min = 9999;
            int v, par;
            diff = difference(N, B);

            for (int u = 0; u < n; u++) {
                if (B.find(u) != B.end()) {
                    for (vector<pair<int, int>>::iterator
                                 j = this->adjList[u].begin();
                         j != this->adjList[u].end();
                         j++) {
                        if (diff.find(j->first) != diff.end()) {
                            if (min > j->second) {
                                min = j->second;
                                par = u;
                                v = j->first;
                            }
                        }
                    }
                }
            }

            B.insert(v);
            cout << par << "->" << v << "," << min << endl;
            cout << v << "->" << par << "," << min << endl;
            //tree.addVertex(par, v, min);
            //tree.addVertex(v, par, min);
        }
        return tree;
    }

    ~Graph_() {
        delete[] adjList;
    }

};


int main() {
    // Graph_ citiesGraph(5);
    // citiesGraph.fillGraphFromFile();

    // citiesGraph.printAsList();
    // citiesGraph.printAsBFS();
    // citiesGraph.printAsDFS();
    //citiesGraph.primsMST(0);

    int V = 256, E = 256;
    Graph g(V, E);
    g.fillGraphFromFile();
    int mst_wt = g.kruskalMST();
    cout << "\nWeight of MST is " << mst_wt;
    return 0;
}