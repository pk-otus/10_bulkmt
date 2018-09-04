#pragma once
#include "special.h"
#include "stream_out.h"

class data_reader : special_command_handler
{
public:
	data_reader(size_t num_commands) :
		special_command_handler(num_commands),		
		commands		(nullptr) {}

	void Perform(std::istream& input_stream)
	{
		auto strCmd = std::string();
		while (!input_stream.eof())
		{			
			strCmd = "";
			input_stream >> strCmd;

			if (TryHandleSpecial(strCmd)) continue;
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
	}
private:
	void Flush() override
	{
		if (commands)
		{			
			ostream_cout os;
			os.PrintElement(*commands);

			ostream_file fs;
			fs.PrintElement(*commands);
					   
			commands = nullptr;
		}
	}
	
	std::unique_ptr<commands_block>	commands;
};
