#pragma once
#include <iostream>
#include <queue>
#include <thread>
#include <map>
#include <functional>
#include <algorithm>

#include "statistics.h"

template<typename T> T PopFirst(std::queue<T>& q)
{
	auto ptr = std::move(q.front());
	q.pop();
	return std::move(ptr);
}

using cout_element_t = std::pair<size_t, std::string>;
using file_element_t = std::unique_ptr<commands_block>;

template <typename T>
class basic_task_queue
{
public:
	explicit basic_task_queue(const std::string& title) : stat_title(title) {}

	virtual ~basic_task_queue()
	{
		for (auto& thrd : threads)
		{
			if (thrd.joinable())
				thrd.join();
		}
		std::vector<std::string> stat_results;
		std::transform(st.begin(), st.end(), std::back_inserter(stat_results),
			[](const auto& d) { return d.second.GetStatistics(); });

		std::sort(stat_results.begin(), stat_results.end());

		for (auto& stat : stat_results)
		{
			std::cout << stat;
		}
	}

	bool IsEmpty() const { return queue_elements.empty(); }

	void AddThreads(size_t num, std::function<void()> func)
	{

		for (size_t i = 0; i < num; ++i)
		{
			threads.emplace_back(func);
			st.insert({ threads.back().get_id(), 
						stat_counter(stat_title, (1 == num ?  0 : threads.size())) });
		}
	}

	void Enqueue(std::unique_ptr<commands_block>& elem);

	void DequeueSingle()
	{
		element_t elem;
		{
			std::lock_guard<std::mutex> lock(queue_guard);
			elem = PopFirst(queue_elements);
		}

		Process(elem);		
	}

private:
	using element_t = T;

	void Process(const element_t& e);

	const std::string						stat_title;
	std::vector<std::thread>				threads;
	std::map<std::thread::id ,stat_counter> st;
	std::mutex								queue_guard;
	std::queue<element_t>					queue_elements;
};

template <>
inline void basic_task_queue<file_element_t>::Enqueue(std::unique_ptr<commands_block>& elem)
{
	queue_elements.emplace(std::move(elem));
}
template <>
inline void basic_task_queue<cout_element_t>::Enqueue(std::unique_ptr<commands_block>& elem)
{
	queue_elements.emplace(elem->CommandsCount(), elem->GetString());
}

template <>
inline void basic_task_queue<file_element_t>::Process(const element_t& e)
{
	auto& stat = st.at(std::this_thread::get_id());
	stat.AddBlock(e->CommandsCount());
	e->LogToFile(stat.ThreadNumber(), stat.BlockCount());
}

template <>
inline void basic_task_queue<cout_element_t>::Process(const element_t& e)
{
	st.at(std::this_thread::get_id()).AddBlock(e.first);
	std::cout << e.second << std::endl;
}
