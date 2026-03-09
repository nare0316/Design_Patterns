#include <iostream>
#include <exception>
#include <memory>

class UnknowPlatform : public  std::runtime_error {
public:
    UnknowPlatform(const std::string& msg) : std::runtime_error(msg) { }
};

class Button {
public:
    virtual ~Button() = default;
    virtual void render() {
        std::cout << "general purpose page" << std::endl;
    }

    virtual void onClick() {
        std::cout << "general purpose button is pressed" << std::endl;
    }
};

class WindowsButton : public Button {
public:
    virtual ~WindowsButton() = default;

    virtual void render() override {
        std::cout << "Windows main page" << std::endl;
    }
    virtual void onClick() override {
        std::cout << "Windows button is pressed" << std::endl;
    }
};

class HTMLButton : public Button {
public:
    virtual ~HTMLButton() = default;

    virtual void render() override {
        std::cout << "HTML main page" << std::endl;
    }
    virtual void onClick() override {
        std::cout << "HTML button is pressed" << std::endl;
    }
};

class Dialog {
public:
    
    virtual ~Dialog() = default;

    virtual std::shared_ptr<Button> createButton() = 0;

    void render() {
        std::shared_ptr<Button> button = createButton();
        button->onClick();
    }
};

class WindowsDialog : public Dialog {
public:
    
    virtual ~WindowsDialog() = default;

    virtual std::shared_ptr<Button> createButton() override {
        return std::make_shared<WindowsButton>();
    }
};

class WebDialog : public Dialog {
public:
    virtual ~WebDialog() = default;

    virtual std::shared_ptr<Button> createButton() override {
        return std::make_shared<HTMLButton>();
    }
};


class Application {
public:
    Dialog *dialog;
    ~Application() {
        delete dialog;
    }

    void init (const std::string& render_type) {
        if (render_type == "Windows") {
            dialog = new WindowsDialog;
        } else if (render_type == "HTML") {
            dialog = new WebDialog;
        } else {
            throw UnknowPlatform("The platform is unknown\n");
        }
    }
};

int main() {
    Application app;
    app.init("Windows");
    app.dialog->render();
}