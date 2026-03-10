#include <iostream>
#include <vector>
#include <memory>

class Component {
protected:
    std::string_view type;
public:
    Component(std::string_view type) : type(type) {}
    virtual ~Component() = default;
    virtual void print_type() const = 0;
};

class Engine : public Component {
public:
    Engine(std::string_view type = "Petrol") : Component(type) {}
    void print_type() const override {
        std::cout << "Engine type: " << type << std::endl;
    }
};

class TripComputer : public Component {
public:
    TripComputer(std::string_view type = "Desktop") : Component(type) {}
    void print_type() const override {
        std::cout << "Trip Computer type: " << type << std::endl;
    }
};

class GPS : public Component {
public:
    GPS(std::string_view type = "Automotive") : Component(type) {}
    void print_type() const override {
        std::cout << "GPS type: " << type << std::endl;
    }
};

class Manual {
    std::vector<std::shared_ptr<Component>> components;
    size_t seats{};
public:
    Manual() = default;
    void set_seats(size_t seat_num) {
        seats = seat_num;
    }
    void print_manual() {
        for (const auto& component : components) {
            component->print_type();
        }
        std::cout << "Seats number: " << seats << std::endl;
    }
    void set_component_doc(std::shared_ptr<Component> component_doc) {
        components.push_back(component_doc);
    }
};

class Car {
    size_t seat_num;
    std::vector<std::shared_ptr<Component>> components;
    std::shared_ptr<Manual> man;
public:
    Car() = default;
    size_t get_seat_num() const {
        return seat_num;
    }
    void set_seat_num(size_t new_seat_num) {
        seat_num = new_seat_num;
    }
    void add_component(std::shared_ptr<Component> new_component) {
        components.push_back(new_component);
    }
    void set_manual(std::shared_ptr<Manual> new_man) {
        man = new_man;
    }
};

class Builder {
public:
    virtual ~Builder() = default;
    virtual void reset() {}
    virtual void setSeats(size_t seat_num) = 0;
    virtual void setEngine(std::shared_ptr<Engine> engine) = 0;
    virtual void setTripComputer(std::shared_ptr<TripComputer> TripComputer) = 0;
    virtual void setGPS(std::shared_ptr<GPS> gps) = 0; 
    virtual void setManual(std::shared_ptr<Manual> man) {}
};

class CarBuilder : public Builder {
    std::shared_ptr<Car> car;
public:
    CarBuilder() {
        reset();
    }
    void reset() override {
        car = std::make_shared<Car>();
    }
    void setSeats(size_t seats_num) override {
        car->set_seat_num(seats_num);
    }
    void setEngine(std::shared_ptr<Engine> engine) override {
        car->add_component(std::move(engine));
    }
    void setTripComputer(std::shared_ptr<TripComputer> computer) override {
        car->add_component(std::move(computer));
    }
    void setGPS(std::shared_ptr<GPS> gps) override {
        car->add_component(std::move(gps));
    }

    void setManual(std::shared_ptr<Manual> manual) override {
        if (manual) {
            car->set_manual(manual);
        }
    }
    std::shared_ptr<Car> getProduct() {
        auto new_car = car;
        reset();
        return new_car;
    }
 };

 class CarManualBuilder : public Builder {
    std::shared_ptr<Manual> manual;
public:
    CarManualBuilder() {
        reset();
    }
    void reset() override {
        manual = std::make_shared<Manual>();
    }
    void setSeats(size_t seats_num) override {
        manual->set_seats(seats_num);
    }
    void setEngine(std::shared_ptr<Engine> engine) override {
        manual->set_component_doc(std::move(engine));
    }
    void setTripComputer(std::shared_ptr<TripComputer> computer) override {
        manual->set_component_doc(std::move(computer));
    }
    void setGPS(std::shared_ptr<GPS> gps) override {
        manual->set_component_doc(std::move(gps));
    }
    std::shared_ptr<Manual> getProduct()  {
        return manual;
    }
 };

class Director {
public:
    void constructCarManual(std::shared_ptr<Builder> builder) {
        builder->reset();
        builder->setSeats(3);
        builder->setEngine(std::make_shared<Engine>("Hydrogen"));
        builder->setTripComputer(std::make_shared<TripComputer>("Laptop"));
        builder->setGPS(std::make_shared<GPS>("Smartphone"));
    }
    void constructSportCar(std::shared_ptr<Builder> builder) {
        builder->reset();
        builder->setSeats(3);
        builder->setEngine(std::make_shared<Engine>("Hydrogen"));
        builder->setTripComputer(std::make_shared<TripComputer>("Laptop"));
        builder->setGPS(std::make_shared<GPS>("Smartphone"));
    }

 };
int main() {
    Director director;
    std::shared_ptr<CarBuilder> builder(new CarBuilder);
    director.constructSportCar(builder);
    std::shared_ptr<Car> car = builder->getProduct();
    

    std::shared_ptr<CarManualBuilder> manual_builder(new CarManualBuilder);
    director.constructCarManual(manual_builder);
    std::shared_ptr<Manual> manual = manual_builder->getProduct();

    car->set_manual(manual);
    manual->print_manual();
    
}

