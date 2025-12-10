#include "PressureMeasurement.h"
#include <iostream>
#include <cassert>

using namespace std;

void testPressureMeasurementCreation() {
    cout << "Testing PressureMeasurement creation...\n";

    // Тест корректного создания
    PressureMeasurement pm("2023.10.15", 1500.5, 1013);
    assert(pm.getDate() == "2023.10.15");
    assert(pm.getHeight() == 1500.5);
    assert(pm.getValue() == 1013);

    cout << "  y Correct creation passed\n";

    // Тест на некорректную дату
    try {
        PressureMeasurement pm2("2023.13.15", 1500.5, 1013);
        assert(false); // Не должно дойти до этой строки
    }
    catch (const invalid_argument& e) {
        cout << "  y Invalid date handled: " << e.what() << "\n";
    }

    // Тест на некорректную высоту
    try {
        PressureMeasurement pm3("2023.10.15", -100, 1013);
        assert(false);
    }
    catch (const invalid_argument& e) {
        cout << "  y Invalid height handled: " << e.what() << "\n";
    }

    // Тест на некорректное значение
    try {
        PressureMeasurement pm4("2023.10.15", 1500.5, -10);
        assert(false);
    }
    catch (const invalid_argument& e) {
        cout << "  y Invalid value handled: " << e.what() << "\n";
    }

    cout << "All PressureMeasurement tests passed!\n\n";
}

void testPressureMeasurementComparison() {
    cout << "Testing PressureMeasurement comparison...\n";

    PressureMeasurement pm1("2023.10.15", 1500.5, 1013);
    PressureMeasurement pm2("2023.10.15", 1500.5, 1013);
    PressureMeasurement pm3("2023.10.16", 1600.0, 1015);

    assert(pm1 == pm2);
    assert(pm1 != pm3);

    cout << "  y Comparison operators work correctly\n";
    cout << "All comparison tests passed!\n\n";
}

// Переименованная функция main
int runPressureMeasurementTests() {
    cout << "=== Starting PressureMeasurement Tests ===\n\n";

    testPressureMeasurementCreation();
    testPressureMeasurementComparison();

    cout << "=== All PressureMeasurement Tests Passed ===\n";
    return 0;
}