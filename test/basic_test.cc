#include <gtest/gtest.h>
#include <map>
#include <string>


TEST(StandardTest, StandardCPP) {
    using namespace std;
    map<string, string> m;
    m["yolo"] = "dami";
    m["ceirl"] = "foko";
    ASSERT_EQ(m.at("yolo"), "dami");
    std::cout << m.at("yolou");
}