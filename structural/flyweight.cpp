#include <iostream>
#include <vector>
#include <memory>

class Canvas {
public:
    void drawBitmap(std::string bitmap, int x, int y) {
        std::cout << "Drawing on the canvas\n";
        std::cout << bitmap << ": (" << x << ", " << y << ")\n";
    }
};

class TreeType {
public:
    std::string name;
    std::string color;
    std::string texture;

    TreeType(std::string& name, std::string& color, std::string& texture) : name(name), color(color), texture(texture) { }

    void draw(Canvas& canvas, int x, int y) {
        std::string bitmap = "Tree[" + name + ", " + color + ", " + texture + "]";
        canvas.drawBitmap(bitmap, x, y);
    }
};


class TreeFactory {
public:
    static std::vector<std::shared_ptr<TreeType>> tree_types;
    static std::shared_ptr<TreeType> getTreeType(std::string& name, std::string& color, std::string& texture) {
        for (auto& tree_type : tree_types) {
            if (tree_type->name == name && tree_type->color == color && tree_type->texture == texture) {
                return tree_type;
            }
        }
        auto tree_type = std::make_shared<TreeType>(name, color, texture);
        tree_types.push_back(tree_type);
        return tree_type;
    }
};

std::vector<std::shared_ptr<TreeType>> TreeFactory::tree_types;

class Tree {
public:
    int x, y;
    std::shared_ptr<TreeType> type;
    Tree(int x, int y, std::shared_ptr<TreeType> type) : x(x), y(y), type(type) { }
    void draw(Canvas& canvas) {
        type->draw(canvas, x, y);
    }
};

class Forest {
public:
    std::vector<std::shared_ptr<Tree>> trees;
    void plantTree(int x, int y, std::string name, std::string color, std::string texture) {
        std::shared_ptr<TreeType> type = TreeFactory::getTreeType(name, color, texture);
        std::shared_ptr<Tree> tree = std::make_shared<Tree>(x, y, type);
        trees.push_back(tree);
    }
    void draw(Canvas& canvas) {
        for (auto& tree : trees) {
            tree->draw(canvas);
        }
    }
};

int main() {
    Canvas canvas;
    Forest forest;
    forest.plantTree(5, 6, "kechi", "green", "communicative");
    forest.plantTree(2, 0, "Tkhki", "blue", "parallel");
    forest.draw(canvas);
}