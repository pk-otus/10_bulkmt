#pragma once
#include <mutex>
#include <condition_variable>

#include "task_queue.h"

template<typename T> class threaded_stream 
{
public:	
	threaded_stream(const std::string& stat_title, size_t num_threads) :
		isWorking	(true),
		queue		(new basic_task_queue<T>(stat_title))
	{
		queue->AddThreads(num_threads, [this]() { DoWork(); });
	}

	~threaded_stream()
	{
		isWorking = false;
		cond_var.notify_all();
	}
	
	void Push(std::unique_ptr<commands_block>& elem)
	{
		std::unique_lock<std::mutex> lock(queue_guard);

		queue->Enqueue(elem);
		cond_var.notify_one();
	}
	
private:
	using queue_type = T;

	void DoWork()
	{
		std::unique_lock<std::mutex> lock(queue_guard);
		while (isWorking)
		{
			while (queue->IsEmpty() && isWorking ) 
			{  // loop to avoid spurious wakeups
				cond_var.wait(lock);
			}

			if (!queue->IsEmpty())
				queue->DequeueSingle();
		}
	}	

	bool								isWorking;
	std::mutex							queue_guard;
	std::condition_variable				cond_var;

	std::unique_ptr<basic_task_queue<T>>	queue;
};

