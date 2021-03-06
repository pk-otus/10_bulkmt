#include <fstream>
#include <gtest/gtest.h>

#include "data_reader.h"

using test_result_t = std::tuple<bool, std::string>;

test_result_t GetTestResult(size_t test_number, int test_bulk_size)
{
	bool isValid = false;

	testing::internal::CaptureStdout();
	{
		std::ifstream file_in("in" + std::to_string(test_number) + ".txt", std::ofstream::in);
		bulkmt::data_reader reader(test_bulk_size, 2);
		for (std::string strCmd; std::getline(file_in, strCmd);)
		{
			strCmd.erase(std::remove(strCmd.begin(), strCmd.end(), '\r'), strCmd.end());
			if (strCmd.empty()) continue;
			reader.Receive(strCmd.c_str(), strCmd.size());
		}
		isValid = reader.GetStatistics().IsValidResults();
	}
	std::string output = testing::internal::GetCapturedStdout();
	auto result = output.substr(0, output.find("\nlog"));
	return std::make_tuple(isValid, result);
}

std::string GetControlResult(size_t test_number)
{
	std::string strOutFile = "out" + std::to_string(test_number) + ".txt";

	std::ifstream t(strOutFile);
	std::string result((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return result;
}


TEST(BulkTest, BulkMTTest1)
{
	std::string control = GetControlResult(1);
	auto tested = GetTestResult(1, 3);
	ASSERT_TRUE(std::get<0>(tested));
	ASSERT_EQ(std::get<1>(tested), control);
}

TEST(BulkTest, BulkMTTest2)
{
	std::string control = GetControlResult(2);
	auto tested = GetTestResult(2, 3);
	ASSERT_TRUE(std::get<0>(tested));
	ASSERT_EQ(std::get<1>(tested), control);
}

TEST(BulkTest, BulkMTTest3)
{
	std::string control = GetControlResult(3);
	auto tested = GetTestResult(3, 3);
	ASSERT_TRUE(std::get<0>(tested));
	ASSERT_EQ(std::get<1>(tested), control);
}

TEST(BulkTest, BulkMTTest4)
{
	std::string control = GetControlResult(4);
	auto tested = GetTestResult(4, 3);
	ASSERT_TRUE(std::get<0>(tested));
	ASSERT_EQ(std::get<1>(tested), control);
}

TEST(BulkTest, BulkMTTest5)
{
	std::string control = GetControlResult(5);
	auto tested = GetTestResult(5, 2);
	ASSERT_TRUE(std::get<0>(tested));
	ASSERT_EQ(std::get<1>(tested), control);
}
