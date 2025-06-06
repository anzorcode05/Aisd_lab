#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <queue>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <utility> 
#include <vector>

template<typename Vertex, typename Distance = double>
class Graph {
public:
    struct Edge {
        Vertex from;
        Vertex to;
        Distance distance;
    };
private:
    std::unordered_map<Vertex, std::vector<Edge>> adjacency_list;

    void dfs_recursive(const Vertex& current,
        const std::unordered_map<Vertex, std::vector<Edge>>& graph,
        std::unordered_set<Vertex>& visited,
        std::vector<Vertex>& result) const {
        visited.insert(current);
        result.push_back(current);

        auto it = graph.find(current);
        if (it != graph.end()) {
            for (const auto& edge : it->second) {
                if (!visited.contains(edge.to)) {
                    dfs_recursive(edge.to, graph, visited, result);
                }
            }
        }
    }

    std::vector<Vertex> dfs(const Vertex& start,
        const std::unordered_map<Vertex, std::vector<Edge>>& graph) const {
        std::unordered_set<Vertex> visited;
        std::vector<Vertex> result;
        dfs_recursive(start, graph, visited, result);
        return result;
    }
public:
    Graph() = default;
    ~Graph() = default;

    bool has_vertex(const Vertex& v) const {
        return adjacency_list.contains(v);
    }

    bool add_vertex(const Vertex& v) {
        if (has_vertex(v)) {
            return false;
        }

        adjacency_list[v] = {};
        return true;
    }

    bool remove_vertex(const Vertex& v) {
        if (!has_vertex(v)) {
            return false;
        }

        adjacency_list.erase(v);

        for (auto& [from, edges] : adjacency_list) {
            int i = 0;
            int j = edges.size() - 1;
            while (i < j) {
                if (edges[i].to != v) {
                    ++i;
                    continue;
                }

                swap(edges[i], edges[j]);
                --j;
            }
            edges.erase(edges.begin() + i, edges.end());//i - стоит на первой вершине у которой to == v если такая веришна существует
        }
        return true;
    }

    std::vector<Vertex> vertices() const {
        std::vector<Vertex> ans;

        for (const auto& [from, edge] : adjacency_list) {
            ans.push_back(from);
        }
        sort(ans.begin(), ans.end()); //отсортировали для красоты, вообще нас об этом не просили
        return ans;
    }

    void add_edge(const Vertex& from, const Vertex& to, const Distance& d) {
        add_vertex(to);
        adjacency_list[from].emplace_back(from, to, d);
    }

    bool remove_edge(const Vertex& from, const Vertex& to) {
        if (!(has_vertex(from) && has_vertes(to))) {
            return false;
        }

        auto& edges = adjacency_list[from];
        int size_before_remove = edges.size();

        int i = 0;
        int j = edges.size() - 1;
        while (i < j) {
            if (edges[i].to != to) {
                ++i;
                continue;
            }

            swap(edges[i], edges[j]);
            --j;
        }
        edges.erase(edges.begin() + i, edges.end());//i - стоит на первой вершине у которой to == v если такая веришна существует

        return size_before_remove == edges.size();
    }

    bool remove_edge(const Edge& e) {
        if (!(has_vertex(e.from) && has_vertes(e.to))) {
            return false;
        }

        auto& edges = adjacency_list[e.from];
        int i = 0;
        int j = edges.size() - 1;
        int size_before_remove = edges.size();
        while (i < j) {
            if (!(edges[i].to == e.to && edges[i].distance == e.distance)) {
                ++i;
                continue;
            }

            swap(edges[i], edges[j]);
            --j;
        }
        edges.erase(edges.begin() + i, edges.end());//i - стоит на первой вершине у которой to == v если такая веришна существует
        return size_before_remove == edges.size();
    }

    bool has_edge(const Vertex& from, const Vertex& to) const {
        if (!(has_vertex(from) && has_vertes(to))) {
            return false;
        }

        const auto& edges = adjacency_list.at(from);
        for (const auto& edge : edges) {
            if (edge.to == to) {
                return true;
            }
        }
        return false;
    }

    bool has_edge(const Edge& e) const {
        if (!(has_vertex(e.from) && has_vertes(e.to))) {
            return false;
        }

        const auto& edges = adjacency_list.at(e.from);
        for (const auto& edge : edges) {
            if (edge.to == e.to && edge.distance == e.distance) {
                return true;
            }
        }
        return false;
    }

    std::vector<Edge> edges(const Vertex& v) const {
        if (!has_vertex(v)) {
            return {};
        }

        return adjacency_list.at(v);
    }

    size_t order() const {
        return adjacency_list.size();
    }

    size_t degree(const Vertex& v) const {
        if (!has_vertex(v)) {
            return 0;
        }
        return adjacency_list.at(v).size();
    }

