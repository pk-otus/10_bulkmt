#pragma once
#include "special.h"
#include "stream_out.h"

class data_reader : special_command_handler
{
public:
	data_reader(size_t num_commands, size_t cnt_file_threads) :
		special_command_handler	(num_commands),
		file_threads			("file", cnt_file_threads),
		cout_threads			("log", 1),
		stats					("main"),
		commands				(nullptr) {}

	~data_reader()
	{
		std::cout << stats.GetStatistics();
	}

	void Perform(std::istream& input_stream)
	{
		auto strCmd = std::string();
		while (!input_stream.eof())
		{			
			strCmd = "";
			input_stream >> strCmd;
			if (strCmd.empty()) continue;

			if (TryHandleSpecial(strCmd))
			{
				stats.AddString();
				continue;
			}
			if (!commands)
			{
				commands = CreateCommandBlock();
			}

			commands->AddCommand(strCmd);
			if (commands->IsFull())
				Flush();
		}
		if (dynamic_cast<limited_commands_block*>(commands.get()))
		{
			Flush();
		}
		else
		{
			if (commands)
				stats.AddString(commands->CommandsCount());
		}
	}
private:
	void Flush() override
	{
		if (commands)
		{	
			stats.AddBlock(commands->CommandsCount());
			cout_threads.Push(commands);
			file_threads.Push(commands);
					   
			commands = nullptr;
		}
	}
	
	threaded_stream<file_element_t> file_threads;
	threaded_stream<cout_element_t> cout_threads;

	stat_special_counter stats;
	std::unique_ptr<commands_block>	commands;
};
