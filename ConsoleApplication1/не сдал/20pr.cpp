#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm> // для transform
#include <locale>    // для локали

// Класс Item с закрытыми атрибутами
class Item {
private:
    std::string surname;        // Фамилия
    unsigned int age;           // Возраст
    std::string city;           // Город
    std::string education;      // Образование ("высшее", "среднее", "среднее профессиональное")

public:
    Item() : age(0) {}

    // Ввод из текстового файла/потока
    void load_txt(std::istream& ist) {
        surname.clear();
        city.clear();
        education.clear();

        ist >> surname;
        if (!ist) return;

        ist >> age;
        if (!ist) return;

        ist >> city;
        if (!ist) return;

        // Образование может быть в кавычках или без
        std::string token;
        ist >> token;
        if (!ist) return;

        if (token.front() == '"') {
            // Образование в кавычках
            token.erase(0, 1); // Удаляем начальную кавычку

            std::string education_str = token;

            while (true) {
                if (!education_str.empty() && education_str.back() == '"') {
                    education_str.pop_back(); // Удаляем конечную кавычку
                    break;
                }
                if (!(ist >> token)) break;
                education_str += " " + token;
            }
            education = education_str;
        }
        else {
            // Без кавычек
            education = token;
        }
    }

    // Ввод из бинарного файла/потока
    void load_bin(std::istream& ist) {
        size_t len;

        // Фамилия
        size_t slen;
        ist.read(reinterpret_cast<char*>(&slen), sizeof(slen));
        surname.resize(slen);
        ist.read(&surname[0], slen);

        // Возраст
        ist.read(reinterpret_cast<char*>(&age), sizeof(age));

        // Город
        size_t clen;
        ist.read(reinterpret_cast<char*>(&clen), sizeof(clen));
        city.resize(clen);
        ist.read(&city[0], clen);

        // Образование
        size_t elen;
        ist.read(reinterpret_cast<char*>(&elen), sizeof(elen));
        education.resize(elen);
        ist.read(&education[0], elen);
    }

    // Сохранение в бинарный файл/поток
    void save_bin(std::ostream& ost) const {
        size_t len;

        len = surname.size();
        ost.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ost.write(surname.c_str(), len);

        ost.write(reinterpret_cast<const char*>(&age), sizeof(age));

        len = city.size();
        ost.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ost.write(city.c_str(), len);

        len = education.size();
        ost.write(reinterpret_cast<const char*>(&len), sizeof(len));
        ost.write(education.c_str(), len);
    }

    static void print_table_head(std::ostream& ost) {
        ost << "| Фамилия | Возраст | Город | Образование |\n";
        ost << "|---------|---------|--------|--------------|\n";
    }

    void print_table_row(std::ostream& ost) const {
        // Можно добавить форматирование по ширине для красоты.
        ost << "| " << surname << " | " << age << " | " << city << " | " << education << " |\n";
    }
};

// Тип контейнера базы данных — вектор объектов Item
using Database = std::vector<Item>;

// Вывод таблицы всех элементов базы данных на поток вывода.
size_t print_table(const Database& data, std::ostream& ost) {
    Item::print_table_head(ost);
    for (const auto& item : data) {
        item.print_table_row(ost);
    }
    return data.size();
}

// Загружает данные из текстового файла/потока в базу данных.
size_t load_txt(Database& data, std::istream& ist) {
    size_t count = 0;
    while (true) {

        // Перед чтением пропускаем пробелы и новые строки
        ist >> std::ws;

        if (ist.eof()) break;

        Item item;

        item.load_txt(ist);

        if (!ist) break;  // Если чтение не удалось — выходим

        data.push_back(item);

        ++count;

        // После каждой записи ожидается конец строки или пробелы, их можно пропустить при следующем цикле.
        // Можно дополнительно проверить или пропустить строку, если нужно.
        std::string dummy_line;
        if (std::getline(ist, dummy_line)) {
            continue;
        }
    }
    return count;
}

// Загружает данные из бинарного файла/потока в базу данных.
size_t load_bin(Database& data, std::istream& ist) {
    size_t count = 0;

    uint64_t size;
    ist.read(reinterpret_cast<char*>(&size), sizeof(size));
    if (!ist) return count;

    for (uint64_t i = 0; i < size; ++i) {

        Item item;
        item.load_bin(ist);
        data.push_back(item);
        ++count;

        if (!ist) break;
    }
    return count;
}

// Сохраняет всю базу данных в бинарном файле.
size_t save_bin(const Database& data, std::ostream& ost) {
    uint64_t size = data.size();

    /* Записываем размер базы */
    ost.write(reinterpret_cast<const char*>(&size), sizeof(size));

    /* Записываем каждый элемент */
    for (const auto& item : data)
        item.save_bin(ost);

    return data.size();
}

int main() {
    setlocale(LC_ALL, "");

    Database db;

    int mode;

    std::cout << "Выберите режим:\n1 - загрузить из текстового файла\n2 - загрузить из двоичного файла\nВаш выбор: ";
    std::cin >> mode;

    if (mode == 1) {

        std::ifstream fin("input.txt");
        if (!fin) {
            std::cerr << "Не удалось открыть input.txt\n";
            return 1;
        }

        load_txt(db, fin);          // Загружаем из текстового файла

        print_table(db, std::cout);  // Выводим таблицу на экран

        std::ofstream fout("data.bin", std::ios::binary);
        if (!fout) {
            std::cerr << "Не удалось открыть data.bin для записи\n";
            return 1;
        }

        save_bin(db, fout);          // Сохраняем в бинарный файл

    }
    else if (mode == 2) {

        std::ifstream fin("data.bin", std::ios_base::binary);
        if (!fin) {
            std::cerr << "Не удалось открыть data.bin\n";
            return 1;
        }

        load_bin(db, fin);          // Загружаем из бинарного файла

        print_table(db, std::cout);  // Выводим таблицу на экран

    }
    else {
        std::cerr << "Неверный режим.\n";
    }

    return 0;
}