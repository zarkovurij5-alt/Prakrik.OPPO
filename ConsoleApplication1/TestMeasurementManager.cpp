#include "MeasurementManager.h"
#include <iostream>
#include <cassert>

using namespace std;

void testMeasurementManagerCreation() {
    cout << "Testing MeasurementManager creation...\n";

    vector<PressureMeasurement> measurements = {
        PressureMeasurement("2023.10.15", 1500.5, 1013),
        PressureMeasurement("2023.10.16", 1600.0, 1015),
        PressureMeasurement("2023.10.17", 1700.75, 1012)
    };

    MeasurementManager manager(measurements);

    assert(manager.getCount() == 3);
    assert(!manager.isEmpty());

    cout << "  y Creation with vector passed\n";

    // Тест сортировки
    auto sorted = manager.getSortedByHeight();
    assert(sorted.size() == 3);
    assert(sorted[0].getHeight() == 1700.75); // Наибольшая высота
    assert(sorted[2].getHeight() == 1500.5);  // Наименьшая высота

    cout << "  y Sorting works correctly\n";

    // Тест top N
    auto top2 = manager.getTopByHeight(2);
    assert(top2.size() == 2);
    assert(top2[0].getHeight() == 1700.75);
    assert(top2[1].getHeight() == 1600.0);

    cout << "  y Top N works correctly\n";
    cout << "All creation tests passed!\n\n";
}

void testMeasurementManagerOperations() {
    cout << "Testing MeasurementManager operations...\n";

    MeasurementManager manager({});
    assert(manager.isEmpty());

    // Тест добавления
    manager.addMeasurement(PressureMeasurement("2023.10.15", 1500.5, 1013));
    assert(manager.getCount() == 1);

    manager.addMeasurement(PressureMeasurement("2023.10.16", 1600.0, 1015));
    assert(manager.getCount() == 2);

    cout << "  y Adding measurements works\n";

    // Тест фильтрации
    auto filtered = manager.filter([](const PressureMeasurement& m) {
        return m.getHeight() > 1550.0;
        });
    assert(filtered.size() == 1);
    assert(filtered[0].getHeight() == 1600.0);

    cout << "  y Filtering works correctly\n";

    // Тест статистики
    assert(manager.getAverageHeight() == 1550.25);
    assert(manager.getAverageValue() == 1014.0);

    cout << "  y Statistics calculation works\n";

    // Тест удаления
    manager.removeMeasurement(0);
    assert(manager.getCount() == 1);

    cout << "  y Removing measurements works\n";

    // Тест очистки
    manager.clear();
    assert(manager.isEmpty());

    cout << "  y Clearing works\n";
    cout << "All operation tests passed!\n\n";
}

// Переименованная функция main
int runMeasurementManagerTests() {
    cout << "=== Starting MeasurementManager Tests ===\n\n";

    testMeasurementManagerCreation();
    testMeasurementManagerOperations();

    cout << "=== All MeasurementManager Tests Passed ===\n";
    return 0;
}