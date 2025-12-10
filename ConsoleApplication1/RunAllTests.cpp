#include <iostream>

using namespace std;

// Объявления тестовых функций
int runPressureMeasurementTests();
int runDataParserTests();
int runFileReaderTests();
int runMeasurementManagerTests();

int main1() {
    cout << "=== Running All Tests ===\n\n";

    int result = 0;

    // Запускаем все тесты
    result += runPressureMeasurementTests();
    result += runDataParserTests();
    result += runFileReaderTests();
    result += runMeasurementManagerTests();

    cout << "\n=== All Tests Completed ===\n";
    cout << "Total tests run: 4\n";

    if (result == 0) {
        cout << " y All tests passed successfully!\n";
    }
    else {
        cout << " x Some tests failed!\n";
    }

    return result;
}