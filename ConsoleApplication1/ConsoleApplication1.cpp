#include "PressureMeasurement.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Парсит строку в объект PressureMeasurement
 *
 * @param input Входная строка для парсинга
 * @return PressureMeasurement Объект с данными измерения
 * @throws runtime_error Если не удалось распарсить данные
 */
PressureMeasurement parserPressureMeasurement(const string& input) {
    regex datePattern(R"(\b\d{4}\.\d{2}\.\d{2}\b)");
    regex doublePattern(R"((?:^|\s)(?!(?:\d{4}\.\d{2}\.\d{2}|\d{2}\.\d{2}))(\d+\.\d+)(?=\s|$))");
    regex intPattern(R"(\b\d+\b)");

    smatch matches;
    string date_;
    double height_ = 0.0;
    int value_ = 0;

    if (!regex_search(input, matches, datePattern)) {
        throw runtime_error("No date found");
    }
    date_ = matches[0];

    vector<string> allDoubles;
    auto search = input.cbegin();

    while (regex_search(search, input.cend(), matches, doublePattern)) {
        string found = matches[0];

        if (found != date_) {
            allDoubles.push_back(found);
        }
        search = matches.suffix().first;
    }

    if (allDoubles.empty()) {
        throw runtime_error("No height found");
    }

    height_ = stod(allDoubles[0]);

    vector<string> allInts;
    search = input.cbegin();

    while (regex_search(search, input.cend(), matches, intPattern)) {
        string found = matches[0];
        bool isPartOfDate = false;

        if (date_.find(found) != string::npos) {
            size_t pos = date_.find(found);
            if ((pos == 0 && found.length() == 4) ||
                (pos == 5 && found.length() == 2) ||
                (pos == 8 && found.length() == 2)) {
                isPartOfDate = true;
            }
        }

        bool isPartOfDouble = false;
        for (const auto& dbl : allDoubles) {
            if (dbl.find(found) != string::npos) {
                if (dbl != found) {
                    isPartOfDouble = true;
                    break;
                }
            }
        }

        if (!isPartOfDate && !isPartOfDouble) {
            allInts.push_back(found);
        }

        search = matches.suffix().first;
    }

    if (allInts.empty()) {
        throw runtime_error("No value found");
    }

    value_ = stoi(allInts[0]);

    return PressureMeasurement(date_, height_, value_);
}

/**
 * @brief Читает измерения из файла
 *
 * @param filename Имя файла для чтения
 * @return vector<PressureMeasurement> Вектор измерений
 * @throws runtime_error Если не удалось открыть файл
 */
vector<PressureMeasurement> readMeasurementsFromFile(const string& filename) {
    ifstream file(filename);
    vector<PressureMeasurement> measurements;

    if (!file.is_open()) {
        throw runtime_error("Error opening file: " + filename);
    }

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            try {
                measurements.push_back(parserPressureMeasurement(line));
            }
            catch (const exception& e) {
                cerr << "Warning: Skipping invalid line: " << line
                    << " - " << e.what() << endl;
            }
        }
    }

    file.close();
    return measurements;
}

/**
 * @brief Сортирует измерения по высоте (по убыванию)
 *
 * @param measurements Вектор измерений для сортировки
 * @return vector<PressureMeasurement> Отсортированный вектор
 */
vector<PressureMeasurement> sortByHeight(const vector<PressureMeasurement>& measurements) {
    vector<PressureMeasurement> sorted = measurements;
    sort(sorted.begin(), sorted.end(),
        [](const PressureMeasurement& a, const PressureMeasurement& b) {
            return a.getHeight() > b.getHeight();
        });
    return sorted;
}

/**
 * @brief Выводит топ N измерений по высоте
 *
 * @param measurements Вектор измерений
 * @param n Количество измерений для вывода (по умолчанию 3)
 */
void printTopN(const vector<PressureMeasurement>& measurements, int n = 3) {
    if (measurements.empty()) {
        cout << "No data to display!" << endl;
        return;
    }

    cout << "\nTop-" << n << " by height:" << endl;
    int displayCount = min(n, (int)measurements.size());

    for (int i = 0; i < displayCount; ++i) {
        cout << measurements[i].getDate() << " " << measurements[i].getHeight()
            << " " << measurements[i].getValue() << endl;
    }
}

/**
 * @brief Выводит все измерения
 *
 * @param measurements Вектор измерений
 */
void printAllMeasurements(const vector<PressureMeasurement>& measurements) {
    if (measurements.empty()) {
        cout << "No data to display!" << endl;
        return;
    }

    cout << "\nAll measurements:" << endl;
    for (const auto& item : measurements) {
        cout << item.getDate() << " " << item.getHeight()
            << " " << item.getValue() << endl;
    }
}

/**
 * @brief Обрабатывает выбор пользователя в меню
 *
 * @param measurements Исходные измерения
 * @param sortedMeasurements Отсортированные измерения
 * @return true Если нужно продолжить цикл
 * @return false Если нужно завершить программу
 */
bool processUserChoice(const vector<PressureMeasurement>& measurements,
    const vector<PressureMeasurement>& sortedMeasurements) {
    char choice;
    cout << "\nMenu options:" << endl;
    cout << "1. Show all measurements" << endl;
    cout << "2. Show Top-3 by height" << endl;
    cout << "3. Show Top-N by height" << endl;
    cout << "4. Quit" << endl;
    cout << "\nEnter your choice: ";
    cin >> choice;

    switch (choice) {
    case '1':
        printAllMeasurements(measurements);
        break;

    case '2':
        printTopN(sortedMeasurements);
        break;

    case '3': {
        int n;
        cout << "Enter N: ";
        cin >> n;
        if (n > 0) {
            printTopN(sortedMeasurements, n);
        }
        else {
            cout << "Invalid number!" << endl;
        }
        break;
    }

    case '4':
    case 'q':
    case 'Q':
        return false;

    default:
        cout << "Invalid choice! Please try again." << endl;
        break;
    }

    return true;
}

/**
 * @brief Основной цикл работы программы
 *
 * @param measurements Вектор измерений
 */
void runMainLoop(const vector<PressureMeasurement>& measurements) {
    vector<PressureMeasurement> sortedMeasurements = sortByHeight(measurements);
    bool continueLoop = true;

    while (continueLoop) {
        continueLoop = processUserChoice(measurements, sortedMeasurements);
    }
}

int main() {
    try {
        vector<PressureMeasurement> measurements = readMeasurementsFromFile("in.txt");

        if (measurements.empty()) {
            cout << "No valid data!" << endl;
            return 1;
        }

        cout << "Successfully loaded " << measurements.size() << " measurements" << endl;
        runMainLoop(measurements);
        cout << "\nGoodbye!" << endl;

        return 0;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
}