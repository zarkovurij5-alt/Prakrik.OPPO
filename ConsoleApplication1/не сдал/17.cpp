#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct Node {
    int data;        // Данные узла
    Node* pNext;     // Указатель на следующий узел
};

// Функция для проверки "красоты" числа
int butiful(int n) {
    int count_digits = 0; // Счетчик цифр в числе
    int n1 = n;           // Копия числа для подсчета цифр

    // Подсчет количества цифр в числе
    while (n1 > 0) {
        count_digits++;
        n1 /= 10;
    }

    // Проверка, делится ли число на количество его цифр
    if (n % count_digits == 0)
        return 2; // Число "красивое"

    return 1; // Число не "красивое"
}

// Функция для вставки узла после указанного узла
Node* insertAfter(Node* head, Node* prev, int newData) {
    Node* newNode = new Node;
    newNode->data = newData;
    newNode->pNext = prev->pNext;
    prev->pNext = newNode;

    return head;
}

// Функция для генерации случайного числа
int generateRandom(int base) {
    return base + (rand() % 4); // Генерируем случайное число от base до base + 3
}

// Функция для вывода списка
void printList(Node* head, ostream& ost) {
    for (Node* cur = head; cur != NULL; cur = cur->pNext) {
        ost << cur->data << " ";
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Инициализация генератора случайных чисел

    ifstream in("in.txt");
    ofstream out("out.txt");

    if (!in.is_open() || !out.is_open()) {
        cerr << "Ошибка открытия файла!" << endl;
        return 1;
    }

    Node* start = NULL;
    Node* tail = NULL;
    Node* prev = NULL;

    // Чтение данных из файла и создание списка
    while (true) {
        Node* list = new Node;
        if (!(in >> list->data)) { // Если не удалось прочитать данные, выходим из цикла
            delete list; // Освобождаем память
            break;
        }
        list->pNext = NULL;

        if (start == NULL) { // Если список пустой
            start = list;
            tail = list;
        }
        else { // Добавляем в конец списка insertAfter(start, cur, randomValue);
            tail->pNext = list;
            tail = list;
        }
    }
    // Проверка на наличие "красивого" числа и вставка новых узлов
    for (Node* cur = start; cur != NULL;) { 
    if (butiful(cur->data) == 2) { // Проверяем, является ли число "красивым"
        Node* lastInserted = cur; // Указатель на последний вставленный узел
        for (int i = 0; i < 5; i++) { 
            int randomValue = generateRandom(cur->data);
            insertAfter(start, cur, randomValue); // Вставляем новый узел после текущего.
            lastInserted = lastInserted->pNext; // Обновляем указатель на последний вставленный узел
        }
        break;//cur = lastInserted; // Переход к следующему элементу после всех вставленных.
    } else {
        cur = cur->pNext; // Переход к следующему элементу.
    }
}

    cout << "Список: ";
    printList(start, cout); // Выводим список в консоль

    out << "Список: ";
    printList(start, out);   // Выводим список в файл

    in.close();
    out.close();

    return 0;
}