#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

// ���� � ���� ������ ���������: ��� ���� ������������
unordered_map<char, vector<char>> graph;

// �������� ������� ������
unordered_map<char, int> in_degree;

// ��� ���������� �������, ����������� � �����
unordered_set<char> nodes;

// ����������� ������� ��� ����������� ����� ����� DFS
bool has_cycle_dfs(char node, unordered_set<char>& visited, unordered_set<char>& rec_stack) {
    visited.insert(node);
    rec_stack.insert(node);

    for (char neighbor : graph[node]) {
        if (rec_stack.count(neighbor)) return true;        // ���� ������
        if (!visited.count(neighbor) && has_cycle_dfs(neighbor, visited, rec_stack))
            return true;
    }

    rec_stack.erase(node); // ����� �� �������� ����� ������
    return false;
}

// �������� �� ������� �����
bool STL_has_cycle() {
    unordered_set<char> visited;
    unordered_set<char> rec_stack;

    for (char node : nodes) {
        if (!visited.count(node)) {
            if (has_cycle_dfs(node, visited, rec_stack))
                return true;
        }
    }
    return false;
}

// ��������, ����� �� ��������� ���������� �������������� �������
bool STL_is_unique_topo_sort() {
    unordered_map<char, int> in_deg_copy = in_degree;
    queue<char> q;
    int count = 0;

    // ������� ��� ������� � �������� �������� 0
    for (char node : nodes) {
        if (in_deg_copy[node] == 0) {
            q.push(node);
        }
    }

    while (!q.empty()) {
        if (q.size() > 1) return false; // ������ ����� ������� � 0 �������� �������� � ������� �� ��������

        char current = q.front(); q.pop();
        count++;

        for (char neighbor : graph[current]) {
            if (--in_deg_copy[neighbor] == 0)
                q.push(neighbor);
        }
    }

    return count == nodes.size();
}

/*
int main() {
    setlocale(LC_ALL, "rus");
    cout << "������� ���� �������� (��������: ab bc ca), ����� EOF (Ctrl+Z �� Windows):\n";


    string pair;
    while (cin >> pair && pair.length() == 2) {
        char from = pair[0];
        char to = pair[1];

        // ���������� �����
        graph[from].push_back(to);
        in_degree[to]++;
        nodes.insert(from);
        nodes.insert(to);

        // ��������, ��� from ���� ���� � in_degree
        if (!in_degree.count(from))
            in_degree[from] = 0;
    }
    auto start = std::chrono::high_resolution_clock::now();
    if (STL_has_cycle()) {
        cout << "������������������ �������������� (���� ����)." << endl;
    }
    else if (STL_is_unique_topo_sort()) {
        cout << "������������������ ������ (���������� ������� ����������)." << endl;
    }
    else {
        cout << "������������������ �������� (�������� ������ �������)." << endl;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    cout << "����� ����������: " << duration.count() << " ������\n";

    return 0;
}
*/
