#include <iostream>
#include <map>
#include <string>
#include <ranges>

using namespace std;


int main(int argc, char const *argv[])
{
    string* ptr;
    map<string, string> s;
    
    s.emplace(std::make_pair("one", "just one"));

    ptr = &(s.at("one"));

    *ptr = "just two now";

    decltype(s.at("one")) p = s.at("one");
    p = "yolo";

    cout << s.at("one") << " -- " << &(s.at("one"));

    return 0;
}
