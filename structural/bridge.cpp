#include <iostream>

class Device {
public:
    virtual ~Device() = default;
    virtual void enable() = 0;
    virtual void disable() = 0;
    virtual bool isEnable() const = 0;
    virtual void setVolume(size_t vol) = 0;
    virtual void setChannel(size_t chann) = 0;
    virtual size_t getVolume() const = 0;
    virtual size_t getChannel() const = 0;
};

class Tv : public Device {
    bool isEn{};
    size_t volume{};
    size_t channel{};
public:
    void enable() override {
        isEn = true;
    }
    void disable() override {
        isEn = false;
    }
    bool isEnable() const override {
        return isEn;
    }
    void setVolume(size_t vol) override {
        volume = vol;
    }
    void setChannel(size_t chann) override {
        channel = chann;
    }

    size_t getVolume() const override {
        return volume;
    }
    size_t getChannel() const override {
        return channel;
    }
};

class Radio : public Device {
    bool isEn{};
    size_t volume{};
    size_t channel{};
public:
    void enable() override {
        isEn = true;
    }
    void disable() override {
        isEn = false;
    }
    bool isEnable() const override {
        return isEn;
    }
    void setVolume(size_t vol) override {
        volume = vol;
    }
    void setChannel(size_t chann) override {
        channel = chann;
    }

    size_t getVolume() const override {
        return volume;
    }
    size_t getChannel() const override {
        return channel;
    }
};

class RemoteControl {
protected:
    Device& device;
public:
    RemoteControl(Device& device) : device(device) {} 
    void togglePower() {
        if (device.isEnable()) {
            device.disable();
        } else {
            device.enable();
        }
        std::cout << (device.isEnable() ? "device is ON" : "device is OFF") << std::endl;
    }    
    void volumeDown() {
        device.setVolume(device.getVolume() - 10);
        std::cout << "volume: " << device.getVolume() << std::endl;
    }
    void volumeUp() {
        device.setVolume(device.getVolume() + 10);
        std::cout << "volume: " << device.getVolume() << std::endl;
    }
    void channelDown() {
        device.setChannel(device.getChannel() - 1);
        std::cout << "channel: " << device.getChannel() << std::endl;
    }
    void channelUp() {
        device.setChannel(device.getChannel() + 1);
        std::cout << "channel: " << device.getChannel() << std::endl;
    }
};

class AdvancedRemoteControl : public RemoteControl {
public:
    AdvancedRemoteControl(Device& device) : RemoteControl(device) {}
    void mute() {
        device.setVolume(0);
        std::cout << "device is mute\n";
    }
};


int main() {
    Tv* tv = new Tv;
    RemoteControl* remote = new RemoteControl(*tv);
    remote->togglePower();

    remote->volumeUp();
    remote->volumeUp();
    remote->volumeDown();
    remote->channelUp();
    remote->channelUp();
    remote->channelDown();
    remote->togglePower();

    delete tv;
    Radio radio;
    AdvancedRemoteControl radio_remote(radio);
    radio_remote.togglePower();
    radio_remote.volumeUp();
    radio_remote.mute();
}