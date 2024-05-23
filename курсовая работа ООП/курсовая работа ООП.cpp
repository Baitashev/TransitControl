#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>
#include <sstream>
#include <iomanip>
#include <chrono>
sqlite3* db;
class Report {
public:
    int id;
    std::string type;
    std::string data;

    Report(int _id, const std::string& _type, const std::string& _data)
        : id(_id), type(_type), data(_data) {}
};

class Route {
public:
    int id;
    std::string startLocation;
    std::string endLocation;

    Route(int _id, const std::string& _start, const std::string& _end)
        : id(_id), startLocation(_start), endLocation(_end) {}
};

class Tariff {
public:
    int id;
    std::string description;
    double rate;

    Tariff(int _id, const std::string& _desc, double _rate)
        : id(_id), description(_desc), rate(_rate) {}
};

class Client {
public:
    int id;
    std::string name;
    std::string contactInfo;

    Client(int _id, const std::string& _name, const std::string& _contact)
        : id(_id), name(_name), contactInfo(_contact) {}
};

class Order {
public:
    int id;
    int clientId;
    int expeditorId;
    int freightId;
    std::string status;

    Order(int _id, int _clientId, int _expId, int _frId, const std::string& _status)
        : id(_id), clientId(_clientId), expeditorId(_expId), freightId(_frId), status(_status) {}
};

class Expeditor {
public:
    int id;
    std::string firstName;
    std::string lastName;
    std::string birthDate;
    std::string address;
    std::string contactPhone;
    std::string licenseNumber;
    std::string issueDate;
    std::string expiryDate;
    int drivingExperience;

    Expeditor(int _id, const std::string& _firstName, const std::string& _lastName, const std::string& _birthDate, const std::string& _address, const std::string& _contactPhone, const std::string& _licenseNumber, const std::string& _issueDate, const std::string& _expiryDate)
        : id(_id), firstName(_firstName), lastName(_lastName), birthDate(_birthDate), address(_address), contactPhone(_contactPhone), licenseNumber(_licenseNumber), issueDate(_issueDate), expiryDate(_expiryDate) {
        //drivingExperience = calculateDrivingExperience(_issueDate);
    }

//private:
//    int calculateDrivingExperience(const std::string& issueDate) {
//        std::istringstream iss(issueDate);
//        std::tm issueTm = {};
//        iss >> std::get_time(&issueTm, "%Y-%m-%d");
//        std::time_t issueTime = std::mktime(&issueTm);
//
//        std::time_t now = std::time(nullptr);
//        double seconds = std::difftime(now, issueTime);
//        int years = static_cast<int>(seconds / (60 * 60 * 24 * 365.25));
//        return years;
//    }
};

class Freight {
public:
    int id;
    std::string description;
    double weight;
    std::string destination;
    std::string status;
    double cost;

    Freight(int _id, const std::string& _desc, double _weight, const std::string& _dest, const std::string& _status, double _cost)
        : id(_id), description(_desc), weight(_weight), destination(_dest), status(_status), cost(_cost) {}
};

class Vehicle {
public:
    int id;
    std::string make;
    std::string model;
    int year;
    double capacity;

    Vehicle(int _id, const std::string& _make, const std::string& _model, int _year, double _capacity)
        : id(_id), make(_make), model(_model), year(_year), capacity(_capacity) {}
};

class WaterTransport : public Vehicle {
public:
    std::string waterType;

    WaterTransport(int _id, const std::string& _make, const std::string& _model, int _year, double _capacity, const std::string& _type)
        : Vehicle(_id, _make, _model, _year, _capacity), waterType(_type) {}
};

class PassengerCar : public Vehicle {
public:
    int numberOfSeats;

    PassengerCar(int _id, const std::string& _make, const std::string& _model, int _year, double _capacity, int _seats)
        : Vehicle(_id, _make, _model, _year, _capacity), numberOfSeats(_seats) {}
};

class Truck : public Vehicle {
public:
    double cargoCapacity;

