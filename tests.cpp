/**
* TESTING FILE 
* 
* Tests implemented with BOOST Library.
* Make sure boost is linked by adding in additional included libraries (VS).
* 
*/

#define BOOST_TEST_MODULE _HullTesting

#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include "InitPoints.h"
#include "ReadFromFile.h"
#include "ConvexHull.h"

#define CQUEUE_SPLIT_TESTS 0
#if CQUEUE_SPLIT_TESTS 
int paramsI[] = { 2,30,40,50 };
BOOST_DATA_TEST_CASE(CQueueSplitsTestI, boost::unit_test::data::make(paramsI), myVar) {
	std::vector<int> values = { 2,30,40,50 };
	Node<int>* leaf1 = new Node<int>(&values[0]);
	Node<int>* leaf2 = new Node<int>(&values[1]);
	Node<int>* leaf3 = new Node<int>(&values[2]);
	Node<int>* leaf4 = new Node<int>(&values[3]);

	std::vector<Node<int>*> leaves = {
	leaf1, leaf2, leaf3, leaf4
	};

	for (int i = 3; i > 0; --i) {
		leaves[i]->setChainLeft(leaves[i - 1]);
	}

	Node<int>* l_internal = new Node<int>(leaf1, leaf1, leaf2);
	Node<int>* r_internal = new Node<int>(leaf3, leaf3, leaf4);

	Node<int>* root = new Node<int>(leaf2, l_internal, r_internal);
	CQueue<int>* CQ = new CQueue<int>(root, 2, leaf1, leaf4);
	CQueue<int>* CQ_less;

	CQ_less = CQ->Split(myVar, true, true);

	CQ->isValidTree();
	CQ_less->isValidTree();

	delete CQ;
	delete CQ_less;
}

int paramsII[] = { 5,10,30,40,50,60 };
BOOST_DATA_TEST_CASE(CQueueSplitsTestII, boost::unit_test::data::make(paramsII), myVar) {
	std::vector<int> values = { 5,10,30,40,50,60 };
	Node<int>* leaf1 = new Node<int>(&values[0]);
	Node<int>* leaf2 = new Node<int>(&values[1]);
	Node<int>* leaf3 = new Node<int>(&values[2]);
	Node<int>* leaf4 = new Node<int>(&values[3]);
	Node<int>* leaf5 = new Node<int>(&values[4]);
	Node<int>* leaf6 = new Node<int>(&values[5]);

	std::vector<Node<int>*> leaves = {
	leaf1, leaf2, leaf3, leaf4, leaf5, leaf6
	};

	for (int i = 5; i > 0; --i) {
		leaves[i]->setChainLeft(leaves[i - 1]);
	}

	Node<int>* l_internal = new Node<int>(leaf1, nullptr, leaf2, leaf1, leaf2, nullptr, leaf3);
	Node<int>* r_internal = new Node<int>(leaf4, nullptr, leaf5, leaf4, leaf5, nullptr, leaf6);

	Node<int>* root = new Node<int>(leaf3, l_internal, r_internal);

	CQueue<int>* CQ = new CQueue<int>(root, 2, leaf1, leaf6);
	CQueue<int>* CQ_less;

	CQ_less = CQ->Split(myVar, true, true);

	CQ->isValidTree();
	CQ_less->isValidTree();

	delete CQ;
	delete CQ_less;

}

int paramsIII[] = { 2,5,20,30,35,40,50,60,65,70,75,80 };

