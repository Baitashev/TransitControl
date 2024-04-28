#include<iostream>
#include<string>
#include<vector>

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
    std::string name;
    std::string licenseNumber;
    int vehicleId;

    Expeditor(int _id, const std::string& _name, const std::string& _license, int _vehId)
        : id(_id), name(_name), licenseNumber(_license), vehicleId(_vehId) {}
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
    void viewRegisteredDrivers() {} // Просмотр зарегистрированных водителей
    void viewRegisteredVehicles() {} // Просмотр зарегистрированных автомобилей
    double viewTotalFreightCost(const std::string& month) { return 0.0; } // Просмотр общей стоимости перевозок за месяц
    void addTariff(Tariff tariff) {} // Добавление тарифа на перевозку
    void generateWaybill() {} // Генерация путевого листа
    void registerExpeditor(Expeditor expeditor) {} // Регистрация экспедитора
    void updateExpeditorInfo(const std::string& expeditorId, const std::string& info) {} // Изменение информации об экспедиторе
    void viewTransportationInfo() {} // Просмотр информации о перевозках
    void deleteExpeditor(const std::string& expeditorId) {} // Удаление экспедитора
    Report generateExpeditorReport(const std::string& expeditorId) { return Report(0, "", ""); } // Генерация отчета об экспедиторе
    void viewReport() {} // Просмотр отчета
    void registerOrder(Order order) {} // Регистрация заказа
    void registerClient(Client client) {} // Регистрация клиента
};

int main() {
    Dispatcher dispatcher;
    dispatcher.viewRegisteredDrivers();
    dispatcher.viewRegisteredVehicles();
    dispatcher.addTariff(Tariff(1, "Description", 10.0));
    dispatcher.generateWaybill();
    dispatcher.registerExpeditor(Expeditor(1, "John Doe", "123456", 1));
    dispatcher.updateExpeditorInfo("123456", "New info");
    dispatcher.viewTransportationInfo();
    dispatcher.deleteExpeditor("123456");
    dispatcher.viewReport();
    dispatcher.registerOrder(Order(1, 1, 1, 1, "In process"));
    dispatcher.registerClient(Client(1, "Client Name", "Contact info"));
    return 0;
}
