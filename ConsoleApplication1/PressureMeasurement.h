#ifndef PRESSUREMEASUREMENT_H
#define PRESSUREMEASUREMENT_H

#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>  // Добавлено для invalid_argument

/**
 * @class PressureMeasurement
 * @brief Класс для хранения данных об измерении давления
 *
 * Хранит дату, высоту и значение измерения давления.
 * Обеспечивает валидацию данных при создании.
 */
class PressureMeasurement {
private:
    std::string m_date;
    double m_height;
    int m_value;

    /**
     * @brief Валидирует входные данные
     * @param date Дата
     * @param height Высота
     * @param value Значение
     * @throws std::invalid_argument Если данные некорректны
     */
    void validateData(const std::string& date, double height, int value) const;

public:
    /**
     * @brief Конструктор с параметрами
     * @param date Дата измерения (формат YYYY.MM.DD)
     * @param height Высота измерения (метры)
     * @param value Значение давления
     * @throws std::invalid_argument Если данные некорректны
     */
    PressureMeasurement(const std::string& date, double height, int value);

    /**
     * @brief Конструктор перемещения
     * @param other Объект для перемещения
     */
    PressureMeasurement(PressureMeasurement&& other) noexcept;

    /**
     * @brief Конструктор копирования
     * @param other Объект для копирования
     */
    PressureMeasurement(const PressureMeasurement& other);

    /**
     * @brief Оператор присваивания копированием
     * @param other Объект для копирования
     * @return Ссылка на текущий объект
     */
    PressureMeasurement& operator=(const PressureMeasurement& other);

    /**
     * @brief Оператор присваивания перемещением
     * @param other Объект для перемещения
     * @return Ссылка на текущий объект
     */
    PressureMeasurement& operator=(PressureMeasurement&& other) noexcept;

    /**
     * @brief Деструктор
     */
    ~PressureMeasurement() = default;

    /**
     * @brief Получает дату измерения
     * @return Строку с датой
     */
    const std::string& getDate() const { return m_date; }

    /**
     * @brief Получает высоту измерения
     * @return Высоту в метрах
     */
    double getHeight() const { return m_height; }

    /**
     * @brief Получает значение давления
     * @return Значение давления
     */
    int getValue() const { return m_value; }

    /**
     * @brief Сравнивает два измерения по дате
     * @param other Другое измерение
     * @return true если даты совпадают
     */
    bool operator==(const PressureMeasurement& other) const;

    /**
     * @brief Сравнивает два измерения по дате
     * @param other Другое измерение
     * @return true если даты не совпадают
     */
    bool operator!=(const PressureMeasurement& other) const;

    /**
     * @brief Выводит измерение в поток
     * @param os Выходной поток
     * @param measurement Измерение для вывода
     * @return Ссылку на выходной поток
     */
    friend std::ostream& operator<<(std::ostream& os,
        const PressureMeasurement& measurement);
};

#endif // PRESSUREMEASUREMENT_H