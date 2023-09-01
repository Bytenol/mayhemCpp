#include <iostream>
#include <string>

using namespace std;


class Box {
    double width;
    public:
        double length;
        friend void printWidth(Box box);
};


class Vec2 {
public:
    int x, y;
    std::string n;
    friend void sayNamex(Vec2& v);
private:
    void sayName(){ std::cout << n; }
};

void sayNamex(Vec2& v) {
    v.sayName();
}


int main(int argc, char const *argv[])
{
    Vec2 v1{ 50, 60, "rolex" }; 
    Vec2 v2{ 13, 23, "bournvita" };
    sayNamex(v2);
    return 0;
}
