#ifndef __POINT_H__
#define __POINT_H__

class Point
{
public:
	Point() {}
	Point(int _col, int _row) : row(_row), col(_col) {}
	int row, col;

	friend bool operator==(const Point&, const Point&);
};

#endif
