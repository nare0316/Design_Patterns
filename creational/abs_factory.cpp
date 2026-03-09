#include <iostream>
#include <memory>

class UnknowPlatform : public  std::runtime_error {
public:
    UnknowPlatform(const std::string& msg) : std::runtime_error(msg) { }
};

class Button {
public:
    virtual ~Button() = default;
    virtual void paint() = 0;
};

class WinButton : public Button {
public:
    virtual ~WinButton() = default;
    virtual void paint() override {
        std::cout << "Window button" << std::endl;
    }
};

class MacButton : public Button {
public:
    virtual ~MacButton() = default;
    virtual void paint() override {
        std::cout << "Mac button" << std::endl;
    }
};


class Checkbox {
public:
    virtual ~Checkbox() = default;
    virtual void paint() = 0;
};

class WinCheckbox : public Checkbox {
public: 
    virtual ~WinCheckbox() = default;
    virtual void paint() override {
        std::cout << "Windows checkbox" << std::endl;
    }
};

class MacCheckbox : public Checkbox {
public:
    virtual ~MacCheckbox() = default;
    virtual void paint() override {
        std::cout << "Mac checkbox" << std::endl;
    }
};



class GUIFactory {
public:
    virtual ~GUIFactory() = default;
    virtual std::shared_ptr<Button> createButton()  = 0;
    virtual std::shared_ptr<Checkbox> createCheckbox() = 0;

};


class WinFactory : public GUIFactory {
public:
    virtual ~WinFactory() = default;
    virtual std::shared_ptr<Button> createButton() override {
        return std::make_shared<WinButton>();
    }
    virtual std::shared_ptr<Checkbox> createCheckbox() override {
        return std::make_shared<WinCheckbox>();
    }
};

class MacFactory : public GUIFactory {
public:
    virtual ~MacFactory() = default;
    virtual std::shared_ptr<Button> createButton() override {
        return std::make_shared<MacButton>();
    }
    virtual std::shared_ptr<Checkbox>  createCheckbox() override {
        return std::make_shared<MacCheckbox>();
    }
};

class Application {
    std::unique_ptr<GUIFactory> factory;
    std::shared_ptr<Button> button;
    std::shared_ptr<Checkbox> checkbox;

public:
    Application(std::unique_ptr<GUIFactory> factory) : factory(std::move(factory)) {}
    void createUI() {
        button = factory->createButton();
        checkbox = factory->createCheckbox();
    }
    void paint() {
        button->paint();
        checkbox->paint();
    }
};


int main() {
    std::string os_type = "Windows";
    std::unique_ptr<GUIFactory> factory;
    if (os_type == "Windows") {
        factory.reset(new WinFactory);
    } else if(os_type == "Mac") {
        factory.reset(new MacFactory);
    } else {
        throw UnknowPlatform("Unknown platform\n");
    }

    Application app(std::move(factory));
    app.createUI();
    app.paint();
}