BOOST_DATA_TEST_CASE(CQueueSplitsTestIII, boost::unit_test::data::make(paramsIII), myVar) {
	std::vector<int> values = { 2,5,20,30,35,40,50,60,65,70,75,80 };
	Node<int>* leaf1 = new Node<int>(&values[0], &values[1]);
	Node<int>* leaf2 = new Node<int>(&values[2], &values[3]);
	Node<int>* leaf3 = new Node<int>(&values[4], &values[5]);
	Node<int>* leaf4 = new Node<int>(&values[6], &values[7]);
	Node<int>* leaf5 = new Node<int>(&values[8], &values[9]);
	Node<int>* leaf6 = new Node<int>(&values[10], &values[11]);

	std::vector<Node<int>*> leaves = {
	leaf1, leaf2, leaf3, leaf4, leaf5, leaf6
	};

	for (int i = 5; i > 0; --i) {
		leaves[i]->setChainLeft(leaves[i - 1]);
	}

	Node<int>* l_internal = new Node<int>(leaf1, leaf1, leaf2);
	Node<int>* m_internal = new Node<int>(leaf3, leaf3, leaf4);
	Node<int>* r_internal = new Node<int>(leaf5, leaf5, leaf6);

	Node<int>* root = new Node<int>(leaf2, nullptr, leaf4, l_internal, m_internal, nullptr, r_internal);
	CQueue<int>* CQ = new CQueue<int>(root, 2, leaf1, leaf6);
	CQueue<int>* CQ_less;

	CQ_less = CQ->Split(myVar, true, true);

	CQ->isValidTree();
	CQ_less->isValidTree();

	delete CQ;
	delete CQ_less;
}

int paramsIV[] = { 2,5,8,10,15,20,25,30,32,35,38,40,45,50,55,60,65,70,72,75,78,80,85,90,95,100,105,110,115,120,125,130 };
BOOST_DATA_TEST_CASE(CQueueSplitsTestIV, boost::unit_test::data::make(paramsIV), myVar) {
	std::vector<int> values = { 2,5,8,10,15,20,25,30,32,35,38,40,45,50,55,60,65,70,72,75,78,80,85,90,95,100,105,110,115,120,125,130 };
	Node<int>* leaf1 = new Node<int>(&values[0], &values[1]);
	Node<int>* leaf2 = new Node<int>(&values[2], &values[3]);
	Node<int>* leaf3 = new Node<int>(&values[4], &values[5]);
	Node<int>* leaf4 = new Node<int>(&values[6], &values[7]);
	Node<int>* leaf5 = new Node<int>(&values[8], &values[9]);
	Node<int>* leaf6 = new Node<int>(&values[10], &values[11]);
	Node<int>* leaf7 = new Node<int>(&values[12], &values[13]);
	Node<int>* leaf8 = new Node<int>(&values[14], &values[15]);
	Node<int>* leaf9 = new Node<int>(&values[16], &values[17]);
	Node<int>* leaf10 = new Node<int>(&values[18], &values[19]);
	Node<int>* leaf11 = new Node<int>(&values[20], &values[21]);
	Node<int>* leaf12 = new Node<int>(&values[22], &values[23]);
	Node<int>* leaf13 = new Node<int>(&values[24], &values[25]);
	Node<int>* leaf14 = new Node<int>(&values[26], &values[27]);
	Node<int>* leaf15 = new Node<int>(&values[28], &values[29]);
	Node<int>* leaf16 = new Node<int>(&values[30], &values[31]);

	std::vector<Node<int>*> leaves = {
		leaf1, leaf2, leaf3, leaf4, leaf5, leaf6, leaf7, leaf8, leaf9, leaf10, leaf11, leaf12, leaf13, leaf14, leaf15, leaf16
	};

	for (int i = 15; i > 0; --i) {
		leaves[i]->setChainLeft(leaves[i - 1]);
	}

	Node<int>* l_internal = new Node<int>(leaf1, leaf2, leaf3, leaf1, leaf2, leaf3, leaf4);
	Node<int>* m1_internal = new Node<int>(leaf5, leaf6, leaf7, leaf5, leaf6, leaf7, leaf8);
	Node<int>* m2_internal = new Node<int>(leaf9, leaf10, leaf11, leaf9, leaf10, leaf11, leaf12);
	Node<int>* r_internal = new Node<int>(leaf13, leaf14, leaf15, leaf13, leaf14, leaf15, leaf16);

	Node<int>* root = new Node<int>(leaf4, leaf8, leaf12, l_internal, m1_internal, m2_internal, r_internal);

	CQueue<int>* CQ = new CQueue<int>(root, 2, leaf1, leaf16);
	CQueue<int>* CQ_less;

	CQ_less = CQ->Split(myVar, true, true);

	CQ->isValidTree();
	CQ_less->isValidTree();

	delete CQ;
	delete CQ_less;
}

