#include "PressureMeasurement.h"
#include <regex>
#include <stdexcept>

PressureMeasurement::PressureMeasurement(const std::string& date,
    double height,
    int value)
    : m_date(date), m_height(height), m_value(value) {
    validateData(date, height, value);
}

PressureMeasurement::PressureMeasurement(PressureMeasurement&& other) noexcept
    : m_date(std::move(other.m_date)),
    m_height(other.m_height),
    m_value(other.m_value) {
    other.m_height = 0.0;
    other.m_value = 0;
}

PressureMeasurement::PressureMeasurement(const PressureMeasurement& other)
    : m_date(other.m_date),
    m_height(other.m_height),
    m_value(other.m_value) {}

PressureMeasurement& PressureMeasurement::operator=(
    const PressureMeasurement& other) {
    if (this != &other) {
        m_date = other.m_date;
        m_height = other.m_height;
        m_value = other.m_value;
    }
    return *this;
}

PressureMeasurement& PressureMeasurement::operator=(
    PressureMeasurement&& other) noexcept {
    if (this != &other) {
        m_date = std::move(other.m_date);
        m_height = other.m_height;
        m_value = other.m_value;

        other.m_height = 0.0;
        other.m_value = 0;
    }
    return *this;
}

void PressureMeasurement::validateData(const std::string& date,
    double height,
    int value) const {
    // Валидация даты
    std::regex datePattern(R"(^\d{4}\.\d{2}\.\d{2}$)");
    if (!std::regex_match(date, datePattern)) {
        throw std::invalid_argument("Invalid date format. Expected YYYY.MM.DD");
    }

    // Валидация года, месяца, дня
    int year = std::stoi(date.substr(0, 4));
    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (year < 1900 || year > 2100) {
        throw std::invalid_argument("Year must be between 1900 and 2100");
    }
    if (month < 1 || month > 12) {
        throw std::invalid_argument("Month must be between 1 and 12");
    }
    if (day < 1 || day > 31) {
        throw std::invalid_argument("Day must be between 1 and 31");
    }

    // Валидация высоты
    if (height < 0 || height > 10000) {
        throw std::invalid_argument("Height must be between 0 and 10000 meters");
    }

    // Валидация значения давления
    if (value < 0 || value > 2000) {
        throw std::invalid_argument("Pressure value must be between 0 and 2000");
    }
}

bool PressureMeasurement::operator==(const PressureMeasurement& other) const {
    return m_date == other.m_date &&
        m_height == other.m_height &&
        m_value == other.m_value;
}

bool PressureMeasurement::operator!=(const PressureMeasurement& other) const {
    return !(*this == other);
}

std::ostream& operator<<(std::ostream& os,
    const PressureMeasurement& measurement) {
    os << std::fixed << std::setprecision(2)
        << measurement.getDate() << " "
        << measurement.getHeight() << " "
        << measurement.getValue();
    return os;
}