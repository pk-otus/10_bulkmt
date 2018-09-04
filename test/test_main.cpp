#include <fstream>
#include <gtest/gtest.h>

#include "data_reader.h"


std::string GetTestResult(size_t test_number, int test_bulk_size)
{
	std::ifstream file_in("in" + std::to_string(test_number) + ".txt", std::ofstream::in);

	testing::internal::CaptureStdout();

	bulk_handler handler(test_bulk_size);
	data_reader reader(file_in, &handler);
	reader.Perform();

	std::string output = testing::internal::GetCapturedStdout();

	return output;
}

std::string GetControlResult(size_t test_number)
{
	std::string strOutFile = "out" + std::to_string(test_number) + ".txt";

	std::ifstream t(strOutFile);
	std::string result((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());

	return result;
}

TEST(BulkTest, BulkTest1)
{
	std::string control = GetControlResult(1);
	std::string tested = GetTestResult(1, 3);
	ASSERT_EQ(tested, control);
}

TEST(BulkTest, BulkTest2)
{
	std::string control = GetControlResult(2);
	std::string tested = GetTestResult(2, 3);
	ASSERT_EQ(tested, control);
}

TEST(BulkTest, BulkTest3)
{
	std::string control = GetControlResult(3);
	std::string tested = GetTestResult(3, 3);
	ASSERT_EQ(tested, control);
}

TEST(BulkTest, BulkTest4)
{
	std::string control = GetControlResult(4);
	std::string tested = GetTestResult(4, 3);
	ASSERT_EQ(tested, control);
}

TEST(BulkTest, BulkTest5)
{
	std::string control = GetControlResult(5);
	std::string tested = GetTestResult(5, 2);
	ASSERT_EQ(tested, control);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