int paramsV[] = { 0,2,5,20,25,30,35,37,40,45,47,50,52,53,55,56,58,60,64,68,70,72,76,80,82,86,90,92,96,100,105,110,120,130,140,150 };

BOOST_DATA_TEST_CASE(CQueueSplitsTestV, boost::unit_test::data::make(paramsV), myVar) {
	std::vector<int> values = { 0,2,5,20,25,30,35,37,40,45,47,50,52,53,55,56,58,60,64,68,70,72,76,80,82,86,90,92,96,100,105,110,120,130,140,150 };

	Node<int>* leaf1 = new Node<int>(&values[0], &values[1], &values[2]);
	Node<int>* leaf2 = new Node<int>(&values[3], &values[4], &values[5]);
	Node<int>* leaf3 = new Node<int>(&values[6], &values[7], &values[8]);
	Node<int>* leaf4 = new Node<int>(&values[9], &values[10], &values[11]);
	Node<int>* leaf5 = new Node<int>(&values[12], &values[13], &values[14]);
	Node<int>* leaf6 = new Node<int>(&values[15], &values[16], &values[17]);
	Node<int>* leaf7 = new Node<int>(&values[18], &values[19], &values[20]);
	Node<int>* leaf8 = new Node<int>(&values[21], &values[22], &values[23]);
	Node<int>* leaf9 = new Node<int>(&values[24], &values[25], &values[26]);
	Node<int>* leaf10 = new Node<int>(&values[27], &values[28], &values[29]);
	Node<int>* leaf11 = new Node<int>(&values[30], &values[31], &values[32]);
	Node<int>* leaf12 = new Node<int>(&values[33], &values[34], &values[35]);

	std::vector<Node<int>*> leaves = {
	leaf1, leaf2, leaf3, leaf4, leaf5, leaf6, leaf7, leaf8, leaf9, leaf10, leaf11, leaf12
	};

	for (int i = 11; i > 0; --i) {
		leaves[i]->setChainLeft(leaves[i - 1]);
	}

	Node<int>* lowerl1internal = new Node<int>(leaf1, leaf1, leaf2);
	Node<int>* lowerm1internal = new Node<int>(leaf3, leaf3, leaf4);
	Node<int>* lowerr1internal = new Node<int>(leaf5, leaf5, leaf6);
	Node<int>* lowerl2internal = new Node<int>(leaf7, leaf7, leaf8);
	Node<int>* lowerm2internal = new Node<int>(leaf9, leaf9, leaf10);
	Node<int>* lowerr2internal = new Node<int>(leaf11, leaf11, leaf12);

	Node<int>* l_internal = new Node<int>(leaf2, nullptr, leaf4, lowerl1internal, lowerm1internal, nullptr, lowerr1internal);
	Node<int>* r_internal = new Node<int>(leaf8, nullptr, leaf10, lowerl2internal, lowerm2internal, nullptr, lowerr2internal);

	Node<int>* root = new Node<int>(leaf6, l_internal, r_internal);

	CQueue<int>* CQ = new CQueue<int>(root, 3, leaf1, leaf12);
	CQueue<int>* CQ_less;

	CQ_less = CQ->Split(myVar, true, true);

	CQ->isValidTree();
	CQ_less->isValidTree();

	delete CQ;
	delete CQ_less;

	BOOST_TEST_MESSAGE("Paramvalue" << myVar);
	BOOST_TEST(true);
}

