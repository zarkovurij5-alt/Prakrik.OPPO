#include "PressureMeasurement.h"
#include "DataParser.h"
#include "FileReader.h"
#include "MeasurementManager.h"

#include <iostream>
#include <iomanip>
#include <memory>
#include <limits>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <map>
#include <functional>

using namespace std;

// Функции тестирования (объявления)
int runAllTests();
int runPressureMeasurementTests();
int runDataParserTests();
int runFileReaderTests();
int runMeasurementManagerTests();

/**
 * @class UserInterface
 * @brief Класс для взаимодействия с пользователем
 */
class UserInterface {
private:
    unique_ptr<MeasurementManager> m_manager;
    string m_currentFilename;

    /**
     * @brief Выводит меню
     */
    void printMenu() const {
        cout << "\n=== Pressure Measurements Manager ===\n";
        cout << "Current file: " << m_currentFilename << "\n";
        cout << "Measurements loaded: " << (m_manager ? m_manager->getCount() : 0) << "\n";
        cout << "1. Show all measurements\n";
        cout << "2. Show Top-N by height\n";
        cout << "3. Show statistics\n";
        cout << "4. Filter measurements\n";
        cout << "5. Add measurement manually\n";
        cout << "6. Remove measurement\n";
        cout << "7. Reload from file\n";
        cout << "8. Save to file\n";
        cout << "9. Search measurements\n";
        cout << "0. Exit to main menu\n";
        cout << "Choose option: ";
    }

    /**
     * @brief Загружает измерения из файла
     * @param filename Имя файла
     * @return true если загрузка успешна
     */
    bool loadFromFile(const string& filename) {
        try {
            auto lines = FileReader::readLines(filename);
            DataParser parser;
            auto measurements = parser.parseLines(lines);

            m_manager = make_unique<MeasurementManager>(move(measurements));
            m_currentFilename = filename;

            cout << "Successfully loaded " << m_manager->getCount()
                << " measurements from " << filename << "\n";
            return true;

        }
        catch (const FileReadException& e) {
            cerr << "File read error: " << e.what() << "\n";
            return false;
        }
        catch (const exception& e) {
            cerr << "Error: " << e.what() << "\n";
            return false;
        }
    }

    /**
     * @brief Обрабатывает выбор пользователя
     * @param choice Выбранный пункт меню
     * @return true если нужно продолжить работу
     */
    bool handleChoice(int choice) {
        if (!m_manager && choice != 7 && choice != 0) {
            cout << "No data loaded. Please load a file first (option 7).\n";
            return true;
        }

        switch (choice) {
        case 1:
            showAllMeasurements();
            break;
        case 2:
            showTopN();
            break;
        case 3:
            showStatistics();
            break;
        case 4:
            filterMeasurements();
            break;
        case 5:
            addMeasurement();
            break;
        case 6:
            removeMeasurement();
            break;
        case 7:
            reloadFromFile();
            break;
        case 8:
            saveToFile();
            break;
        case 9:
            searchMeasurements();
            break;
        case 0:
            cout << "Returning to main menu...\n";
            return false;
        default:
            cout << "Invalid choice! Please try again.\n";
            break;
        }
        return true;
    }

    /**
     * @brief Показывает все измерения
     */
    void showAllMeasurements() const {
        if (!m_manager || m_manager->isEmpty()) {
            cout << "No measurements available.\n";
            return;
        }

        cout << "\n=== All Measurements ===\n";
        cout << setw(4) << "#"
            << setw(12) << "Date"
            << setw(12) << "Height (m)"
            << setw(12) << "Value\n";
        cout << string(40, '-') << "\n";

        size_t index = 1;
        for (const auto& m : m_manager->getAllMeasurements()) {
            cout << setw(4) << index++
                << setw(12) << m.getDate()
                << setw(12) << fixed << setprecision(2) << m.getHeight()
                << setw(12) << m.getValue() << "\n";
        }
    }

