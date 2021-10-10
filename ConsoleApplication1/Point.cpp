#include "Point.h"

Point::Point(int x_value, int y_value, std::string initialized_time) {
	this->x = x_value;
	this->y = y_value;
	this->initTime = initialized_time;
	deletable = false;
}

Point::~Point() {
	if (!this->deletable)
		std::cout << "The point didn't log yet\n";
}

string Point::logValue() {
	std::string record = std::to_string(this->y) + " " + this->initTime + "\n";
	makeDeletable();
	return record;
}

void Point::makeDeletable()
{

	this->deletable = true;

}
