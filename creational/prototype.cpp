#include <iostream>

class Shape {
public:
    size_t x;
    size_t y;
    std::string color;

    Shape() = default;
    Shape(Shape& shape) {
        x = shape.x;
        y = shape.y;
        color = shape.color;
    }

    virtual std::string get_type() const = 0;
    virtual std::shared_ptr<Shape> clone() = 0;
};

class Rectangle : public Shape {
public:
    size_t width;
    size_t height;

    Rectangle() = default;
    Rectangle(Rectangle& rectangle) : Shape(rectangle) {
        width = rectangle.width;
        height = rectangle.height;
    }

   std::shared_ptr<Shape> clone() override {
        return std::make_shared<Rectangle>(*this);
    }

    virtual std::string get_type() const override {
        return "Recatngle";
    }

};

class Circle : public Shape {
public:
    size_t radius;
    Circle() = default;
    Circle(Circle& circle) : Shape(circle) {
        radius = circle.radius;
    }
    std::shared_ptr<Shape> clone() override {
        return std::make_shared<Circle>(*this);
    }
    virtual std::string get_type() const override {
        return "Circle";
    }

};

class Application {
public:
    std::vector<std::shared_ptr<Shape>> shapes;

    Application() {
        std::shared_ptr<Circle> circle = std::make_shared<Circle>();
        circle->x = 10;
        circle->y = 20;
        circle->radius = 5;
        shapes.push_back(circle);

        std::shared_ptr<Circle> circle2 = std::dynamic_pointer_cast<Circle>(circle->clone());
        shapes.push_back(circle2);

        std::shared_ptr<Rectangle> rectangle = std::make_shared<Rectangle>();
        rectangle->width = 40;
        rectangle->height = 30;
        shapes.push_back(rectangle);

        std::shared_ptr<Rectangle> rectangle2 = std::dynamic_pointer_cast<Rectangle>(rectangle->clone());
        shapes.push_back(rectangle2);
    }

    std::vector<std::shared_ptr<Shape>> businessLogic() {
        std::vector<std::shared_ptr<Shape>> new_shapes;
        for (auto& shape : shapes) {
            new_shapes.push_back(shape->clone());
        }
        return new_shapes;
    }

  

};

int main() {
    Application app;
    size_t size = app.shapes.size();

    std::vector<std::shared_ptr<Shape>> new_shapes = app.businessLogic();
    for (size_t i{}; i < size; ++i) {
        std::cout << new_shapes[i]->get_type() << std::endl;
    }
}