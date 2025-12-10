#include "DataParser.h"
#include <sstream>
#include <algorithm>
#include <stdexcept>  // Уже добавлено в заголовочном файле

DataParser::DataParser()
    : m_datePattern(R"(\b\d{4}\.\d{2}\.\d{2}\b)"),
    m_doublePattern(R"((?:^|\s)(?!(?:\d{4}\.\d{2}\.\d{2}|\d{2}\.\d{2}))(\d+\.\d+)(?=\s|$))"),
    m_intPattern(R"(\b\d+\b)") {}

bool DataParser::validateDate(const std::string& date) const {
    if (!std::regex_match(date, m_datePattern)) {
        return false;
    }

    try {
        int year = std::stoi(date.substr(0, 4));
        int month = std::stoi(date.substr(5, 2));
        int day = std::stoi(date.substr(8, 2));

        if (year < 1900 || year > 2100) return false;
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > 31) return false;

        return true;
    }
    catch (...) {
        return false;
    }
}

bool DataParser::validateHeight(double height) const {
    return height >= 0 && height <= 10000;
}

bool DataParser::validateValue(int value) const {
    return value >= 0 && value <= 2000;
}

PressureMeasurement DataParser::parse(const std::string& input) const {
    std::smatch matches;
    std::string date;
    double height = 0.0;
    int value = 0;

    // 1. Поиск даты
    if (!std::regex_search(input, matches, m_datePattern)) {
        throw ParseException("No valid date found in input", input, "DATE_ERROR");
    }
    date = matches[0];

    if (!validateDate(date)) {
        throw ParseException("Invalid date format or value", input, "DATE_VALIDATION_ERROR");
    }

    // 2. Поиск дробных чисел (для высоты)
    std::vector<std::string> allDoubles;
    auto search = input.cbegin();

    while (std::regex_search(search, input.cend(), matches, m_doublePattern)) {
        std::string found = matches[1];  // Используем группу 1
        if (found != date) {
            allDoubles.push_back(found);
        }
        search = matches.suffix().first;
    }

    if (allDoubles.empty()) {
        throw ParseException("No height value found", input, "HEIGHT_ERROR");
    }

    try {
        height = std::stod(allDoubles[0]);
        if (!validateHeight(height)) {
            throw ParseException("Height value out of valid range", input, "HEIGHT_VALIDATION_ERROR");
        }
    }
    catch (const std::exception& e) {
        throw ParseException("Failed to parse height value: " + std::string(e.what()),
            input, "HEIGHT_PARSE_ERROR");
    }

    // 3. Поиск целых чисел (для значения давления)
    std::vector<std::string> allInts;
    search = input.cbegin();

    while (std::regex_search(search, input.cend(), matches, m_intPattern)) {
        std::string found = matches[0];
        bool isPartOfDate = false;

        // Проверка, не является ли частью даты
        if (date.find(found) != std::string::npos) {
            size_t pos = date.find(found);
            if ((pos == 0 && found.length() == 4) ||
                (pos == 5 && found.length() == 2) ||
                (pos == 8 && found.length() == 2)) {
                isPartOfDate = true;
            }
        }

        // Проверка, не является ли частью дробного числа
        bool isPartOfDouble = false;
        for (const auto& dbl : allDoubles) {
            if (dbl.find(found) != std::string::npos && dbl != found) {
                isPartOfDouble = true;
                break;
            }
        }

        if (!isPartOfDate && !isPartOfDouble) {
            allInts.push_back(found);
        }

        search = matches.suffix().first;
    }

    if (allInts.empty()) {
        throw ParseException("No pressure value found", input, "VALUE_ERROR");
    }

    try {
        value = std::stoi(allInts[0]);
        if (!validateValue(value)) {
            throw ParseException("Pressure value out of valid range", input, "VALUE_VALIDATION_ERROR");
        }
    }
    catch (const std::exception& e) {
        throw ParseException("Failed to parse pressure value: " + std::string(e.what()),
            input, "VALUE_PARSE_ERROR");
    }

    try {
        return PressureMeasurement(date, height, value);
    }
    catch (const std::invalid_argument& e) {
        throw ParseException("Failed to create measurement: " + std::string(e.what()),
            input, "MEASUREMENT_CREATION_ERROR");
    }
}

std::vector<PressureMeasurement> DataParser::parseLines(
    const std::vector<std::string>& lines) const {
    std::vector<PressureMeasurement> measurements;
    measurements.reserve(lines.size());

    for (const auto& line : lines) {
        if (!line.empty()) {
            try {
                measurements.push_back(parse(line));
            }
            catch (const ParseException& e) {
                // Логируем ошибку, но продолжаем обработку
                std::cerr << "Warning: Failed to parse line: " << line
                    << " - Error: " << e.what() << std::endl;
            }
        }
    }

    return measurements;
}

bool DataParser::canParse(const std::string& input) const {
    try {
        parse(input);
        return true;
    }
    catch (...) {
        return false;
    }
}