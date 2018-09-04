#pragma once
#include <iostream>
#include <fstream>

#include "commands_block.h"

class IStreamOut 
{
public:
	virtual ~IStreamOut() = default;
	virtual void PrintElement(const commands_block& block) = 0;
};

class ostream_cout : public IStreamOut
{
public:
	void PrintElement(const commands_block& block) override
	{
		std::cout << block;
	}
};

class ostream_file : public IStreamOut
{
public:
	void PrintElement(const commands_block& block) override
	{
		auto stream = std::ofstream(block.GetLogFileName(), std::ofstream::out);
		stream << block;
		stream.close();
	}

};