#pragma once
#include <vector>

#include "Point_2D.h"

std::vector<Point_2D*> readPointsFromFile(int numOfPoints, std::string fileName) {
	PROFILE_FUNCTION();

	std::vector<Point_2D*> outVec;
	std::ifstream inData(fileName);
	char delim;
	double x, y;

	for (int i = 0; i < numOfPoints; ++i) {
		inData >> delim
			>> x
			>> delim
			>> y
			>> delim >> delim;
		//std::cout << x << "," << y << std::endl;

		outVec.push_back(new Point_2D(x, y));
	}
	return outVec;
}

Point_2D* readPointsFromFileArray(int numOfPoints, std::string fileName) {
	PROFILE_FUNCTION();

	Point_2D* outVec = new Point_2D[numOfPoints];
	std::ifstream inData(fileName);
	char delim;
	double x, y;

	for (int i = 0; i < numOfPoints; ++i) {
		inData >> delim
			>> x
			>> delim
			>> y
			>> delim >> delim;
		//std::cout << x << "," << y << std::endl;

		outVec[i] = Point_2D(x, y);
	}
	return outVec;
}