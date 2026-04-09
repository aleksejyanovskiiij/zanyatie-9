// zanyatie 9 variant 4
#include <iostream>
#include <cstring>
#include <algorithm>
#include <cassert>
#include <fstream>

enum flight_type {
    CHARTER,
    TRANSIT,
    COUPLING,
};

struct flight {
    char destination[50];
    char departure_time[6];
    char arrival_time[6];
    int flight_duration;
    int ticket_price;
    flight_type type;
};

const int FLIGHTS_COUNT = 20;

void save_to_file(flight* arr, int size, const char* filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Ошибка открытия файла для записи" << '\n';
        return;
    }
    file.write(reinterpret_cast<char*>(arr), size*sizeof(flight));
    file.close();
}

void load_from_file(flight* arr, const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Файл не найден, создан новый массив" << '\n';
        return;
    }
    file.read(reinterpret_cast<char*>(arr), FLIGHTS_COUNT*sizeof(flight));
    file.close();
}

void filter_flights(flight* source, int source_size, flight* dest, int& dest_size) {
    dest_size = 0;
    for (int i = 0; i < source_size; i++) {
        int len = strlen(source[i].destination);
        if (
            len >= 4 && 
            strcmp(source[i].destination + len - 4, "град") == 0 && 
            source[i].type == TRANSIT
        ) {
            dest[dest_size] = source[i];
            dest_size++;
        }
    }
}

