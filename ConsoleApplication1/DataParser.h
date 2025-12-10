#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "PressureMeasurement.h"
#include <string>
#include <regex>
#include <vector>
#include <stdexcept>  // Добавлено для runtime_error

/**
 * @class DataParser
 * @brief Класс для парсинга строк с данными измерений давления
 *
 * Обрабатывает строки формата "YYYY.MM.DD высота значение"
 * с различными вариантами форматирования.
 */
class DataParser {
private:
    std::regex m_datePattern;
    std::regex m_doublePattern;
    std::regex m_intPattern;

    /**
     * @brief Валидирует дату
     * @param date Строка с датой
     * @return true если дата корректна
     */
    bool validateDate(const std::string& date) const;

    /**
     * @brief Валидирует высоту
     * @param height Высота в метрах
     * @return true если высота корректна
     */
    bool validateHeight(double height) const;

    /**
     * @brief Валидирует значение давления
     * @param value Значение давления
     * @return true если значение корректно
     */
    bool validateValue(int value) const;

public:
    /**
     * @brief Конструктор по умолчанию
     */
    DataParser();

    /**
     * @brief Парсит строку в объект PressureMeasurement
     * @param input Входная строка для парсинга
     * @return PressureMeasurement Объект с данными измерения
     * @throws ParseException Если не удалось распарсить данные
     */
    PressureMeasurement parse(const std::string& input) const;

    /**
     * @brief Парсит вектор строк
     * @param lines Вектор строк для парсинга
     * @return Вектор объектов PressureMeasurement
     */
    std::vector<PressureMeasurement> parseLines(
        const std::vector<std::string>& lines) const;

    /**
     * @brief Проверяет, может ли строка быть корректно распарсена
     * @param input Входная строка
     * @return true если строка может быть распарсена
     */
    bool canParse(const std::string& input) const;
};

/**
 * @class ParseException
 * @brief Исключение для ошибок парсинга
 */
class ParseException : public std::runtime_error {
private:
    std::string m_inputLine;
    std::string m_errorType;

public:
    ParseException(const std::string& message,
        const std::string& inputLine = "",
        const std::string& errorType = "")
        : std::runtime_error(message),
        m_inputLine(inputLine),
        m_errorType(errorType) {}

    const std::string& getInputLine() const { return m_inputLine; }
    const std::string& getErrorType() const { return m_errorType; }
};

#endif // DATAPARSER_H