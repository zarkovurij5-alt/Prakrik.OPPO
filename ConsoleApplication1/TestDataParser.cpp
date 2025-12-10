#include "DataParser.h"
#include <iostream>
#include <cassert>

using namespace std;

void testDataParserValidInput() {
    cout << "Testing DataParser with valid input...\n";

    DataParser parser;

    // Тест корректной строки
    string validInput = "2023.10.15 1500.50 1013";
    PressureMeasurement pm = parser.parse(validInput);

    assert(pm.getDate() == "2023.10.15");
    assert(pm.getHeight() == 1500.50);
    assert(pm.getValue() == 1013);

    cout << "  y Valid input parsed correctly\n";

    // Тест с дополнительными пробелами
    string spacedInput = "  2023.10.15   1500.50   1013  ";
    pm = parser.parse(spacedInput);

    assert(pm.getDate() == "2023.10.15");
    assert(pm.getHeight() == 1500.50);
    assert(pm.getValue() == 1013);

    cout << "  y Input with spaces parsed correctly\n";

    // Тест с несколькими строками
    vector<string> lines = {
        "2023.10.15 1500.50 1013",
        "2023.10.16 1600.00 1015",
        "2023.10.17 1700.75 1012"
    };

    auto measurements = parser.parseLines(lines);
    assert(measurements.size() == 3);
    assert(measurements[0].getDate() == "2023.10.15");
    assert(measurements[1].getHeight() == 1600.00);
    assert(measurements[2].getValue() == 1012);

    cout << "  y Multiple lines parsed correctly\n";
    cout << "  y All valid input tests passed!\n\n";
}

void testDataParserInvalidInput() {
    cout << "Testing DataParser with invalid input...\n";

    DataParser parser;

    // Тест строки без даты
    try {
        parser.parse("1500.50 1013");
        assert(false);
    }
    catch (const ParseException& e) {
        cout << "  y No date handled: " << e.what() << "\n";
    }

    // Тест строки без высоты
    try {
        parser.parse("2023.10.15 1013");
        assert(false);
    }
    catch (const ParseException& e) {
        cout << "  y No height handled: " << e.what() << "\n";
    }

    // Тест строки без значения
    try {
        parser.parse("2023.10.15 1500.50");
        assert(false);
    }
    catch (const ParseException& e) {
        cout << "  y No value handled: " << e.what() << "\n";
    }

    // Тест некорректной даты
    try {
        parser.parse("2023.13.15 1500.50 1013");
        assert(false);
    }
    catch (const ParseException& e) {
        cout << "  y Invalid date handled: " << e.what() << "\n";
    }

    // Тест canParse метод
    assert(parser.canParse("2023.10.15 1500.50 1013") == true);
    assert(parser.canParse("invalid input") == false);

    cout << "  y canParse method works correctly\n";
    cout << "All invalid input tests passed!\n\n";
}

// Переименованная функция main
int runDataParserTests() {
    cout << "=== Starting DataParser Tests ===\n\n";

    testDataParserValidInput();
    testDataParserInvalidInput();

    cout << "=== All DataParser Tests Passed ===\n";
    return 0;
}