    bool is_connected() const {
        if (adjacency_list.empty()) return true;

        Vertex start = adjacency_list.begin()->first;

        if (walk(start).size() != adjacency_list.size()) return false;

        std::unordered_map<Vertex, std::vector<Edge>> reversed_list;
        for (const auto& [u, edges] : adjacency_list) {
            for (const auto& edge : edges) {
                reversed_list[edge.to].push_back({ edge.to, edge.from, edge.distance });
            }
        }

        auto visited = dfs(start, reversed_list);
        return visited.size() == adjacency_list.size();
    }

    std::vector<Edge> shortest_path(const Vertex& from, const Vertex& to) const {
        if (!(has_vertex(from) && has_vertex(to))) {
            return {};
        }

        for (const auto& pair : adjacency_list) {
            for (const auto& edge : pair.second) {
                if (edge.distance < 0) {
                    throw std::runtime_error("Error");
                }
            }
        }

        std::unordered_map<Vertex, Distance> distances;
        std::unordered_map<Vertex, Vertex> previous;

        for (const auto& [from, _] : adjacency_list) {
            distances[from] = std::numeric_limits<Distance>::infinity();
        }

        distances[from] = static_cast<Distance>(0);

        typedef std::pair<Distance, Vertex> QueueElement;
        auto cmp = [](const QueueElement& a, const QueueElement& b) { return a.first > b.first; };
        std::priority_queue<QueueElement, std::vector<QueueElement>, decltype(cmp)> queue(cmp);
        queue.push(std::make_pair(static_cast<Distance>(0), from));

        while (!queue.empty()) {
            auto [current_distance, current_vertex] = queue.top();
            queue.pop();

            if (current_vertex == to) {
                break;
            }

            for (const auto& edge : adjacency_list.at(current_vertex)) {
                Distance new_distance = current_distance + edge.distance;

                if (new_distance < distances[edge.to]) {
                    distances[edge.to] = new_distance;
                    previous[edge.to] = current_vertex;
                    queue.push(std::make_pair(new_distance, edge.to));
                }
            }
        }

        if (distances[to] == std::numeric_limits<Distance>::infinity()) {
            return {};
        }

        std::vector<Edge> path;
        for (Vertex v = to; v != from; v = previous[v]) {
            Vertex u = previous[v];
            for (const auto& edge : adjacency_list.at(u)) {
                if (edge.to == v) {
                    path.push_back(edge);
                    break;
                }
            }
        }
        std::reverse(path.begin(), path.end());
        return path;
    }

    std::vector<Vertex> walk(const Vertex& start_v) const {
        std::unordered_set<Vertex> visited;
        std::queue<Vertex> queue;
        std::vector<Vertex> result;

        if (!has_vertex(start_v)) {
            return result;
        }

        queue.push(start_v);
        visited.insert(start_v);

        while (!queue.empty()) {
            Vertex current = queue.front();
            queue.pop();
            result.push_back(current);

            for (const auto& edge : adjacency_list.at(current)) {
                if (visited.contains(edge.to)) continue;

                visited.insert(edge.to);
                queue.push(edge.to);
            }
        }
        return result;
    }
};


void task() {
    /*
    Пусть дан связный граф, в котором узлы – это торговые точки.
    Необходимо превратить одну из торговых точек в склад.
    Цена доставки от склада в точку зависит от расстояния.
    Найдите оптимальную с точки зрения максимальных затрат точку
    (т.е. точку, для которой максимальное расстояние до любой другой точки минимально).
    */

    Graph<std::string> graph;

    graph.add_edge("Shop_1", "Shop_2", 4.0);
    graph.add_edge("Shop_1", "Shop_3", 8.0);
    graph.add_edge("Shop_2", "Shop_3", 2.0);
    graph.add_edge("Shop_3", "Shop_4", 1.0);
    graph.add_edge("Shop_4", "Shop_1", 6.0);

    if (!graph.is_connected()) {
        std::cout << "Graph is not strongly connected.\n";
        return;
    }

    std::vector<std::string> vertices = graph.vertices();
    std::string warehouse;

    double best_max_distance = std::numeric_limits<double>::infinity(); // общий минимум

    for (const auto& candidate : vertices) {
        double max_distance = 0.0;

        //Вычисление кратчайших путей для каждой пары (candidate, target)
        for (const auto& target : vertices) {
            if (candidate == target) {
                continue;
            }

            auto shortest_path = graph.shortest_path(candidate, target);
            double path_distance = 0.0;
            for (const auto& edge : shortest_path) {
                path_distance += edge.distance;
            }

            if (path_distance > max_distance) {
                max_distance = path_distance;
            }

        }
        if (max_distance < best_max_distance) {
            best_max_distance = max_distance;
            warehouse = candidate;
        }
    }

    std::cout << "Optimal warehouse location: " << warehouse << " with maximum distance to any shop: " << best_max_distance << "\n";
}

int main() {
    task();

    return 0;
}