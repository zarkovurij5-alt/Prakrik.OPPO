#include "FileReader.h"
#include <iostream>
#include <cassert>
#include <fstream>

using namespace std;

void testFileReader() {
    cout << "Testing FileReader...\n";

    // Создаем тестовый файл
    string testFilename = "test_data.txt";
    ofstream testFile(testFilename);
    testFile << "2023.10.15 1500.50 1013\n";
    testFile << "2023.10.16 1600.00 1015\n";
    testFile << "2023.10.17 1700.75 1012\n";
    testFile.close();

    // Тест чтения файла
    auto lines = FileReader::readLines(testFilename);
    assert(lines.size() == 3);
    assert(lines[0] == "2023.10.15 1500.50 1013");
    assert(lines[1] == "2023.10.16 1600.00 1015");
    assert(lines[2] == "2023.10.17 1700.75 1012");

    cout << "  y File reading works correctly\n";

    // Тест проверки существования файла
    assert(FileReader::fileExists(testFilename) == true);
    assert(FileReader::fileExists("nonexistent.txt") == false);

    cout << "  y File existence check works correctly\n";

    // Тест размера файла
    try {
        size_t fileSize = FileReader::getFileSize(testFilename);
        assert(fileSize > 0);
        cout << "  y File size check works correctly\n";
    }
    catch (const exception& e) {
        cout << "  y File size check: " << e.what() << "\n";
    }

    // Тест несуществующего файла
    try {
        FileReader::readLines("nonexistent.txt");
        assert(false);
    }
    catch (const FileReadException& e) {
        cout << "  y Non-existent file handled: " << e.what() << "\n";
    }

    // Удаляем тестовый файл
    remove(testFilename.c_str());

    cout << "  y All FileReader tests passed!\n\n";
}

// Переименованная функция main
int runFileReaderTests() {
    cout << "=== Starting FileReader Tests ===\n\n";

    testFileReader();

    cout << "=== All FileReader Tests Passed ===\n";
    return 0;
}