    Truck(int _id, const std::string& _make, const std::string& _model, int _year, double _capacity, double _cargo)
        : Vehicle(_id, _make, _model, _year, _capacity), cargoCapacity(_cargo) {}
};

class AirTransport : public Vehicle {
public:
    double wingspan;

    AirTransport(int _id, const std::string& _make, const std::string& _model, int _year, double _capacity, double _wingspan)
        : Vehicle(_id, _make, _model, _year, _capacity), wingspan(_wingspan) {}
};

class Dispatcher {
public:
    void deleteExpeditor() {
        int id;

        // Запрос пользователю ввести идентификатор экспедитора
        std::cout << "Enter Expeditor ID to delete: ";
        std::cin >> id;

        // Подготовка SQL-запроса
        std::string query = "DELETE FROM Expeditor WHERE id = ?;";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Привязка параметра
        sqlite3_bind_int(stmt, 1, id);

        // Выполнение запроса
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "Expeditor deleted successfully." << std::endl;
        }

        // Завершение работы с запросом
        sqlite3_finalize(stmt);
    }

    void viewRegisteredOrders()
    {
        
            setlocale(LC_ALL, "rus");

            // SQL запрос для выборки всех заказов
            std::string query = "SELECT id, clientId, expeditorId, freightId, status, start_date, end_date FROM OrderTable;";

            // Подготовка SQL запроса
            sqlite3_stmt* stmt;
            int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
            if (rc != SQLITE_OK) {
                std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
                return;
            }

            // Вывод результатов
            std::cout << "Orders:" << std::endl;
            while (sqlite3_step(stmt) == SQLITE_ROW) {
                int id = sqlite3_column_int(stmt, 0);
                int clientId = sqlite3_column_int(stmt, 1);
                int expeditorId = sqlite3_column_int(stmt, 2);
                int freightId = sqlite3_column_int(stmt, 3);

                const unsigned char* status = sqlite3_column_text(stmt, 4);
                const unsigned char* startDate = sqlite3_column_text(stmt, 5);
                const unsigned char* endDate = sqlite3_column_text(stmt, 6);

                std::cout << "ID: " << id << std::endl;
                std::cout << "Client ID: " << clientId << std::endl;
                std::cout << "Expeditor ID: " << expeditorId << std::endl;
                std::cout << "Freight ID: " << freightId << std::endl;

                std::cout << "Status: " << (status ? reinterpret_cast<const char*>(status) : "NULL") << std::endl;
                std::cout << "Start Date: " << (startDate ? reinterpret_cast<const char*>(startDate) : "NULL") << std::endl;
                std::cout << "End Date: " << (endDate ? reinterpret_cast<const char*>(endDate) : "NULL") << std::endl;

                std::cout << std::endl;
            }

            // Завершение работы с запросом
            sqlite3_finalize(stmt);
        

    }

    void viewRegisteredDrivers() {
        setlocale(LC_ALL, "rus");
        // Ваш код для просмотра зарегистрированных водителей
        std::string query = "SELECT id, firstName, lastName, birthDate, licenseNumber, vehicleId, address, contactPhone, issueDate, expiryDate, drivingExperience FROM Expeditor;";

        // Execute the query
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Fetch and display results
        std::cout << "Registered Drivers:" << std::endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* firstName = sqlite3_column_text(stmt, 1);
            const unsigned char* lastName = sqlite3_column_text(stmt, 2);
            const unsigned char* birthDate = sqlite3_column_text(stmt, 3);
            const unsigned char* licenseNumber = sqlite3_column_text(stmt, 4);
            int vehicleId = sqlite3_column_int(stmt, 5);
            const unsigned char* address = sqlite3_column_text(stmt, 6);
            const unsigned char* contactPhone = sqlite3_column_text(stmt, 7);
            const unsigned char* issueDate = sqlite3_column_text(stmt, 8);
            const unsigned char* expiryDate = sqlite3_column_text(stmt, 9);
            int drivingExperience = sqlite3_column_int(stmt, 10);

            std::cout << "ID: " << id << std::endl;
            std::cout << "First Name: " << firstName << std::endl;
            std::cout << "Last Name: " << lastName << std::endl;
            std::cout << "Birth Date: " << birthDate << std::endl;
            std::cout << "License Number: " << licenseNumber << std::endl;
            std::cout << "Vehicle ID: " << vehicleId << std::endl;
            std::cout << "Address: " << address << std::endl;
            std::cout << "Contact Phone: " << contactPhone << std::endl;
            std::cout << "Issue Date: " << issueDate << std::endl;
            std::cout << "Expiry Date: " << expiryDate << std::endl;
            std::cout << "Driving Experience: " << drivingExperience << std::endl;
            std::cout << std::endl;
        }

        // Finalize the statement
        sqlite3_finalize(stmt);
    }

    void viewRegisteredVehicles() {
        // Ваш код для просмотра зарегистрированных автомобилей
        setlocale(LC_ALL, "rus");
        // Ваш код для просмотра зарегистрированных транспортных средств
        std::string query = "SELECT id, make, model, year, capacity FROM vehicle;";

        // Execute the query
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Fetch and display results
        std::cout << "Registered Vehicles:" << std::endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* make = sqlite3_column_text(stmt, 1);
            const unsigned char* model = sqlite3_column_text(stmt, 2);
            int year = sqlite3_column_int(stmt, 3);
            double capacity = sqlite3_column_double(stmt, 4);

            std::cout << "ID: " << id << std::endl;
            std::cout << "Make: " << make << std::endl;
            std::cout << "Model: " << model << std::endl;
            std::cout << "Year: " << year << std::endl;
            std::cout << "Capacity: " << capacity << std::endl;
            std::cout << std::endl;
        }

        // Finalize the statement
        sqlite3_finalize(stmt);

    }
    void viewRegisteredClients()
    {
        setlocale(LC_ALL, "rus");
        // Ваш код для просмотра зарегистрированных клиентов
        std::string query = "SELECT id, name, contactInfo FROM Client;";

        // Execute the query
        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to execute query: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Fetch and display results
        std::cout << "Registered Clients:" << std::endl;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* name = sqlite3_column_text(stmt, 1);
            const unsigned char* contactInfo = sqlite3_column_text(stmt, 2);

            std::cout << "ID: " << id << std::endl;
            std::cout << "Name: " << name << std::endl;
            std::cout << "Contact Info: " << contactInfo << std::endl;
            std::cout << std::endl;
        }

        // Finalize the statement
        sqlite3_finalize(stmt);

    }
    void registerExpeditor() {
        int vehicleId, issueDate, expiryDate, drivingExperience;
        std::string firstName, lastName, birthDate, address, licenseNumber, contactPhone;

        // Запрос пользователю ввести данные о водителе
        std::cout << "Enter First Name: ";
        std::cin >> firstName;
        std::cout << "Enter Last Name: ";
        std::cin >> lastName;
        std::cout << "Enter Birth Date (YYYY-MM-DD): ";
        std::cin >> birthDate;
        std::cout << "Enter License Number: ";
        std::cin >> licenseNumber;
        std::cout << "Enter Vehicle ID: ";
        std::cin >> vehicleId;
        std::cout << "Enter Address: ";
        std::cin.ignore(); // Очистка буфера ввода перед getline
        std::getline(std::cin, address);
        std::cout << "Enter Contact Phone: ";
        std::cin >> contactPhone;
        std::cout << "Enter Issue Date (YYYY-MM-DD): ";
        std::cin >> issueDate;
        std::cout << "Enter Expiry Date (YYYY-MM-DD): ";
        std::cin >> expiryDate;
        std::cout << "Enter Driving Experience: ";
        std::cin >> drivingExperience;

        // Подготовка SQL-запроса
        std::string query = "INSERT INTO Expeditor (firstName, lastName, birthDate, licenseNumber, vehicleId, address, contactPhone, issueDate, expiryDate, drivingExperience) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Привязка параметров
        sqlite3_bind_text(stmt, 1, firstName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, lastName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, birthDate.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 4, licenseNumber.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 5, vehicleId);
        sqlite3_bind_text(stmt, 6, address.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 7, contactPhone.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 8, issueDate);
        sqlite3_bind_int(stmt, 9, expiryDate);
        sqlite3_bind_int(stmt, 10, drivingExperience);

        // Выполнение запроса
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        }

        // Завершение работы с запросом
        sqlite3_finalize(stmt);
    }

    
    
    void viewReport() {
        // Ваш код для просмотра отчета
    }
   
    void registerOrder() {
        int clientId, expeditorId, freightId;
        std::string startDate, endDate;

        // Запрос пользователю ввести данные о заказе
        std::cout << "Enter Client ID: ";
        std::cin >> clientId;
        std::cout << "Enter Expeditor ID: ";
        std::cin >> expeditorId;
        std::cout << "Enter Freight ID: ";
        std::cin >> freightId;
        /*std::cout << "Enter Status: ";
        std::cin >> status;*/
        std::cout << "Enter Start Date (YYYY-MM-DD): ";
        std::cin >> startDate;
        std::cout << "Enter End Date (YYYY-MM-DD): ";
        std::cin >> endDate;

        // Подготовка SQL-запроса
        std::string query = "INSERT INTO OrderTable (clientId, expeditorId, freightId, status, start_date, end_date) VALUES (?, ?, ?, ?, ?, ?);";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Привязка параметров
        sqlite3_bind_int(stmt, 1, clientId);
        sqlite3_bind_int(stmt, 2, expeditorId);
        sqlite3_bind_int(stmt, 3, freightId);
        //sqlite3_bind_text(stmt, 4, status.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 5, startDate.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 6, endDate.c_str(), -1, SQLITE_TRANSIENT);

        // Выполнение запроса
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        }

        // Завершение работы с запросом
        sqlite3_finalize(stmt);
    }
    void registerClient() {
        int id;
        std::string name, contactInfo;

        // Запрос пользователю ввести данные о клиенте
        std::cout << "Enter Client ID: ";
        std::cin >> id;
        std::cin.ignore(); // Игнорируем оставшийся символ новой строки после ввода числа
        std::cout << "Enter Name: ";
        std::getline(std::cin, name); // Используем getline для ввода строки с пробелами
        std::cout << "Enter Contact Info: ";
        std::getline(std::cin, contactInfo);

        // Подготовка SQL-запроса
        std::string query = "INSERT INTO Client (id, name, contactInfo) VALUES (?, ?, ?);";

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Привязка параметров
        sqlite3_bind_int(stmt, 1, id);
        sqlite3_bind_text(stmt, 2, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, contactInfo.c_str(), -1, SQLITE_TRANSIENT);

        // Выполнение запроса
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            std::cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << std::endl;
        }
        else {
            std::cout << "Client registered successfully." << std::endl;
        }

        // Завершение работы с запросом
        sqlite3_finalize(stmt);
    }
};