void sort_by_duration(flight* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j].flight_duration > arr[j + 1].flight_duration) {
                flight temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void print_flight(const flight& f) {
    std::cout << "пункт посадки: " << f.destination << '\n';
    std::cout << "время отправления: " << f.departure_time << '\n';
    std::cout << "время прибытия: " << f.arrival_time << '\n';
    std::cout << "время полета: " << f.flight_duration << " мин" << '\n';
    std::cout << "стоимость билета: " << f.ticket_price << " руб" << '\n';
    std::cout << "тип рейса: ";
    switch (f.type) {
        case CHARTER:  std::cout << "чартер"; break;
        case TRANSIT:  std::cout << "транзит"; break;
        case COUPLING: std::cout << "стыковка"; break;
    }
    std::cout << '\n'; 
}

void print_flight_by_destination(flight* arr, int len, const char* dest) {
    for (int i = 0; i < len; i++) {
        if (strcmp(arr[i].destination, dest) == 0) {
            print_flight(arr[i]);
            return;
        }
    }
    std::cout << "рейс не найден" << '\n';
}

void print_night_charter(flight* arr, int len, int count_to_print) {
    int count = 0;
    for (int i = 0; i < len && count < count_to_print; i++) {
        if (arr[i].type == CHARTER) {
            int hour = (arr[i].departure_time[0] - '0') * 10 + (arr[i].departure_time[1] - '0');
            if (hour >= 22 || hour < 6) {
                std::cout << "чартерный рейс " << count + 1 << ":" << '\n';
                print_flight(arr[i]);
                count++;
            }
        }
    }
    if (count == 0) std::cout << "ночных чартерных рейсов не найдено" << '\n';
}

void update_flight(flight* arr, int len, const char* dest) {
    for (int i = 0; i < len; i++) {
        if (strcmp(arr[i].destination, dest) == 0) {
            std::cout << "введите новые данные для рейса " << dest << ":" << '\n';
            std::cout << "пункт посадки: ";
            std::cin >> arr[i].destination;
            std::cout << "время отправления (часы:минуты): ";
            std::cin >> arr[i].departure_time;
            std::cout << "время прибытия (часы:минуты): ";
            std::cin >> arr[i].arrival_time;
            std::cout << "время полета (мин): ";
            std::cin >> arr[i].flight_duration;
            std::cout << "стоимость билета: ";
            std::cin >> arr[i].ticket_price;
            int type_choice;
            std::cout << "тип рейса (0 == чартер, 1 == транзит, 2 == стыковка): ";
            std::cin >> type_choice;
            arr[i].type = (flight_type)(type_choice);
            std::cout << "данные обновлены" << '\n';
            return;
        }
    }
    std::cout << "рейс не найден" << '\n';
}

void filter_connection_n(flight* src, int src_len, flight* dest, int& dest_len, int n) {
    dest_len = 0;
    for (int i = 0; i < src_len; i++) {
        if (src[i].type == COUPLING && src[i].ticket_price < n) {
            dest[dest_len] = src[i];
            dest_len++;
        }
    }
}

void wrapper_print(flight* arr, int len, const char* filter_name) {
    std::cout << "отфильтрованные данные: " << filter_name << '\n';
    if (len == 0) {
        std::cout << "нет данных" << '\n';
        return;
    }
    for (int i = 0; i < len; i++) {
        print_flight(arr[i]);
    }
}

int main() {
    const int CHARTERS_TO_PRINT = 5;
    flight flights[FLIGHTS_COUNT];
    load_from_file(flights, "flights.txt");

    std::ifstream test("flights.txt", std::ios::binary);
    if (!test) {
        flight default_flights[20] = {
            {"москваград",    "08:00", "10:30", 150, 5000, TRANSIT},
            {"санктград",     "12:00", "14:00", 120, 4500, CHARTER},
            {"новоград",      "23:30", "01:00", 90, 3000,  CHARTER},
            {"киевград",      "09:00", "11:00", 120, 4000, TRANSIT},
            {"минскград",     "22:00", "23:30", 90, 3500,  CHARTER},
            {"лондонград",    "14:00", "17:00", 180, 8000, COUPLING},
            {"парижград",     "20:00", "23:30", 210, 7500, CHARTER},
            {"берлинград",    "03:00", "05:00", 120, 4200, TRANSIT},
            {"рим",           "10:00", "12:30", 150, 5500, CHARTER},
            {"мадридград",    "01:00", "03:30", 150, 4800, CHARTER},
            {"ослоград",      "16:00", "18:00", 120, 3800, COUPLING},
            {"стокгольмград", "04:00", "06:30", 150, 5200, TRANSIT},
            {"варшавград",    "13:00", "15:00", 120, 4100, CHARTER},
            {"прагаград",     "21:00", "23:00", 120, 4300, COUPLING},
            {"будапештград",  "02:00", "04:00", 120, 3900, TRANSIT},
            {"венаград",      "11:00", "13:30", 150, 4600, CHARTER},
            {"амстердамград", "23:00", "01:30", 150, 6200, CHARTER},
            {"брюссельград",  "05:00", "07:00", 120, 4400, COUPLING},
            {"лиссабонград",  "19:00", "22:00", 180, 5800, TRANSIT},
            {"афины",         "00:30", "02:30", 120, 4900, CHARTER}
        };
        for (int i = 0; i < 20; i++) {
            flights[i] = default_flights[i];
        }
        save_to_file(flights, 20, "flights.txt");
    }
    test.close();

    flight transit_flights[FLIGHTS_COUNT];
    int transit_count;
    filter_flights(flights, FLIGHTS_COUNT, transit_flights, transit_count);

    sort_by_duration(transit_flights, transit_count);

    std::cout << "все транзитные рейсы, оканчивающиеся на град, отсортированные по времени полета: \n";
    for (int i = 0; i < transit_count; i++) {
        print_flight(transit_flights[i]);
    }

    std::cout << "данные по конкретному рейсу (москваград): " << '\n';
    print_flight_by_destination(transit_flights, transit_count, "москваград");

    std::cout << "5 чартерных рейсов, вылетающих ночью: " << '\n';
    print_night_charter(flights, FLIGHTS_COUNT, CHARTERS_TO_PRINT);

    update_flight(flights, FLIGHTS_COUNT, "москваград");
    save_to_file(flights, FLIGHTS_COUNT, "flights.txt");

    flight cheap_connection[FLIGHTS_COUNT];
    int cheap_count;
    filter_connection_n(flights, FLIGHTS_COUNT, cheap_connection, cheap_count, 6000);
    wrapper_print(cheap_connection, cheap_count, "стыковочные рейсы дешевле 6000 баксов");

    return 0;
}