int paramsVI[] = { 5, 10,30, 40, 50, 60, 70, 80, 90, 100, 110, 120 };
BOOST_DATA_TEST_CASE(CQueueSplitsTestVI, boost::unit_test::data::make(paramsVI), myVar) {
	std::vector<int> values = { 5, 10,30, 40, 50, 60, 70, 80, 90, 100, 110, 120 };

	Node<int>* leaf1 = new Node<int>(&values[0]);
	Node<int>* leaf2 = new Node<int>(&values[1]);
	Node<int>* leaf3 = new Node<int>(&values[2]);
	Node<int>* leaf4 = new Node<int>(&values[3]);
	Node<int>* leaf5 = new Node<int>(&values[4]);
	Node<int>* leaf6 = new Node<int>(&values[5]);
	Node<int>* leaf7 = new Node<int>(&values[6]);
	Node<int>* leaf8 = new Node<int>(&values[7]);
	Node<int>* leaf9 = new Node<int>(&values[8]);
	Node<int>* leaf10 = new Node<int>(&values[9]);
	Node<int>* leaf11 = new Node<int>(&values[10]);
	Node<int>* leaf12 = new Node<int>(&values[11]);

	std::vector<Node<int>*> leaves = {
	leaf1, leaf2, leaf3, leaf4, leaf5, leaf6, leaf7, leaf8, leaf9, leaf10, leaf11, leaf12
	};

	for (int i = 11; i > 0; --i) {
		leaves[i]->setChainLeft(leaves[i - 1]);
	}

	Node<int>* l_internal = new Node<int>(leaf1, nullptr, leaf2, leaf1, leaf2, nullptr, leaf3);
	Node<int>* m1_internal = new Node<int>(leaf4, nullptr, leaf5, leaf4, leaf5, nullptr, leaf6);
	Node<int>* m2_internal = new Node<int>(leaf7, nullptr, leaf8, leaf7, leaf8, nullptr, leaf9);
	Node<int>* r_internal = new Node<int>(leaf10, nullptr, leaf11, leaf10, leaf11, nullptr, leaf12);

	Node<int>* root = new Node<int>(leaf3, leaf6, leaf9, l_internal, m1_internal, m2_internal, r_internal);

	CQueue<int>* CQ = new CQueue<int>(root, 2, leaf1, leaf12);
	CQueue<int>* CQ_less;

	CQ_less = CQ->Split(myVar, true, true);

	CQ->isValidTree();
	CQ_less->isValidTree();

	delete CQ;
	delete CQ_less;

}

int paramsVII[] = { 10, 30, 50, 60, 80, 90, 100, 110 };

BOOST_DATA_TEST_CASE(CQueueSplitsTestVII, boost::unit_test::data::make(paramsVII), myVar) {
	std::vector<int> values = { 10, 30, 50, 60, 80, 90, 100, 110 };

	Node<int>* leaf1 = new Node<int>(&values[0]);
	Node<int>* leaf2 = new Node<int>(&values[1]);
	Node<int>* leaf3 = new Node<int>(&values[2]);
	Node<int>* leaf4 = new Node<int>(&values[3]);
	Node<int>* leaf5 = new Node<int>(&values[4]);
	Node<int>* leaf6 = new Node<int>(&values[5]);
	Node<int>* leaf7 = new Node<int>(&values[6]);
	Node<int>* leaf8 = new Node<int>(&values[7]);

	std::vector<Node<int>*> leaves = {
	leaf1, leaf2, leaf3, leaf4, leaf5, leaf6, leaf7, leaf8
	};

	for (int i = 7; i > 0; --i) {
		leaves[i]->setChainLeft(leaves[i - 1]);
	}

	Node<int>* l_internal = new Node<int>(leaf1, leaf1, leaf2);
	Node<int>* m1_internal = new Node<int>(leaf3, leaf3, leaf4);
	Node<int>* m2_internal = new Node<int>(leaf5, leaf5, leaf6);
	Node<int>* r_internal = new Node<int>(leaf7, leaf7, leaf8);

	Node<int>* root = new Node<int>(leaf2, leaf4, leaf6, l_internal, m1_internal, m2_internal, r_internal);

	CQueue<int>* CQ = new CQueue<int>(root, 2, leaf1, leaf8);
	CQueue<int>* CQ_less;

	CQ_less = CQ->Split(myVar, true, true);

	CQ->isValidTree();
	CQ_less->isValidTree();

	delete CQ;
	delete CQ_less;

}

#endif

