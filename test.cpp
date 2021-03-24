#include <iostream>
#include <cmath>
#include <sstream>
#include "pch.h"
using namespace std;

#define MAX_BRANCHES 13

struct Branch
{
	int id;
	Branch(int id)
	{
		this->id = id;
	}
};

void input(double& a, double& b, double& c, double& d, double& e, double& f, std::istream& cin = std::cin)
{
	cin >> a >> b >> c >> d >> e >> f;
}


Branch output(double a, double b, double c, double d, double e, double f, std::ostream& cout = std::cout)
{
	if ((a == 0) && (b == 0) && (c == 0) && (d == 0) && (e == 0) && (f == 0))
	{
		cout << '5';
		return Branch(0);
	}
	else if ((a * d - c * b != 0) && ((e * d - b * f != 0) || (a * f - c * e != 0)))
	{
		double y = (a * f - c * e) / (a * d - c * b);
		double x = (d * e - b * f) / (d * a - b * c);
		cout << "2 " << x << ' ' << y;
		return Branch(1);
	}
	else if (((a * d - c * b == 0) && ((e * d - b * f != 0) || (a * f - c * e != 0))) ||
		(a == 0 && c == 0 && e / b != f / d) ||
		(b == 0 && d == 0 && e / a != f / c) ||
		(a == 0 && b == 0 && c == 0 && d == 0 && (e / f > 0)))
	{
		if (((a == 0 && b == 0 && e == 0 && d != 0 && c == 0) ||
			(c == 0 && d == 0 && f == 0 && b != 0 && a == 0)))
		{
			double y;
			if (b == 0)
				y = f / d;
			else if (d == 0)
				y = e / b;
			else if (e == 0 || f == 0)
				y = 0;
			cout << '4' << ' ' << y;
			return Branch(2);
		}
		else if (((a == 0 && b == 0 && e == 0 && c != 0 && d == 0) ||
			(c == 0 && d == 0 && f == 0 && a != 0 && b == 0)))
		{
			double x;
			if (a == 0)
				x = f / c;
			else if (c == 0)
				x = e / a;
			else if (e == 0 || f == 0)
				x = 0;
			cout << '3' << ' ' << x;
			return Branch(3);
		}
		else
		{
			cout << '0';
			Branch(4);
		}
	}
	else if (a == 0 && c == 0)
	{
		double y;
		if (e == 0)
			y = f / d;
		else if (f == 0)
			y = e / b;
		else
			y = e / b;
		cout << '4' << ' ' << y;
		return Branch(5);
	}
	else if (b == 0 && d == 0)
	{
		double x;
		if (e == 0)
			x = f / c;
		else if (f == 0)
			x = e / a;
		else
			x = e / a;
		cout << '3' << ' ' << x;
		return Branch(6);
	}
	else if (b == 0 && e == 0)
	{
		double k, n;
		k = -c / d;
		n = f / d;
		cout << '1' << ' ' << k << ' ' << n;
		return Branch(7);
	}
	else if (d == 0 && f == 0)
	{
		double k, n;
		k = -a / b;
		n = e / b;
		cout << '1' << ' ' << k << ' ' << n;
		return Branch(8);
	}
	else if (a == 0 && e == 0)
	{
		double k, n;
		k = -d / c;
		n = f / c;
		cout << '1' << ' ' << k << ' ' << n;
		return Branch(9);
	}
	else if (c == 0 && f == 0)
	{
		double k, n;
		k = -b / a;
		n = e / a;
		cout << '1' << ' ' << k << ' ' << n;
		return Branch(10);
	}
	else if ((a / b == c / d))
	{
		double k, n;
		k = -c / d;
		n = f / d;
		cout << '1' << ' ' << k << ' ' << n;
		return Branch(11);
	}
	else
	{
		cout << "Are you kidding me?";
		return Branch(12);
	}
}

int main()
{
	//double a, b, c, d, e, f;
	//input(a, b, c, d, e, f);
	//output(a, b, c, d, e, f);
	return RUN_ALL_TESTS();
}

TEST(TestInput, CorrectInputGiven) {
	double a, b, c, d, e, f;
	istringstream is("2 3 4.1 5 6 7.7");
	ASSERT_NO_THROW(
		input(a, b, c, d, e, f, is);
	) << "Input exception not handled";
	EXPECT_EQ(a, 2);
	EXPECT_EQ(b, 3);
	EXPECT_EQ(c, 4.1);
	EXPECT_EQ(d, 5);
	EXPECT_EQ(e, 6);
	EXPECT_EQ(f, 7.7);
}

