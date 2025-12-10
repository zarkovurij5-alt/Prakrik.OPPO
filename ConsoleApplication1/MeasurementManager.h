#pragma once
#ifndef MEASUREMENTMANAGER_H
#define MEASUREMENTMANAGER_H

#include "PressureMeasurement.h"
#include <vector>
#include <memory>
#include <functional>

/**
 * @class MeasurementManager
 * @brief Класс для управления коллекцией измерений
 *
 * Предоставляет функционал для работы с коллекцией измерений:
 * сортировка, фильтрация, статистика.
 */
class MeasurementManager {
private:
    std::vector<PressureMeasurement> m_measurements;
    std::vector<PressureMeasurement> m_sortedByHeight;

    /**
     * @brief Обновляет отсортированную копию
     */
    void updateSortedCache();

public:
    /**
     * @brief Конструктор с измерениями
     * @param measurements Вектор измерений
     */
    explicit MeasurementManager(
        const std::vector<PressureMeasurement>& measurements);

    /**
     * @brief Конструктор с перемещением измерений
     * @param measurements Вектор измерений
     */
    explicit MeasurementManager(
        std::vector<PressureMeasurement>&& measurements);

    /**
     * @brief Добавляет измерение
     * @param measurement Измерение для добавления
     */
    void addMeasurement(const PressureMeasurement& measurement);

    /**
     * @brief Добавляет измерение с перемещением
     * @param measurement Измерение для добавления
     */
    void addMeasurement(PressureMeasurement&& measurement);

    /**
     * @brief Удаляет измерение по индексу
     * @param index Индекс измерения
     * @throws std::out_of_range Если индекс неверный
     */
    void removeMeasurement(size_t index);

    /**
     * @brief Получает все измерения
     * @return Константную ссылку на вектор измерений
     */
    const std::vector<PressureMeasurement>& getAllMeasurements() const;

    /**
     * @brief Получает измерения, отсортированные по высоте (убывание)
     * @return Вектор измерений, отсортированных по высоте
     */
    const std::vector<PressureMeasurement>& getSortedByHeight() const;

    /**
     * @brief Получает топ N измерений по высоте
     * @param n Количество измерений
     * @return Вектор топ N измерений
     */
    std::vector<PressureMeasurement> getTopByHeight(size_t n) const;

    /**
     * @brief Фильтрует измерения по условию
     * @param predicate Условие фильтрации
     * @return Вектор отфильтрованных измерений
     */
    std::vector<PressureMeasurement> filter(
        std::function<bool(const PressureMeasurement&)> predicate) const;

    /**
     * @brief Вычисляет среднюю высоту
     * @return Средняя высота
     */
    double getAverageHeight() const;

    /**
     * @brief Вычисляет среднее значение давления
     * @return Среднее значение давления
     */
    double getAverageValue() const;

    /**
     * @brief Получает количество измерений
     * @return Количество измерений
     */
    size_t getCount() const;

    /**
     * @brief Проверяет, пуста ли коллекция
     * @return true если коллекция пуста
     */
    bool isEmpty() const;

    /**
     * @brief Очищает коллекцию
     */
    void clear();
};

#endif // MEASUREMENTMANAGER_H