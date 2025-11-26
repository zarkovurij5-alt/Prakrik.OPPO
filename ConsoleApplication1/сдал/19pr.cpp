#include <iostream>
#include <vector>
#include <utility> // Для std::pair
#include <algorithm> // Для std::min
#include <climits> // Для INT_MAX

using namespace std;

// Возможные ходы коня
const vector<pair<int, int>> knightMoves = {
    {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
    {-2, -1}, {-1, -2}, {1, -2}, {2, -1}
};

// Проверка на допустимость координат
bool isValid(int x, int y,int **arr) {
    return (x >= 0 && x < 8 && y >= 0 && y < 8 && arr[x][y]==0);
}

// Рекурсивная функция для поиска минимального пути
//int findMinMoves(int** arr,int x1, int y1, int x2, int y2) {
//    if (x1 ==x1 && y1 == y2) {
//        return 0; // Достигли цели
//    } // Помечаем текущую позицию как посещенную
//    int minMoves = INT_MAX; // Инициализируем минимальное количество ходов
//    arr[x1][y1] = 1;
//    for (const auto& move : knightMoves) {
//        int nextX = x1 + move.first;
//        int nextY = y1 + move.second;
//
//        if (isValid(nextX, nextY, arr)) {
//            findMinMoves(arr, nextX, nextY, x2, y2);
//
//        }
//    }
//}

void findMinMoves(int** arr, int x1, int y1, int x2, int y2, int movesCount, int& minMoves,
    vector<pair<int, int>>& currentPath, vector<pair<int, int>>& bestPath) {
    // Если текущий путь уже не лучше найденного
    if (movesCount >= minMoves) {
        return;
    }

    // Добавляем текущую позицию в текущий путь
    currentPath.push_back({ x1, y1 });

    // Если достигли цели
    if (x1 == x2 && y1 == y2) {
        if (movesCount < minMoves) {
            minMoves = movesCount;
            bestPath = currentPath; // сохраняем лучший путь
        }
        currentPath.pop_back(); // убираем из текущего пути перед возвратом
        return;
    }

    // Помечаем текущую позицию как посещенную
    arr[x1][y1] = 1;

    for (const auto& move : knightMoves) {
        int nextX = x1 + move.first;
        int nextY = y1 + move.second;

        if (isValid(nextX, nextY, arr)) {
            findMinMoves(arr, nextX, nextY, x2, y2, movesCount + 1, minMoves, currentPath, bestPath);
        }
    }

    // Снимаем отметку о посещении и удаляем из текущего пути
    arr[x1][y1] = 0;
    currentPath.pop_back();
}

     // Снимаем отметку о посещении для backtracking
    //return minMoves; // Возвращаем минимальное количество ходов

int main() {
    string startPosStr, endPosStr;
    int len = 8; int** arr; int n = 8; int x1, y1,x2,y2; char s1, s2;
    arr = new int* [n];
    for (int i = 0; i < n; i++)
        arr[i] = new int[n];

    for (int i = 0; i < 8; i++) {
       for (int j = 0; j < 8; j++) {
                        arr[i][j] = 0;
       }
    }


    cout << "Введите начальную позицию коня (например 'a 1'): ";
    cin >> s1 >> y1; y1 -= 1;
    switch (s1) {
    case 'a': x1 = 0; break;
    case 'b': x1 = 1; break;
    case 'c': x1 = 2; break;
    case 'd': x1 = 3; break;
    case 'e': x1 = 4; break;
    case 'f': x1 = 5; break;
    case 'g': x1 = 6; break;
    case 'h': x1 = 7; break;
    default: cout << "Некорректная позиция!" << endl; return -1;
    }
    cout << "Введите конечную позицию коня (например 'h 8'): ";
    cin >> s2 >> y2; y2 -= 1;
    switch (s2) {
    case 'a': x2 = 0; break;
    case 'b': x2 = 1; break;
    case 'c': x2 = 2; break;
    case 'd': x2 = 3; break;
    case 'e': x2 = 4; break;
    case 'f': x2 = 5; break;
    case 'g': x2 = 6; break;
    case 'h': x2 = 7; break;
    default: cout << "Некорректная позиция!" << endl; return -1;
    }
    // Преобразование шахматной нотации в координаты
    vector<pair<int, int>> currentPath;
    vector<pair<int, int>> bestPath;
    int minMoves = INT_MAX;

    findMinMoves(arr, x1, y1, x2, y2, 0, minMoves, currentPath, bestPath);

    if (minMoves == INT_MAX) {
        cout << "Путь не найден." << endl;
    }
    else {
        cout << "Минимальное количество ходов: " << minMoves << endl;
        cout << "Путь:" << endl;
        for (size_t i = 0; i < bestPath.size(); ++i) {
            char colChar = 'a' + bestPath[i].first; // преобразуем обратно в шахматную нотацию
            int rowNum = bestPath[i].second + 1;
            cout << colChar << " " << rowNum;
            if (i != bestPath.size() - 1)
                cout << " -> ";
            else
                cout << endl;
        }
    }
    cout << "Минимальное количество ходов: " << minMoves << endl;

    return 0;
}