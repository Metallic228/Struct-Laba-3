#include <iostream>
#include <queue>
#include <vector>
#include <cstring>
#include <chrono>
using namespace std;

#define MAX 26  // количество возможных символов (буквы a-z)

// Граф представлен матрицей смежности
bool adj[MAX][MAX];   // adj[i][j] = true, если i предшествует j
int in_degree[MAX];   // количество входящих рёбер для каждой вершины
bool used[MAX];       // флаг, использовался ли символ в вводе

// Проверка на единственность топологической сортировки (Kahn's algorithm)
bool is_unique_topo_sort(int total_nodes) {
    queue<int> q;
    int processed = 0;

    // Копируем массив входящих рёбер, чтобы не испортить оригинал
    int in_deg_copy[MAX];
    memcpy(in_deg_copy, in_degree, sizeof(in_degree));

    // Добавляем все вершины с нулевым входом
    for (int i = 0; i < MAX; ++i) {
        if (used[i] && in_deg_copy[i] == 0)
            q.push(i);
    }

    while (!q.empty()) {
        // Если на текущем шаге в очереди больше одной вершины —
        // возможны разные варианты сортировки, значит порядок не единственный
        if (q.size() > 1)
            return false;

        int node = q.front();
        q.pop();
        processed++;

        // Уменьшаем количество входящих рёбер у всех соседей
        for (int i = 0; i < MAX; ++i) {
            if (adj[node][i]) {
                in_deg_copy[i]--;
                if (in_deg_copy[i] == 0)
                    q.push(i);
            }
        }
    }

    // Если мы обработали все вершины, то сортировка завершена успешно
    return processed == total_nodes;
}

// Вспомогательная функция для DFS и поиска цикла
bool has_cycle_util(int node, bool visited[], bool rec_stack[]) {
    visited[node] = true;       // Помечаем текущую вершину как посещённую
    rec_stack[node] = true;     // Помещаем вершину в стек рекурсии

    // Рекурсивно обходим всех соседей
    for (int i = 0; i < MAX; ++i) {
        if (adj[node][i]) {    // Если есть ребро от node к i (т.е. сосед)
            if (!visited[i] && has_cycle_util(i, visited, rec_stack))
                return true;  // Если сосед не посещён и есть цикл в его поддереве
            else if (rec_stack[i])
                return true;  // Если сосед в текущем стеке рекурсии, то цикл найден
        }
    }

    rec_stack[node] = false; // Убираем вершину из стека рекурсии (больше не в текущем пути)
    return false;            // Если цикл не найден, возвращаем false
}

// Проверка графа на наличие цикла
bool has_cycle() {
    bool visited[MAX] = {};        // Массив для отслеживания посещённых вершин
    bool rec_stack[MAX] = {};      // Массив для отслеживания вершин в текущем пути (стек рекурсии)

    // Проверяем каждую вершину, которая используется
    for (int i = 0; i < MAX; ++i) {
        if (used[i] && !visited[i]) {  // Если вершина используется и не была посещена
            if (has_cycle_util(i, visited, rec_stack))  // Вызываем рекурсивную проверку цикла
                return true;  // Если цикл найден, возвращаем true
        }
    }
    return false;  // Если цикл не найден после обхода всех вершин
}

int main() {
    setlocale(LC_ALL, "rus");
    int n;
    cout << "Введите количество пар: ";
    cin >> n;

    cout << "Введите пары в формате ab (например: bc для b < c):" << endl;
    for (int i = 0; i < n; ++i) {
        string pair;
        cin >> pair;
        char u = pair[0];
        char v = pair[1];

        int from = u - 'a'; // переводим символ в индекс 0-25
        int to = v - 'a';

        // Если ребро ещё не добавлено — добавляем
        if (!adj[from][to]) {
            adj[from][to] = true;
            in_degree[to]++; // увеличиваем число входящих рёбер у вершины to
        }

        used[from] = used[to] = true; // отмечаем символы как использованные
    }
    auto start = std::chrono::high_resolution_clock::now();
    // Сначала проверяем, есть ли цикл
    if (has_cycle()) {
        cout << "Последовательность противоречива.\n";
        return 0;
    }

    // Считаем общее количество уникальных символов
    int total_nodes = 0;
    for (int i = 0; i < MAX; ++i)
        if (used[i]) total_nodes++;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Проверка, можно ли выстроить единственную цепочку
    if (is_unique_topo_sort(total_nodes)) {
        cout << "Последовательность полная.\n";
    }
    else {
        cout << "Последовательность не полная, но и не противоречива.\n";
    }
    cout << "Время выполнения: " << duration.count() << " секунд\n";
    return 0;
}
