#include "FileReader.h"
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <unistd.h>
#endif

std::vector<std::string> FileReader::readLines(const std::string& filename) {
    if (!fileExists(filename)) {
        throw FileReadException("File does not exist", filename, 1);
    }

    std::ifstream file(filename);
    if (!file.is_open()) {
        throw FileReadException("Failed to open file", filename, 2);
    }

    // Проверка размера файла (упрощенная версия)
    file.seekg(0, std::ios::end);
    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);

    if (fileSize > 100 * 1024 * 1024) {  // 100 MB
        throw FileReadException("File is too large (max 100 MB)", filename, 3);
    }

    std::vector<std::string> lines;
    std::string line;

    while (std::getline(file, line)) {
        // Удаляем лишние пробелы в начале и конце
        size_t start = line.find_first_not_of(" \t\n\r\f\v");
        if (start != std::string::npos) {
            size_t end = line.find_last_not_of(" \t\n\r\f\v");
            line = line.substr(start, end - start + 1);
        }
        else {
            line.clear();  // Вся строка состоит из пробельных символов
        }

        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    if (file.bad()) {
        throw FileReadException("Error while reading file", filename, 4);
    }

    file.close();
    return lines;
}

bool FileReader::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}

size_t FileReader::getFileSize(const std::string& filename) {
    if (!fileExists(filename)) {
        throw FileReadException("File does not exist", filename, 1);
    }

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw FileReadException("Failed to open file for size check", filename, 5);
    }

    size_t size = static_cast<size_t>(file.tellg());
    file.close();
    return size;
}