#include <iostream>
#include <vector>
#include <memory>


class Graphic {
public:
    virtual ~Graphic() = default;
    virtual void move(size_t z, size_t y) = 0;
    virtual void draw() = 0;
    virtual bool operator==(const Graphic& oth) = 0;
};

class Dot : public Graphic {
protected:
    size_t x, y;
public:
    Dot(size_t x, size_t y) : x(x), y(y) {}
    void move(size_t x, size_t y) {
        this->x += x;
        this->y += y;
    }

    void draw() {
        std::cout << "." << " (" << x << "," << y << ")" << std::endl;
    }
    bool operator==(const Graphic& oth)  {
        const Dot* othr = dynamic_cast<const Dot*>(&oth);
        if (!othr) return false;
    
        return x == othr->x && y == othr->y;
    }

};

class Circle : public Dot {
    float radius;
public:
    Circle(size_t x, size_t y, float radius) : Dot(x, y), radius(radius) {}
    void draw() {
        std::cout << "◯ - > " << "radius: " << radius << ", center: x = " << x << ", y = " << y << std::endl;
    }
    bool operator==(const Graphic& oth) {
        const Circle* othr = dynamic_cast<const Circle*>(&oth);
        if (!othr) return false;
    
        return x == othr->x &&
               y == othr->y &&
               radius == othr->radius;
    }

};

class CompoundGraphic :  public Graphic {
public:
    std::vector<std::shared_ptr<Graphic>> graphics;
    void add(std::shared_ptr<Graphic> child) {
        graphics.push_back(std::move(child));
    }
    void remove(std::shared_ptr<Graphic> child) {
        size_t size = graphics.size();
        for (size_t i{}; i < size; ++i) {
            if (*(graphics[i]) == *child) {
                graphics.erase(graphics.begin() + i);
                return;
             }
        }
    }

    void move(size_t x, size_t y) {
        for (auto& child : graphics) {
            child->move(x, y);
        }
    }

    void draw() {
        for (auto& child : graphics) {
            child->draw();
        }
    }
    virtual bool operator==(const Graphic& oth)  {
        return this == &oth;
    }

};

class ImageEditor {
public:
    std::shared_ptr<CompoundGraphic> all;
    void load() {
        all = std::make_shared<CompoundGraphic>();
        all->add(std::make_shared<Dot>(1, 2));
        all->add(std::make_shared<Circle>(6, 3, 10));
    }

    void groupSelected(std::vector<std::shared_ptr<Graphic>> graph_vec) {
        std::shared_ptr<CompoundGraphic> new_graphics = std::make_shared<CompoundGraphic>();
        for (auto& graph : graph_vec) {
            new_graphics->add(graph);
            all->remove(graph);
        }
        all->add(new_graphics);
        all->draw();
    }
};

int main() {
    std::unique_ptr<ImageEditor> image_editor = std::make_unique<ImageEditor>();
    image_editor->load();
    image_editor->all->draw();
    std::vector<std::shared_ptr<Graphic>> graph_vec;
    graph_vec.push_back(std::make_shared<Dot>(1, 2));
    graph_vec.push_back(std::make_shared<Circle>(5, 3, 10));
    std::cout << ".......\n";
    image_editor->groupSelected(graph_vec);
    std::cout << ".......\n";

}