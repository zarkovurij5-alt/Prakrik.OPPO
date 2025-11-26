#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <regex>

using namespace std;
// Дата высота(Дробное) значение(целое). В рандомном порядке, вывести топ 3 по высоте.
//struct PressureMeasurement {
//    string date;
//    double height = 0.0;
//    int value = 0;
//
//    PressureMeasurement(const string& input) {
//        stringstream ss(input);
//        string token;
//
//        /*date = extractDataFromString(input, "asd");
//        height = stod(extractDataFromString(input, "asd"));
//        value = stoi(extractDataFromString(input, "asd"));*/
//
//        // Обрабатываю каждый токен в строке
//        while (ss >> token) {
//            if (isDate(token)) {
//                date = token;
//            }
//            else if (isDouble(token)) {
//                height = stod(token);
//            }
//            else if (isInteger(token)) {
//                value = stoi(token);
//            }
//        }
//    }
//
//private:
//    //string extractDataFromString(const string& str, const regex& ep) {
//    //}
//
//    bool isDate(const string& token) {
//        if (token.length() != 10) return false;
//        if (token[4] != '.' || token[7] != '.') return false;
//
//        for (int i = 0; i < 10; i++) {
//            if (i == 4 || i == 7) continue;
//            if (!isdigit(token[i])) return false;
//        }
//        return true;
//    }
//
//    bool isDouble(const string& token) {
//        if (token.empty()) return false;
//        bool hasDecimalPoint = false;
//        bool hasDigit = false;
//
//        for (size_t i = 0; i < token.length(); i++) {
//            char c = token[i];
//            if (c == '.') {
//                if (hasDecimalPoint) return false;
//                hasDecimalPoint = true;
//            }
//            else if (isdigit(c)) {
//                hasDigit = true;
//            }
//            else {
//                return false;
//            }
//        }
//        return hasDigit && hasDecimalPoint; // Должна содержать точку для высоты
//    }
//
//    bool isInteger(const string& token) {
//        if (token.empty()) return false;
//        for (char c : token) {
//            if (!isdigit(c)) {
//                return false;
//            }
//        }
//        return true;
//    }
//};

struct PressureMeasurement {
    string date;
    double height = 0.0;
    int value = 0;

    PressureMeasurement(const string& input) {
        // Регулярные выражения для разных типов данных
        regex date_pattern(R"(\d{4}\.\d{2}\.\d{2})");
        regex double_pattern(R"(\b\d+\.\d+\b)");
        regex int_pattern(R"(\b\d+\b)");

        smatch matches;

        // Ищем дату
        if (regex_search(input, matches, date_pattern)) {
            date = matches[0];
        }

        // Ищем дробное число (высоту)
        if (regex_search(input, matches, double_pattern)) {
            height = stod(matches[0]);
        }

        // Ищем целое число (значение) - ищем все, берем последнее
        string::const_iterator search_start = input.cbegin();
        string last_int;
        while (regex_search(search_start, input.cend(), matches, int_pattern)) {
            last_int = matches[0];
            search_start = matches.suffix().first;
        }
        if (!last_int.empty()) {
            value = stoi(last_int);
        }
    }
};


int main() {
    ifstream file("in.txt");
    string line;
    vector<PressureMeasurement> measurements;
    char choise;

    if (!file.is_open()) {
        cout << "Error opening file!" << endl;
        return 1;
    }

    cout << "Original data(sort):" << endl;
    while (getline(file, line)) {
        try {
            PressureMeasurement m(line);
            measurements.push_back(m);
            cout << "Date: " << m.date << " Height: " << m.height << " Value: " << m.value << endl;
        }
        catch (const exception& e) {
            cout << "Error processing: " << line << " - " << e.what() << endl;
        }
    }
    file.close();

    if (measurements.empty()) {
        cout << "No valid data!" << endl;
        return 1;
    }
    cout << "sort for Top-3 by height?(y,n)" << endl;
    cin >> choise;
    if (choise == 'y' || choise == 'Y') {
        // Сортировка по высоте в порядке убывания
        sort(measurements.begin(), measurements.end(),
            [](const PressureMeasurement& a, const PressureMeasurement& b) {
                return a.height > b.height;
            });
        cout << "\nTop-3 by height:" << endl;
        for (int i = 0; i < min(3, (int)measurements.size()); i++) {
            cout << measurements[i].date << " " << measurements[i].height << " " << measurements[i].value << endl;
        }
    }
    else {
        for (int i = 0; i < measurements.size(); i++) {
            cout << measurements[i].date << " " << measurements[i].height << " " << measurements[i].value << endl;
        }
    }
    return 0;
}