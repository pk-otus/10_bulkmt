#pragma once
class stat_counter
{
public:
	explicit stat_counter(const std::string& t, size_t num = 0) :
		title			(t),
		thread_num		(num),
		block_counter	(0),
		command_counter	(0) {}

	~stat_counter() = default;

	size_t ThreadNumber() const { return thread_num; }
	size_t BlockCount() const	{ return block_counter; }
	size_t CommandCount() const { return command_counter; }

	std::string GetStatistics() const
	{
		auto strThreadNum = thread_num 
								? std::to_string(thread_num) 
								: std::string();
		return title + strThreadNum + ": " + CoreStatistics();
	}

	void AddBlock(size_t countCommands)
	{
		++block_counter;
		command_counter += countCommands;
	}

protected:
	virtual std::string CoreStatistics() const
	{
		return	std::to_string(block_counter) + " blocks, " +
				std::to_string(command_counter) + " commands\n";
	}
private:
	const std::string	title;
	const size_t		thread_num;
	size_t				block_counter;
	size_t				command_counter;
};

class stat_special_counter final: public stat_counter
{
public:
	explicit stat_special_counter(const std::string& t) :
		stat_counter(t),
		string_counter(0) {}

	void AddString(size_t cnt = 1)
	{
		string_counter += cnt;
	}
private:
	std::string CoreStatistics() const override
	{
		return	std::to_string(string_counter + CommandCount()) + " strings, " +
				stat_counter::CoreStatistics();
	}

	size_t				string_counter;
};