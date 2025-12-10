#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @class FileReader
 * @brief Класс для чтения данных из файлов
 *
 * Обеспечивает безопасное чтение файлов с обработкой ошибок.
 */
class FileReader {
public:
    /**
     * @brief Читает все строки из файла
     * @param filename Имя файла для чтения
     * @return Вектор строк из файла
     * @throws FileReadException Если не удалось прочитать файл
     */
    static std::vector<std::string> readLines(const std::string& filename);

    /**
     * @brief Проверяет существование файла
     * @param filename Имя файла
     * @return true если файл существует
     */
    static bool fileExists(const std::string& filename);

    /**
     * @brief Получает размер файла
     * @param filename Имя файла
     * @return Размер файла в байтах
     */
    static size_t getFileSize(const std::string& filename);

private:
    FileReader() = delete;  // Статический класс
};

/**
 * @class FileReadException
 * @brief Исключение для ошибок чтения файла
 */
class FileReadException : public std::runtime_error {
private:
    std::string m_filename;
    int m_errorCode;

public:
    FileReadException(const std::string& message,
        const std::string& filename = "",
        int errorCode = 0)
        : std::runtime_error(message),
        m_filename(filename),
        m_errorCode(errorCode) {}

    const std::string& getFilename() const { return m_filename; }
    int getErrorCode() const { return m_errorCode; }
};

#endif // FILEREADER_H