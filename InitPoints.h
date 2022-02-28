#pragma once
#include <random>

#include "Instrumentor.h"

std::pair<int, std::string> InitializeRandomPointsFile(int numOfPoints, int boundaries)
{
	PROFILE_FUNCTION();

	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_real_distribution<double> distribution(-boundaries, boundaries);

	std::ofstream outFile;

	std::string fileName("test_points/Init_Test_Points_" + std::to_string(numOfPoints) + ".txt");

	outFile.open(fileName);

	outFile << std::fixed << std::setprecision(11);

	if (outFile) {

		for (int i = 0; i < numOfPoints - 1; ++i) {
			outFile << "(" << 0.0 + distribution(g) << "," << 1.0 * distribution(g) << "), \n";
		}

		outFile << "(" << 0.0 + distribution(g) << "," << 1.0 * distribution(g) << ")" << std::endl;

		outFile.close();
		return std::pair< int, std::string>(numOfPoints, fileName);
	}
	else {
		std::cerr << "Could not open File!" << std::endl;
		return std::pair<int, std::string>();
	}
}


std::pair<int, std::string> InitializeRandomPointsCircleFile(int numOfPoints, int boundaries)
{
	PROFILE_FUNCTION();

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 rngAngle(seed);
	seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 rngRadius(seed);

	std::uniform_real_distribution<double> angleDistribution(0, 360);
	std::uniform_real_distribution<double> radiusDistribution(0, boundaries);
	double angle, radius, x, y;


	std::string fileName("test_points/Circle_Test_Points_" + std::to_string(numOfPoints) + ".txt");
	std::ofstream outFile;

	outFile.open(fileName);
	outFile << std::fixed << std::setprecision(9);

	if (outFile) {
		for (int i = 0; i < numOfPoints - 1; i++) {
			angle = angleDistribution(rngAngle);
			radius = radiusDistribution(rngRadius);

			x = radius * sin(angle);
			y = radius * cos(angle);

			outFile << "(" << x << ", " << y << "), \n";

		}

		angle = angleDistribution(rngAngle);
		radius = radiusDistribution(rngRadius);

		x = radius * sin(angle);
		y = radius * cos(angle);

		outFile << "(" << x << ", " << y << ")" << std::endl;

		outFile.close();
		return std::pair< int, std::string>(numOfPoints, fileName);
	}
	else {
		std::cerr << "Could not open File!" << std::endl;
		return std::pair<int, std::string>();
	}

}