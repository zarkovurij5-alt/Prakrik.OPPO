#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <string>
using namespace std;

const int MAX_NAME_LENGTH = 100;
const int MAX_CITY_LENGTH = 100;
const int MAX_EDUCATION_LENGTH = 22;

enum Education {
    HIGHER,
    SECONDARY,
    SECONDARY_PROFESSIONAL,
    UNKNOWN
};

struct Tourist {
    char surname[MAX_NAME_LENGTH];
    int age;
    char city[MAX_CITY_LENGTH];
    Education education;
};

bool compareBySurname(const Tourist& a, const Tourist& b) {
    return strcmp(a.surname, b.surname) < 0;
}

//for (i = 0; i < n; i++)
//    for (j = 0; j < n; j++)
//        if (strcmp(data[i].surname, data[j].surname) < 0)
//        {
//            //printf("TRUE  %s < %s\n",people[i].name,people[j].name);
//
//            swap = data[i].surname;
//            data[i].surname = data[j].surname;
//            data[j].surname = swap;
//
//            swap = data[i].age;
//            data[i].age = data[j].data;
//            data[j].age = swap;
//
//            swap = data[i].city;
//            data[i].city = data[j].city;
//            data[j].city = swap;
//
//            swap = data[i].education;
//            data[i].education = data[j].education;
//            data[j].education = swap;
//        }



void print_data(const Tourist* data, size_t len, ostream& ost) {
    ost << "Фамилия\tВозраст\tГород\tОбразование\n";
    for (size_t i = 0; i < len; ++i) {
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

Tourist* load_txt(istream& ist, size_t& len) {
    vector<Tourist> tourists;
    Tourist temp;
    string educationStr;
    while (ist >> temp.surname >> temp.age >> temp.city >> educationStr)
    {
        if (educationStr == "Vi") {
            temp.education = HIGHER;
        }
        else if (educationStr == "Cr") {
            temp.education = SECONDARY;
        }
        /*else if (educationStr == "'Cr") {
            temp.education = SECONDARY_PROFESSIONAL;
        }*/
        
        else {
            temp.education = SECONDARY_PROFESSIONAL;
        }
        tourists.push_back(temp);
    }
    len = tourists.size();
    Tourist* data = new Tourist[len];
    std::copy(tourists.begin(), tourists.end(), data);
    //printf(data->city,"!!!");
    return data;
}

void save_bin(const Tourist* data, size_t len, ostream& ost) {
    ost.write(reinterpret_cast<const char*>(&len), sizeof(len));
    ost.write(reinterpret_cast<const char*>(data), sizeof(Tourist) * len);
}

Tourist* load_bin(istream& ist, size_t& len) {
    ist.read(reinterpret_cast<char*>(&len), sizeof(len));
    Tourist* data = new Tourist[len];
    ist.read(reinterpret_cast<char*>(data), sizeof(Tourist) * len);
    return data;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int mode;
    cout << "Выберите режим работы (1 - текстовый файл, 2 - двоичный файл): ";
    cin >> mode;
    string swapi;
    if (mode == 1) {
        ifstream infile("in.txt");
        size_t len;
        Tourist* data = load_txt(infile, len);
       /* for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (strcmp(data[i].surname, data[j].surname) < 0)
                {
                    swap(data[i], data[j]);
                }
            }
        }*/
        sort(data, data + len, compareBySurname);
        print_data(data, len, cout);
        ofstream outfile("tourists.bin", ios::binary);
        save_bin(data, len, outfile);
        delete[] data;
    }
    else if (mode == 2) {
        ifstream infile("tourists.bin", ios::binary);
        size_t len;
        Tourist* data = load_bin(infile, len);
        sort(data, data + len, compareBySurname);
        print_data(data, len, cout);
        delete[] data;
    }
    else {
        cout << "Неверный режим." << endl;
    }

    return 0;
}