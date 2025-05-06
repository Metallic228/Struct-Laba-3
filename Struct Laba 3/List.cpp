#include <iostream>
#include <cstring>
#include <queue>
#include <chrono>
#define MAX 26 // ���������� ���� ����������� ��������
using namespace std;



// ���� ���������� ������
struct ListNode {
    int vertex;
    ListNode* next;
};

// ������ ������� ���������
ListNode* adj[MAX] = { nullptr };

bool List_used[MAX] = { false };
int List_in_degree[MAX] = { 0 };

// ���������� �����
void add_edge(int from, int to) {
    ListNode* node = new ListNode{ to, adj[from] };
    adj[from] = node;
    List_in_degree[to]++;
    List_used[from] = List_used[to] = true;
}

// �������� �� ���� � ������� DFS
bool List_has_cycle_util(int v, bool visited[], bool rec_stack[]) {
    visited[v] = true;
    rec_stack[v] = true;

    for (ListNode* node = adj[v]; node; node = node->next) {
        int u = node->vertex;
        if (!visited[u] && List_has_cycle_util(u, visited, rec_stack))
            return true;
        else if (rec_stack[u])
            return true;
    }

    rec_stack[v] = false;
    return false;
}

bool List_has_cycle() {
    bool visited[MAX] = {};
    bool rec_stack[MAX] = {};

    for (int i = 0; i < MAX; ++i) {
        if (List_used[i] && !visited[i]) {
            if (List_has_cycle_util(i, visited, rec_stack))
                return true;
        }
    }
    return false;
}

// �������� �� �������������� �������������� ����������
bool is_unique_topo_sort() {
    int in_deg_copy[MAX];
    memcpy(in_deg_copy, List_in_degree, sizeof(List_in_degree));
    queue<int> q;

    // ��������� ������� � ������� �������� ��������
    for (int i = 0; i < MAX; ++i) {
        if (List_used[i] && in_deg_copy[i] == 0)
            q.push(i);
    }

    int visited_count = 0;

    while (!q.empty()) {
        if (q.size() > 1)
            return false;  // ��������� ��������� ������ => ������������ �������

        int curr = q.front(); q.pop();
        visited_count++;

        for (ListNode* node = adj[curr]; node; node = node->next) {
            int neighbor = node->vertex;
            if (--in_deg_copy[neighbor] == 0)
                q.push(neighbor);
        }
    }

    // ���������, ��� �� ������� ���� ��������
    for (int i = 0; i < MAX; ++i) {
        if (List_used[i] && in_deg_copy[i] > 0)
            return false;  // �� ��� ������� ���� ����������
    }

    return true;
}
/*
int main() {
    setlocale(LC_ALL, "rus");
    cout << "������� ���������� ���: ";
    int n;
    cin >> n;
    cout << "������� ���� (��������: ab):\n";

    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        if (s.length() == 2) {
            int from = s[0] - 'a';
            int to = s[1] - 'a';
            add_edge(from, to);
        }
    }

    auto start = std::chrono::high_resolution_clock::now();

    if (List_has_cycle()) {
        cout << "������������������ �������������.\n";
    }
    else if (is_unique_topo_sort()) {
        cout << "������������������ ������.\n";
    }
    else {
        cout << "������������������ ���������, �� �� ������.\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    cout << "����� ����������: " << duration.count() << " ������\n";

    // ������� ������
    for (int i = 0; i < MAX; ++i) {
        while (adj[i]) {
            ListNode* tmp = adj[i];
            adj[i] = adj[i]->next;
            delete tmp;
        }
    }

    return 0;
}
*/