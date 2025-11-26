#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>

const int MAX_NAME_LENGTH = 100;
const int MAX_CITY_LENGTH = 100;
const int MAX_EDUCATION_LENGTH = 20;

enum Education {
    HIGHER,
    SECONDARY,
    SECONDARY_PROFESSIONAL
};

struct Tourist {
    char surname[MAX_NAME_LENGTH];
    int age;
    char city[MAX_CITY_LENGTH];
    Education education;
};

void print_data(const Tourist* data, std::size_t len, std::ostream& ost) {
    ost << "Фамилия\tВозраст\tГород\tОбразование\n";
    for (std::size_t i = 0; i < len; ++i) {
        ost << data[i].surname << "\t"
            << data[i].age << "\t"
            << data[i].city << "\t";
        switch (data[i].education) {
        case HIGHER: ost << "высшее"; break;
        case SECONDARY: ost << "среднее"; break;
        case SECONDARY_PROFESSIONAL: ost << "среднее профессиональное"; break;
        default: ost << "неизвестно"; break;
        }
        ost << "\n";
    }
}

Tourist* load_txt(std::istream& ist, std::size_t& len) {
    std::vector<Tourist> tourists;
    Tourist temp;
    while (ist >> temp.surname >> temp.age >> temp.city) {
        std::string educationStr;
        ist >> educationStr;
        if (educationStr == "высшее") {
            temp.education = HIGHER;
        }
        else if (educationStr == "среднее") {
            temp.education = SECONDARY;
        }
        else (educationStr == "среднее профессиональное") {
            temp.education = SECONDARY_PROFESSIONAL;
        }
        tourists.push_back(temp);
    }
    len = tourists.size();
    Tourist* data = new Tourist[len];
    std::copy(tourists.begin(), tourists.end(), data);
    return data;
}

void save_bin(const Tourist* data, std::size_t len, std::ostream& ost) {
    ost.write(reinterpret_cast<const char*>(&len), sizeof(len));
    ost.write(reinterpret_cast<const char*>(data), sizeof(Tourist) * len);
}

Tourist* load_bin(std::istream& ist, std::size_t& len) {
    ist.read(reinterpret_cast<char*>(&len), sizeof(len));
    Tourist* data = new Tourist[len];
    ist.read(reinterpret_cast<char*>(data), sizeof(Tourist) * len);
    return data;
}

int main() {
    int mode;
    std::cout << "Выберите режим работы (1 - текстовый файл, 2 - двоичный файл): ";
    std::cin >> mode;

    if (mode == 1) {
        std::ifstream infile("tourists.txt");
        std::size_t len;
        Tourist* data = load_txt(infile, len);
        print_data(data, len, std::cout);
        std::ofstream outfile("tourists.bin", std::ios::binary);
        save_bin(data, len, outfile);
        delete[] data;
    }
    else if (mode == 2) {
        std::ifstream infile("tourists.bin", std::ios::binary);
        std::size_t len;
        Tourist* data = load_bin(infile, len);
        print_data(data, len, std::cout);
        delete[] data;
    }
    else {
        std::cout << "Неверный режим." << std::endl;
    }

    return 0;
}