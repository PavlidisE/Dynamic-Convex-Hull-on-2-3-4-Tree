#pragma once
#include <iostream>
#include <iomanip>

#include "Instrumentor.h"

typedef double coord_t;

class Point_2D
{
	/**
	 * @author Evripidis Pavlidis
	 * @since 2020-5-12
	 */

	coord_t x, y;
public:

	// We are ordering by value of y coord
	bool operator <(const Point_2D& p) const {
		return y < p.y || (y == p.y && x < p.x);
	}

	bool operator >(const Point_2D& p) const {
		return y > p.y || (y == p.y && x > p.x);
	}

	bool operator == (const Point_2D& p) const {
		return x == p.x && y == p.y;
	}

	bool operator <= (const Point_2D& p) const {
		return  y < p.y || (y == p.y && x <= p.x);
	}

	Point_2D() {
		this->x = INFINITY;
		this->y = INFINITY;
	};

	Point_2D(double x, double y) {
		this->x = x;
		this->y = y;
	}

	Point_2D(const Point_2D& other) {
		this->x = other.x;
		this->y = other.y;
	}

	friend std::ostream& operator<< (std::ostream& out, const Point_2D& point);

	coord_t GetX();
	coord_t GetY();

};

inline std::ostream& operator<< (std::ostream& out, const Point_2D& point)
{
	out << "(" << point.x << ", " << point.y << ")"; /*std::fixed << std::setprecision(3) <<*/

	return out;
}

inline coord_t Point_2D::GetX() {
	return this->x;
}

inline coord_t Point_2D::GetY() {
	return this->y;
}