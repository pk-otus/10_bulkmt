#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <algorithm>
#include <iterator>

class commands_block
{
public:
	explicit commands_block(size_t sz) :
		sz_fixed_buffer(sz),
		tm_created(0)
	{
	}

	bool IsFull() const
	{
		return sz_fixed_buffer == pool_commands.size();
	}
	void AddCommand(const std::string& cmd)
	{
		if (pool_commands.empty())
		{
			tm_created = time(nullptr);
		}
		pool_commands.push_back(cmd);
	}

	void Flush()
	{
		if (pool_commands.empty()) return;

		std::ofstream logfile("bulk" + std::to_string(tm_created) + ".log", std::ofstream::out);
		WriteToStream(logfile);
		logfile.close();

		WriteToStream(std::cout);

		pool_commands.clear();
	}
private:
	void WriteToStream(std::ostream& os)
	{
		const std::string DELIMITER = ", ";
		std::string result = "bulk: ";
		for(const auto& item : pool_commands)
		{
			result += item + DELIMITER;
		}
		result = result.substr(0, result.size() - DELIMITER.size());
		os << result << std::endl;
	}

	const size_t				sz_fixed_buffer;
	time_t						tm_created;
	std::vector<std::string>	pool_commands;
};
