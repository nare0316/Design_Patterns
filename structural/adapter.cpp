#include <iostream>
#include <cmath>

class RoundPeg {
    float radius;
public:
    virtual ~RoundPeg() = default;
    RoundPeg(float radius) : radius(radius) {}
    RoundPeg() = default;
    virtual float getRadius() const {
        return radius;
    }
};

class RoundHole {
    float radius;
public:
    RoundHole(float radius) : radius(radius) {}
    RoundHole() = default;
    float getRadius() const {
        return radius;
    }

    bool fits(RoundPeg& peg) {
        return radius >= peg.getRadius();
    }
};


class SquarePeg {
    float width;
public:
    SquarePeg(float width) : width(width) {}
    SquarePeg() = default;
    float getWidth() const {
        return width;
    }
};


class SqurePegAdapter : public RoundPeg {
    const SquarePeg& peg;

public:
    SqurePegAdapter(const SquarePeg& peg) : peg(peg) {}

    float getRadius() const override {
        return peg.getWidth() * (sqrt(2) / 2);
    }
};

int main() {
    RoundHole hole(10);
    RoundPeg peg(5);
    SquarePeg peg2(9);

    SqurePegAdapter AdaptedSquarePed(peg2);
    std::cout << hole.fits(AdaptedSquarePed) << std::endl;
}