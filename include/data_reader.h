#pragma once
#include <iostream>

#include "bulk_handler.h"

class data_reader
{
public:
	data_reader(std::istream& input, bulk_handler* o) :
		input_stream	(input),
		single_observer	(o) {}

	void Perform() const
	{
		while (!input_stream.eof())
		{
			auto strCmd = std::string();
			input_stream >> strCmd;
			if (strCmd.empty()) continue;
			single_observer->NewCommandReceived(strCmd);
		}
		single_observer->TryFlush();
	}
private:
	std::istream& input_stream;
	bulk_handler* const single_observer;
};
