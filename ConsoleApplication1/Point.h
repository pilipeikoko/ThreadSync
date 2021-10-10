#pragma once
#include <iostream>
#include <string>

using namespace std;

class Point {
private:
    int x;
    int y;
    bool deletable;
    string initTime;

    void makeDeletable();

public:
    Point(int x_value, int y_value, std::string initialized_time);
    ~Point();

    string logValue();
};