#define ONLINE 0
#if ONLINE 
int dataSetSize[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000 };
BOOST_DATA_TEST_CASE(OnlineRandomPointsConvexHullTest, boost::unit_test::data::make(dataSetSize), numOfPoints)
{

	Instrumentor::Get().BeginSession("Profile", "profiling_results/results_" + std::to_string(numOfPoints) + ".json");

	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_real_distribution<double> distribution(-1000000, 1000000);

	Point_2D* p;
	ConvexHull<Point_2D> ch;
	std::vector<Point_2D*> points;

	{
		PROFILE_FUNCTION();

		for (int i = 0; i < numOfPoints; ++i) {
			p = new Point_2D(0.0 + distribution(g), 1.0 * distribution(g));
			ch.Insert(p);
		}
	}

	ch.OutputHull("test_results/Output_" + std::to_string(numOfPoints) + ".txt");

	ch.OutputPoints("test_points/Test_Points_" + std::to_string(numOfPoints) + ".txt");

	ch.DeletePoints();

	Instrumentor::Get().EndSession();
}
#endif

#define CIRCLE_DISTR 0
#if CIRCLE_DISTR
int dataCircleSetSize[] = { 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000 };
BOOST_DATA_TEST_CASE(RandomCirclePointsConvexHullTest, boost::unit_test::data::make(dataCircleSetSize), numOfPoints)
{

	Instrumentor::Get().BeginSession("Profile", "profiling_results/circle_results_" + std::to_string(numOfPoints) + ".json");

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 rngAngle(seed);
	seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 rngRadius(seed);

	std::uniform_real_distribution<double> angleDistribution(0, 360);
	std::uniform_real_distribution<double> radiusDistribution(0, 1000000);
	double angle, radius, x, y;

	Point_2D* p;
	ConvexHull<Point_2D> ch;
	std::vector<Point_2D*> points;

	{
		PROFILE_FUNCTION();

		for (int i = 0; i < numOfPoints; ++i) {
			angle = angleDistribution(rngAngle);
			radius = radiusDistribution(rngRadius);

			x = radius * sin(angle);
			y = radius * cos(angle);

			p = new Point_2D(x, y);
			ch.Insert(p);
		}
	}

	ch.OutputHull("test_results/Circle_Output_" + std::to_string(numOfPoints) + ".txt");

	ch.OutputPoints("test_points/Circle_Test_Points_" + std::to_string(numOfPoints) + ".txt");

	ch.DeletePoints();

	Instrumentor::Get().EndSession();
}
#endif

#define OFFLINE 0
#if OFFLINE
BOOST_AUTO_TEST_CASE(ReadFromFileOfflineConvexHullTest)
{
	int numOfPoints = 1000000;
	std::string timestamp(__TIME__);
	std::replace(timestamp.begin(), timestamp.end(), ':', '-');

	std::string testID(std::to_string(numOfPoints) + "__" + timestamp);

	Instrumentor::Get().BeginSession("Profile", "profiling_results/results_" + testID + ".json");

	std::pair<int, std::string> testVars = InitializeRandomPointsFile(numOfPoints, 50000);

	std::vector<Point_2D*> points = readPointsFromFile(testVars.first, testVars.second);

	ConvexHull<Point_2D> CH;
	{
		PROFILE_FUNCTION();
		for (Point_2D* p : points) {
			CH.Insert(p);
		}
	}

	CH.OutputHull("test_results/Output_" + testID + ".txt");


	for (Point_2D* p : points) {
		delete p;
	}

	Instrumentor::Get().EndSession();
}
#endif

#define DELETIONS 0
#if DELETIONS 
BOOST_AUTO_TEST_CASE(Deletions)
{

	int numOfPoints = 200000;
	Instrumentor::Get().BeginSession("Profile", "profiling_results/deletions_results_" + std::to_string(numOfPoints) + ".json");

	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_real_distribution<double> distribution(-1000000, 1000000);

	Point_2D* p;
	ConvexHull<Point_2D> ch;
	std::vector<Point_2D*> points;

	{
		PROFILE_FUNCTION();

		for (int i = 0; i < numOfPoints; ++i) {
			p = new Point_2D(0.0 + distribution(g), 1.0 * distribution(g));
			points.push_back(p);
			ch.Insert(p);
		}
	}

	std::shuffle(points.begin(), points.end(), g);

	{
		PROFILE_FUNCTION();

		for (int i = 0; i < 100000; ++i) {
			ch.Delete(*points[i]);
		}
	}

	for (Point_2D* p : points) {
		delete p;
	}

	Instrumentor::Get().EndSession();
}
#endif