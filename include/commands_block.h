#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <ctime>
#include <iterator>

class commands_block
{
public:
	commands_block() :	tm_created(time(nullptr)) { }

	commands_block				(const commands_block&) = delete;
	commands_block& operator=	(const commands_block&) = delete;

	virtual ~commands_block() = default;

	std::string GetLogFileName() const
	{
		return "bulk" + std::to_string(tm_created) + ".log";
	}

	friend std::ostream& operator<<(std::ostream& os, const commands_block& cmd)
	{
		if (!cmd.pool_commands.empty())
		{
			const std::string DELIMITER = ", ";
			std::string result = "bulk: ";
			for (const auto& item : cmd.pool_commands)
			{
				result += item + DELIMITER;
			}
			result = result.substr(0, result.size() - DELIMITER.size());
			os << result << std::endl;
		}
		return os;
	}

	virtual bool IsFull() const { return false; }

	void AddCommand(const std::string& cmd)
	{
		pool_commands.push_back(cmd);
	}

	size_t CommandsCount() const { return pool_commands.size(); }

private:
	const time_t				tm_created;
	std::vector<std::string>	pool_commands;
};

class limited_commands_block : public commands_block
{
public:
	explicit limited_commands_block(size_t sz) : sz_fixed_buffer(sz) { }

	bool IsFull() const override
	{
		return sz_fixed_buffer == CommandsCount();
	}

private:
	const size_t				sz_fixed_buffer;
};