void executeSQL(const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void createTables() {
    std::vector<std::string> tableCreationQueries = {
        "CREATE TABLE IF NOT EXISTS Report (id INTEGER PRIMARY KEY, type TEXT, data TEXT);",
        "CREATE TABLE IF NOT EXISTS Route (id INTEGER PRIMARY KEY, startLocation TEXT, endLocation TEXT);",
        "CREATE TABLE IF NOT EXISTS Tariff (id INTEGER PRIMARY KEY, description TEXT, rate REAL);",
        "CREATE TABLE IF NOT EXISTS Client (id INTEGER PRIMARY KEY, name TEXT, contactInfo TEXT);",
        "CREATE TABLE IF NOT EXISTS OrderTable (id INTEGER PRIMARY KEY, clientId INTEGER, expeditorId INTEGER, freightId INTEGER, status TEXT);",
        "CREATE TABLE IF NOT EXISTS Expeditor (id INTEGER PRIMARY KEY, firstName TEXT, lastName TEXT, birthDate TEXT, address TEXT, contactPhone TEXT, licenseNumber TEXT, issueDate TEXT, expiryDate TEXT, drivingExperience INTEGER);",
        "CREATE TABLE IF NOT EXISTS Freight (id INTEGER PRIMARY KEY, description TEXT, weight REAL, destination TEXT, status TEXT, cost REAL);",
        "CREATE TABLE IF NOT EXISTS Vehicle (id INTEGER PRIMARY KEY, make TEXT, model TEXT, year INTEGER, capacity REAL);",
        "CREATE TABLE IF NOT EXISTS WaterTransport (id INTEGER PRIMARY KEY, make TEXT, model TEXT, year INTEGER, capacity REAL, waterType TEXT);",
        "CREATE TABLE IF NOT EXISTS PassengerCar (id INTEGER PRIMARY KEY, make TEXT, model TEXT, year INTEGER, capacity REAL, numberOfSeats INTEGER);",
        "CREATE TABLE IF NOT EXISTS Truck (id INTEGER PRIMARY KEY, make TEXT, model TEXT, year INTEGER, capacity REAL, cargoCapacity REAL);",
        "CREATE TABLE IF NOT EXISTS AirTransport (id INTEGER PRIMARY KEY, make TEXT, model TEXT, year INTEGER, capacity REAL, wingspan REAL);"
    };

    for (const auto& query : tableCreationQueries) {
        executeSQL(query);
    }
}
void showMenu() {
    std::cout << "==== Transport Company Menu ====" << std::endl;
    std::cout << "1. Исполнители" << std::endl;
    std::cout << "2. Автомобили" << std::endl;
    std::cout << "3. Клиенты" << std::endl;
    //std::cout << "4. Generate Waybill" << std::endl;
    std::cout << "5. Зарегистрировать исполнителя" << std::endl;
    std::cout << "6. Заказы" << std::endl;
   /* std::cout << "6. Update Expeditor Info" << std::endl;
    std::cout << "7. View Transportation Info" << std::endl;*/
    std::cout << "8. Удалить исполнителя " << std::endl;
    std::cout << "9. Создать отчет по исполнителю" << std::endl;
    //std::cout << "10. Создать отчет?" << std::endl;
    std::cout << "11. Зарегистрировать заказ" << std::endl;
    std::cout << "12. Зарегистрировать клиента" << std::endl;
    std::cout << "13. Выход" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "Введите число: ";
}

int main() {
    setlocale(LC_ALL, "rus");
    int rc = sqlite3_open("transport_company.db", &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }
    else {
        std::cout << "Opened database successfully" << std::endl;
    }

    // Создаем таблицы в базе данных
    createTables();

    // Создаем объект класса Dispatcher
    Dispatcher dispatcher;

    // Выводим меню и обрабатываем выбор пользователя
    int choice;
    do {
        // Отображаем меню
        showMenu();

        // Запрашиваем выбор пользователя
        std::cin >> choice;

        // Выполняем действие в зависимости от выбора
        switch (choice) {
        case 1:
            dispatcher.viewRegisteredDrivers();
            break;
        case 2:
            dispatcher.viewRegisteredVehicles();
            break;
        case 3:
            dispatcher.viewRegisteredClients();
            break;
        //case 4:
            //break;
        case 5:
            dispatcher.registerExpeditor();
            break;
        case 6:
            dispatcher.viewRegisteredOrders();
            break;
        //case 7:
        //    dispatcher.viewTransportationInfo();
        //    break;
        case 8:
            dispatcher.deleteExpeditor();
            break;
        case 9:
            dispatcher.viewReport();
            break;
        /*case 10:
            break;*/
        case 11:
            dispatcher.registerOrder();
            break;
        case 12:
            dispatcher.registerClient();
            break;
        case 13:
            std::cout << "Выход..." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
            break;
        }
    } while (choice != 13);

    sqlite3_close(db);

    return 0;
}
