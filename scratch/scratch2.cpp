#include <iostream>
#include <filesystem>
#include <string>

using namespace std;


int main(int argc, char const *argv[])
{
    auto f = filesystem::absolute(filesystem::current_path());
    cout << f;
    return 0;
}
