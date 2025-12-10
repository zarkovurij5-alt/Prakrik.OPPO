#include "MeasurementManager.h"
#include <algorithm>
#include <numeric>
#include <stdexcept>

MeasurementManager::MeasurementManager(
    const std::vector<PressureMeasurement>& measurements)
    : m_measurements(measurements) {
    updateSortedCache();
}

MeasurementManager::MeasurementManager(
    std::vector<PressureMeasurement>&& measurements)
    : m_measurements(std::move(measurements)) {
    updateSortedCache();
}

void MeasurementManager::updateSortedCache() {
    m_sortedByHeight = m_measurements;
    std::sort(m_sortedByHeight.begin(), m_sortedByHeight.end(),
        [](const PressureMeasurement& a, const PressureMeasurement& b) {
            return a.getHeight() > b.getHeight();
        });
}

void MeasurementManager::addMeasurement(const PressureMeasurement& measurement) {
    m_measurements.push_back(measurement);
    updateSortedCache();
}

void MeasurementManager::addMeasurement(PressureMeasurement&& measurement) {
    m_measurements.push_back(std::move(measurement));
    updateSortedCache();
}

void MeasurementManager::removeMeasurement(size_t index) {
    if (index >= m_measurements.size()) {
        throw std::out_of_range("Index out of range");
    }
    m_measurements.erase(m_measurements.begin() + index);
    updateSortedCache();
}

const std::vector<PressureMeasurement>&
MeasurementManager::getAllMeasurements() const {
    return m_measurements;
}

const std::vector<PressureMeasurement>&
MeasurementManager::getSortedByHeight() const {
    return m_sortedByHeight;
}

std::vector<PressureMeasurement>
MeasurementManager::getTopByHeight(size_t n) const {
    if (n == 0 || m_sortedByHeight.empty()) {
        return {};
    }

    size_t count = std::min(n, m_sortedByHeight.size());
    return std::vector<PressureMeasurement>(m_sortedByHeight.begin(),
        m_sortedByHeight.begin() + count);
}

std::vector<PressureMeasurement> MeasurementManager::filter(
    std::function<bool(const PressureMeasurement&)> predicate) const {
    std::vector<PressureMeasurement> result;
    std::copy_if(m_measurements.begin(), m_measurements.end(),
        std::back_inserter(result), predicate);
    return result;
}

double MeasurementManager::getAverageHeight() const {
    if (m_measurements.empty()) {
        return 0.0;
    }

    double sum = std::accumulate(m_measurements.begin(), m_measurements.end(), 0.0,
        [](double acc, const PressureMeasurement& m) {
            return acc + m.getHeight();
        });
    return sum / m_measurements.size();
}

double MeasurementManager::getAverageValue() const {
    if (m_measurements.empty()) {
        return 0.0;
    }

    double sum = std::accumulate(m_measurements.begin(), m_measurements.end(), 0.0,
        [](double acc, const PressureMeasurement& m) {
            return acc + m.getValue();
        });
    return sum / m_measurements.size();
}

size_t MeasurementManager::getCount() const {
    return m_measurements.size();
}

bool MeasurementManager::isEmpty() const {
    return m_measurements.empty();
}

void MeasurementManager::clear() {
    m_measurements.clear();
    m_sortedByHeight.clear();
}