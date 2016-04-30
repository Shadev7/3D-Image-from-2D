#include <Point.h>

bool operator==(const Point& p1, const Point& p2) {
	if (p1.row != p2.row)
		return false;
	if (p1.col != p2.col)
		return false;
	return true;
}