    /**
     * @brief Показывает топ-N измерений по высоте
     */
    void showTopN() {
        if (!m_manager || m_manager->isEmpty()) {
            cout << "No measurements available.\n";
            return;
        }

        int n;
        cout << "Enter N: ";
        if (!(cin >> n) || n <= 0) {
            cout << "Invalid number!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        auto topN = m_manager->getTopByHeight(n);

        cout << "\n=== Top-" << n << " by Height ===\n";
        cout << setw(4) << "#"
            << setw(12) << "Date"
            << setw(12) << "Height (m)"
            << setw(12) << "Value\n";
        cout << string(40, '-') << "\n";

        size_t index = 1;
        for (const auto& m : topN) {
            cout << setw(4) << index++
                << setw(12) << m.getDate()
                << setw(12) << fixed << setprecision(2) << m.getHeight()
                << setw(12) << m.getValue() << "\n";
        }
    }

    /**
     * @brief Показывает статистику
     */
    void showStatistics() const {
        if (!m_manager || m_manager->isEmpty()) {
            cout << "No measurements available.\n";
            return;
        }

        cout << "\n=== Statistics ===\n";
        cout << "Total measurements: " << m_manager->getCount() << "\n";
        cout << "Average height: " << fixed << setprecision(2)
            << m_manager->getAverageHeight() << " m\n";
        cout << "Average pressure value: " << fixed << setprecision(2)
            << m_manager->getAverageValue() << "\n";

        auto sorted = m_manager->getSortedByHeight();
        if (!sorted.empty()) {
            cout << "\nHighest measurement:\n";
            cout << "  " << sorted.front() << "\n";

            cout << "Lowest measurement:\n";
            cout << "  " << sorted.back() << "\n";
        }
    }

    /**
     * @brief Фильтрует измерения
     */
    void filterMeasurements() const {
        if (!m_manager || m_manager->isEmpty()) {
            cout << "No measurements available.\n";
            return;
        }

        cout << "\n=== Filter Options ===\n";
        cout << "1. By minimum height\n";
        cout << "2. By maximum height\n";
        cout << "3. By year\n";
        cout << "4. By pressure value range\n";
        cout << "Choose option: ";

        int option;
        if (!(cin >> option)) {
            cout << "Invalid option!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        vector<PressureMeasurement> filtered;

        switch (option) {
        case 1: {
            double minHeight;
            cout << "Enter minimum height: ";
            if (cin >> minHeight) {
                filtered = m_manager->filter([minHeight](const PressureMeasurement& m) {
                    return m.getHeight() >= minHeight;
                    });
            }
            break;
        }
        case 2: {
            double maxHeight;
            cout << "Enter maximum height: ";
            if (cin >> maxHeight) {
                filtered = m_manager->filter([maxHeight](const PressureMeasurement& m) {
                    return m.getHeight() <= maxHeight;
                    });
            }
            break;
        }
        case 3: {
            int year;
            cout << "Enter year (YYYY): ";
            if (cin >> year) {
                filtered = m_manager->filter([year](const PressureMeasurement& m) {
                    return m.getDate().substr(0, 4) == to_string(year);
                    });
            }
            break;
        }
        case 4: {
            int minVal, maxVal;
            cout << "Enter minimum pressure value: ";
            if (!(cin >> minVal)) break;
            cout << "Enter maximum pressure value: ";
            if (!(cin >> maxVal)) break;

            filtered = m_manager->filter([minVal, maxVal](const PressureMeasurement& m) {
                return m.getValue() >= minVal && m.getValue() <= maxVal;
                });
            break;
        }
        default:
            cout << "Invalid option!\n";
            return;
        }

        if (filtered.empty()) {
            cout << "No measurements match the criteria.\n";
        }
        else {
            cout << "\nFound " << filtered.size() << " measurements:\n";
            cout << setw(12) << "Date"
                << setw(12) << "Height (m)"
                << setw(12) << "Value\n";
            cout << string(36, '-') << "\n";

            for (const auto& m : filtered) {
                cout << setw(12) << m.getDate()
                    << setw(12) << fixed << setprecision(2) << m.getHeight()
                    << setw(12) << m.getValue() << "\n";
            }
        }
    }

    /**
     * @brief Ищет измерения
     */
    void searchMeasurements() const {
        if (!m_manager || m_manager->isEmpty()) {
            cout << "No measurements available.\n";
            return;
        }

        cout << "\n=== Search Options ===\n";
        cout << "1. Search by date\n";
        cout << "2. Search by exact height\n";
        cout << "3. Search by exact pressure value\n";
        cout << "Choose option: ";

        int option;
        if (!(cin >> option)) {
            cout << "Invalid option!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        vector<PressureMeasurement> results;

        switch (option) {
        case 1: {
            string date;
            cout << "Enter date (YYYY.MM.DD): ";
            getline(cin, date);

            if (!date.empty()) {
                results = m_manager->filter([date](const PressureMeasurement& m) {
                    return m.getDate() == date;
                    });
            }
            break;
        }
        case 2: {
            double height;
            cout << "Enter exact height: ";
            if (cin >> height) {
                results = m_manager->filter([height](const PressureMeasurement& m) {
                    return abs(m.getHeight() - height) < 0.001;
                    });
            }
            break;
        }
        case 3: {
            int value;
            cout << "Enter exact pressure value: ";
            if (cin >> value) {
                results = m_manager->filter([value](const PressureMeasurement& m) {
                    return m.getValue() == value;
                    });
            }
            break;
        }
        default:
            cout << "Invalid option!\n";
            return;
        }

        if (results.empty()) {
            cout << "No measurements found.\n";
        }
        else {
            cout << "\nFound " << results.size() << " measurement(s):\n";
            for (const auto& m : results) {
                cout << "  " << m << "\n";
            }
        }
    }

    /**
     * @brief Добавляет измерение вручную
     */
    void addMeasurement() {
        string date;
        double height;
        int value;

        cout << "\n=== Add New Measurement ===\n";
        cout << "Enter date (YYYY.MM.DD): ";
        cin >> date;

        cout << "Enter height (meters): ";
        if (!(cin >> height)) {
            cout << "Invalid height!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        cout << "Enter pressure value: ";
        if (!(cin >> value)) {
            cout << "Invalid value!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        try {
            PressureMeasurement measurement(date, height, value);
            m_manager->addMeasurement(move(measurement));
            cout << "Measurement added successfully!\n";
        }
        catch (const invalid_argument& e) {
            cout << "Error: " << e.what() << "\n";
        }
    }

    /**
     * @brief Удаляет измерение
     */
    void removeMeasurement() {
        if (!m_manager || m_manager->isEmpty()) {
            cout << "No measurements to remove.\n";
            return;
        }

        // Показываем все измерения с номерами
        cout << "\n=== All Measurements ===\n";
        cout << setw(4) << "#"
            << setw(12) << "Date"
            << setw(12) << "Height (m)"
            << setw(12) << "Value\n";
        cout << string(40, '-') << "\n";

        size_t index = 1;
        for (const auto& m : m_manager->getAllMeasurements()) {
            cout << setw(4) << index++
                << setw(12) << m.getDate()
                << setw(12) << fixed << setprecision(2) << m.getHeight()
                << setw(12) << m.getValue() << "\n";
        }

        cout << "\nEnter measurement number to remove (1-" << m_manager->getCount() << "): ";

        size_t removeIndex;
        if (!(cin >> removeIndex) || removeIndex < 1 || removeIndex > m_manager->getCount()) {
            cout << "Invalid number!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        try {
            // Получаем данные измерения перед удалением
            auto measurements = m_manager->getAllMeasurements();
            auto& toRemove = measurements[removeIndex - 1];

            m_manager->removeMeasurement(removeIndex - 1);

            cout << "Measurement #" << removeIndex << " removed successfully!\n";
            cout << "Removed: " << toRemove.getDate() << " "
                << fixed << setprecision(2) << toRemove.getHeight()
                << " " << toRemove.getValue() << "\n";
        }
        catch (const out_of_range& e) {
            cout << "Error: " << e.what() << "\n";
        }
        catch (const exception& e) {
            cout << "Error removing measurement: " << e.what() << "\n";
        }
    }

    /**
     * @brief Перезагружает данные из файла
     */
    void reloadFromFile() {
        string filename;
        cout << "Enter filename (default: " << m_currentFilename << "): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, filename);

        if (filename.empty()) {
            filename = m_currentFilename.empty() ? "in.txt" : m_currentFilename;
        }

        if (loadFromFile(filename)) {
            cout << "Data reloaded successfully from " << filename << ".\n";
        }
        else {
            cout << "Failed to reload data from " << filename << ".\n";
        }
    }

    /**
     * @brief Сохраняет данные в файл
     */
    void saveToFile() {
        if (!m_manager || m_manager->isEmpty()) {
            cout << "No measurements to save.\n";
            return;
        }

        string filename;
        cout << "Enter filename to save (default: " << m_currentFilename << "): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, filename);

        if (filename.empty()) {
            if (m_currentFilename.empty()) {
                filename = "out.txt";
            }
            else {
                // Создаем имя для резервной копии
                string baseName = m_currentFilename;
                size_t dotPos = baseName.find_last_of('.');
                if (dotPos != string::npos) {
                    baseName = baseName.substr(0, dotPos);
                }
                filename = baseName + "_backup.txt";
            }
        }

        try {
            ofstream file(filename);
            if (!file.is_open()) {
                cout << "Failed to open file " << filename << " for writing.\n";
                return;
            }

            // Сохраняем все измерения
            for (const auto& m : m_manager->getAllMeasurements()) {
                file << m.getDate() << " "
                    << fixed << setprecision(2) << m.getHeight() << " "
                    << m.getValue() << "\n";
            }

            file.close();

            if (file.fail()) {
                cout << "Error occurred while saving to " << filename << "\n";
            }
            else {
                cout << "Successfully saved " << m_manager->getCount()
                    << " measurements to " << filename << "\n";
                m_currentFilename = filename; // Обновляем текущее имя файла
            }

        }
        catch (const exception& e) {
            cout << "Error saving file: " << e.what() << "\n";
        }
    }

public:
    /**
     * @brief Запускает пользовательский интерфейс
     */
    void run() {
        // Пытаемся загрузить данные из файла по умолчанию
        if (FileReader::fileExists("in.txt")) {
            cout << "Found default file 'in.txt'. Loading...\n";
            loadFromFile("in.txt");
        }
        else {
            cout << "Default file 'in.txt' not found.\n";
            cout << "Please load a data file (option 7).\n";
        }

        bool running = true;
        while (running) {
            printMenu();

            int choice;
            if (!(cin >> choice)) {
                cout << "Invalid input!\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }

            running = handleChoice(choice);
        }
    }
};

/**
 * @brief Главное меню программы
 */
void showMainMenu() {
    cout << "========================================\n";
    cout << "   PRESSURE MEASUREMENTS MANAGER\n";
    cout << "========================================\n";
    cout << "1. Run main program\n";
    cout << "2. Run all tests\n";
    cout << "3. Run PressureMeasurement tests\n";
    cout << "4. Run DataParser tests\n";
    cout << "5. Run FileReader tests\n";
    cout << "6. Run MeasurementManager tests\n";
    cout << "0. Exit\n";
    cout << "========================================\n";
    cout << "Choose option: ";
}

/**
 * @brief Обрабатывает выбор в главном меню
 * @param choice Выбранный пункт
 * @return true если нужно продолжить работу
 */
bool handleMainMenuChoice(int choice) {
    switch (choice) {
    case 1: {
        UserInterface ui;
        ui.run();
        break;
    }
    case 2:
        runAllTests();
        break;
    case 3:
        runPressureMeasurementTests();
        break;
    case 4:
        runDataParserTests();
        break;
    case 5:
        runFileReaderTests();
        break;
    case 6:
        runMeasurementManagerTests();
        break;
    case 0:
        cout << "Goodbye!\n";
        return false;
    default:
        cout << "Invalid choice! Please try again.\n";
        break;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();

    return true;
}

/**
 * @brief Основная функция программы
 */
int main() {
    struct MenuItem
    {
        std::string message;
        std::function<void(void)> callback;
    };

    static  std::map<int, MenuItem> Menu = {
        {1, {"Run main program", []() {  UserInterface ui; ui.run(); }}},
        {2, {"Run all tests", runAllTests}},
    };





    //try {
        bool running = true;

        while (running) {
            system("cls");  // Очистка экрана (Windows)
            // system("clear"); // Для Linux/Mac

            //showMainMenu();

            cout << "========================================\n";
            cout << "   PRESSURE MEASUREMENTS MANAGER\n";
            cout << "========================================\n";

            for (auto [id, item] : Menu) {
                cout << id << " - " << item.message << std::endl;
            }

            cout << "========================================\n";
            cout << "Choose option: ";


            int choice;
            cin >> choice;

            try {
                Menu.at(choice).callback();
            }
            catch (...) {
               
                //return 1;
            }

        //    if (!(cin >> choice)) {
        //        cout << "Invalid input!\n";
        //        cin.clear();
        //        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        //        continue;
        //    }

        //    running = handleMainMenuChoice(choice);
        }

        return 0;
   /* }
    catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }*/
}

// Реализации тестовых функций (можно вынести в отдельные файлы)
int runAllTests() {
    cout << "\n=== Running All Tests ===\n\n";

    int result = 0;

    // Запускаем все тесты
    result += runPressureMeasurementTests();
    result += runDataParserTests();
    result += runFileReaderTests();
    result += runMeasurementManagerTests();

    cout << "\n=== All Tests Completed ===\n";
    cout << "Total tests run: 4\n";

    if (result == 0) {
        cout << " All tests passed successfully!\n";
    }
    else {
        cout << " Some tests failed!\n";
    }

    return result;
}