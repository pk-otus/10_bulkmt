#pragma once
#include "special.h"
#include "task_queue.h"

namespace bulkmt
{
	class data_reader : special_command_handler
	{
	public:
		data_reader(size_t num_commands, size_t cnt_file_threads) :
			special_command_handler(num_commands),
			file_threads(cnt_file_threads),
			cout_threads(1),
			commands(nullptr) {}

		~data_reader()
		{
			std::cout << GetStatistics();
		}


		void Receive(const char* str, size_t sz)
		{
			if (1 == sz && TryHandleSpecial(*str))
			{
				stats.AddString();
				return;
			}
			if (!commands)
			{
				commands = CreateCommandBlock();
			}

			commands->AddCommand(str, sz);
			if (commands->IsFull())
				Flush();
		}

		data_reader_results GetStatistics()
		{
			if (commands)
			{
				if (commands->NeedsLastFulsh())
				{
					Flush();
				}
				else
				{
					stats.AddString(commands->CommandsCount());
				}
				commands = nullptr;
			}

			return { stats,
			cout_threads.GetStatistics().front(),
			file_threads.GetStatistics() };
		}

	private:
		void Flush() override
		{
			if (commands)
			{
				stats.basic_stat.AddBlock(commands->CommandsCount());
				cout_threads.Push(commands);
				file_threads.Push(commands);

				commands = nullptr;
			}
		}

		task_queue<file_element_t> file_threads;
		task_queue<cout_element_t> cout_threads;

		stat_special_counter stats;
		std::unique_ptr<commands_block>	commands;
	};
}