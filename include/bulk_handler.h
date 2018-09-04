#pragma once
#include "commands_block.h"

class bulk_handler
{
public:
	explicit bulk_handler(size_t n_commands) :
		count_brackets(0),
		commands(n_commands)
	{ }

	void NewCommandReceived(const std::string& cmd)
	{
		if (TryHandleSpecial(cmd)) return;
		commands.AddCommand(cmd);

		if (commands.IsFull())
			TryFlush();
	}

	void TryFlush()
	{
		if (0 == count_brackets)
			commands.Flush();
	}
private:
	bool TryHandleSpecial(const std::string& cmd)
	{
		if ("{" == cmd)
		{
			TryFlush();
			++count_brackets;
			return true;
		}
		if ("}" == cmd)
		{
			if (count_brackets)
			{
				--count_brackets;
				TryFlush();
			}
			return true;
		}
		return false;
	}

	size_t			count_brackets;
	commands_block	commands;
};