TEST(TestInput, IncorrectInputGiven) {
	double a, b, c, d, e, f;
	istringstream is("fef''' 2 3 4.1 5 r.r 6 7.7");
	ASSERT_NO_THROW(
		input(a, b, c, d, e, f, is);
	) << "Input exception not handled";
	EXPECT_EQ(a, 2);
	EXPECT_EQ(b, 3);
	EXPECT_EQ(c, 4.1);
	EXPECT_EQ(d, 5);
	EXPECT_EQ(e, 6);
	EXPECT_EQ(f, 7.7);
}

class TestOutput : public ::testing::Test {
protected:
	void SetUp() override {
		input.push_back({ 0, 0, 0,
				0, 0, 0 });
		result.push_back("5");
		input.push_back({ 0, 2, 8, 
				0, 0, 0 });
		result.push_back("4 4");
		input.push_back({ 0, 2, 8,
				0, 4, 16});
		result.push_back("4 4");
		input.push_back({ 2, 0, 8,
				0, 0, 0 });
		result.push_back("3 4");
		input.push_back({ 2, 0, 8,
				4, 0, 16 });
		result.push_back("3 4");
		input.push_back({ 1, 1, 5,
				2, -3, 1 });
		result.push_back("2 3.2 1.8");
		input.push_back({ 1, 0, 5,
				0, 1, -1 });
		result.push_back("2 5 -1");
		input.push_back({ 1, 0, 0,
				1, 1, -1 });
		result.push_back("2 0 -1");
		input.push_back({ 1, 1, 5,
				0, 1, 0 });
		result.push_back("2 5 0");
		input.push_back({ 1, 1, 5,
				1, 1, 5 });
		result.push_back("1 -1 5");
		input.push_back({ 1, 1, 5,
				2, 2, 10 });
		result.push_back("1 -1 5");
		input.push_back({ 2, 5, 5,
				2, 5, 10 });
		result.push_back("0");
		input.push_back({ 2, 5, 5,
				4, 10, 20 });
		result.push_back("0");
		input.push_back({ 0, 1, 0,
				0, 2, 20 });
		result.push_back("0");
		input.push_back({ 2, 0, 0,
				2, 0, 14 });
		result.push_back("0");
	}

	vector<vector<int>> input;
	vector<string> result;
};

TEST_F(TestOutput, AllBranchesReachable) {
	ostringstream os;
	vector<bool> reachedBranches;
	for (int i = 0; i < MAX_BRANCHES; i++)
		reachedBranches.push_back(false);

	for (int i = 0; i < result.size(); i++)
	{
		ASSERT_NO_THROW(
			Branch branch = output(input[i][0], input[i][1], input[i][2], input[i][3], input[i][4], input[i][5], os);
			reachedBranches[branch.id] = true;
		) << "Output exception not handled";
		os.str("");
		os.clear();
	}

	EXPECT_EQ(reachedBranches[MAX_BRANCHES-1], false) << "Unexpected last branch reach";

	for (int i = 0; i < MAX_BRANCHES-1; i++)
	{
		EXPECT_EQ(reachedBranches[i], true) << "Branch " + to_string(i) + " never reached";
	}
}

TEST_F(TestOutput, CorrectSituationRecognizing) {
	ostringstream os;

	for (int i = 0; i < result.size(); i++)
	{
		ASSERT_NO_THROW(
			Branch branch = output(input[i][0], input[i][1], input[i][2], input[i][3], input[i][4], input[i][5], os);
		auto expectedSituation = result[i][0];
		auto actualSituation = os.str()[0];
		EXPECT_EQ(actualSituation, expectedSituation) << "Wrong situation number (Branch " + to_string(branch.id) + ")" << endl <<
			"Input: " + to_string(input[i][0]) + " " + to_string(input[i][1]) + " " + to_string(input[i][2]) + " " + to_string(input[i][3]) + " " + to_string(input[i][4]) + " " + to_string(input[i][5]) << endl;
		) << "Output exception not handled";
		os.str("");
		os.clear();
	}
}

TEST_F(TestOutput, CorrectOutputComputing) {
	ostringstream os;

	for (int i = 0; i < result.size(); i++)
	{
		ASSERT_NO_THROW(
			Branch branch = output(input[i][0], input[i][1], input[i][2], input[i][3], input[i][4], input[i][5], os);
			string expectedOutput = result[i];
			string actualOutput = os.str();
			EXPECT_EQ(actualOutput, expectedOutput) << "Wrong output (Branch " + to_string(branch.id) + ")" << endl <<
				"Input: " + to_string(input[i][0]) + " " + to_string(input[i][1]) + " " + to_string(input[i][2]) + " " + to_string(input[i][3]) + " " + to_string(input[i][4]) + " " + to_string(input[i][5]) << endl;
		) << "Output exception not handled";
		os.str("");
		os.clear();
	}
}

