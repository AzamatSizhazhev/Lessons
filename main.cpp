#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <string>

using namespace std;

class Vertex {
    int id;
    string name;

public:
    Vertex() : id(-1), name("") {}
    Vertex(int id, const string& name) : id(id), name(name) {}

    int getId() const { return id; }
    string getName() const { return name; }

    bool operator==(const Vertex& other) const {
        return id == other.id;
    }
};

class Edge {
    int to;
    int weight;

public:
    Edge() : to(-1), weight(0) {}
    Edge(int to, int weight) : to(to), weight(weight) {}

    int getTo() const { return to; }
    int getWeight() const { return weight; }

    bool operator<(const Edge& other) const {
        return weight > other.weight;
    }
};

class Graph {
    int n;
    vector<Vertex> vertices;
    vector<vector<Edge>> adj;

public:
    Graph(int n) : n(n), vertices(n), adj(n) {
        for (int i = 0; i < n; ++i)
            vertices[i] = Vertex(i, "v" + to_string(i));
    }

    int size() const { return n; }

    const Vertex& getVertex(int i) const { return vertices[i]; }

    void addEdge(int from, int to, int weight) {
        adj[from].push_back(Edge(to, weight));
        adj[to].push_back(Edge(from, weight));
    }

    const vector<Edge>& getNeighbors(int v) const { return adj[v]; }
};

vector<int> dijkstra(const Graph& g, int start) {
    const int INF = numeric_limits<int>::max();

    vector<int> dist(g.size(), INF);
    dist[start] = 0;

    priority_queue<Edge> pq;
    pq.push(Edge(start, 0));

    while (!pq.empty()) {
        Edge cur = pq.top();
        pq.pop();

        int v = cur.getTo();
        int d = cur.getWeight();

        if (d <= dist[v]) {
            const vector<Edge>& neighbors = g.getNeighbors(v);
            for (size_t i = 0; i < neighbors.size(); ++i) {
                int to = neighbors[i].getTo();
                int w  = neighbors[i].getWeight();

                if (dist[v] + w < dist[to]) {
                    dist[to] = dist[v] + w;
                    pq.push(Edge(to, dist[to]));
                }
            }
        }
    }

    return dist;
}

int main() {
    Graph g(6);
    g.addEdge(0, 1, 1);
    g.addEdge(1, 2, 2);
    g.addEdge(0, 3, 4);
    g.addEdge(3, 2, 1);
    g.addEdge(3, 4, 5);
    g.addEdge(4, 5, 3);

    vector<int> dist = dijkstra(g, 0);

    cout << "Кратчайшие расстояния от вершины 0:" << endl;
    for (size_t i = 0; i < dist.size(); ++i) {
        cout << "  до " << g.getVertex(i).getName() << ": ";
        if (dist[i] == numeric_limits<int>::max())
            cout << "INF";
        else
            cout << dist[i];
        cout << endl;
    }

    return 0;
}