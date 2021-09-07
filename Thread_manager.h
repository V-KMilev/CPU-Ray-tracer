#pragma once

#include <iostream>

#include <condition_variable>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <atomic>
#include <queue>

#include "Bucket.h"

void render(const Bucket &my_bucket_task);

const int MAX_NUMBER_OF_THREADS = std::thread::hardware_concurrency();

class ThreadPool {
public:
	// using Task = std::function<Bucket()>;             // Task is used as void funtion(void)

	explicit ThreadPool(std::size_t num_Threads) {    // explicit so size_t to size_t only
		start(num_Threads);
	}

	~ThreadPool() {
		stop();
	}

	void enter_queue(const Bucket &my_bucket_task) {              // Promise to get in future

		{
			std::unique_lock<std::mutex> lock(my_Event_Mutex);    // Lock to make it single threaded
			// Add new Task at the end of the queue
			my_Tasks.emplace(std::move(my_bucket_task));
			counter++;
		}

		my_Event_Var.notify_all();                                // Wakeup all threads
	}

	void master_wait() {
		std::unique_lock<std::mutex> lock(my_Master_Mutex);                            // Lock to make it single threaded
		
		while (!my_done) {
			my_Release_Master.wait(lock, [=]() { return my_Stopping || my_done; });    // Wait until there is no break condition
		}
		my_done = false;
	}

private:
	std::vector<std::thread> my_Threads;          // Vector to hold all the threads

	std::condition_variable my_Event_Var;         // Variable for current thread condition
	std::mutex my_Event_Mutex;                    // Current event mutex

	std::condition_variable my_Release_Master;    // Variable for master thread condition
	std::mutex my_Master_Mutex;                   // Master mutex

	bool my_Stopping = false;                     // Boolen for stopped thread
	bool my_done = false;                            // Boolen for all Task are done

	std::queue<Bucket> my_Tasks;                  // Queue with the Tasks

	std::atomic<int> counter = {0};               // Task index

	void start(std::size_t num_Threads) {

		for (int i = 0; i < num_Threads; i++) {

			// Add new thread with a task at the end of the vector
			my_Threads.emplace_back(
				[=]() {
					while (true) {

						Bucket task;
						{
							std::unique_lock<std::mutex> lock(my_Event_Mutex);                              // Lock to make it single threaded
							my_Event_Var.wait(lock, [=]() { return my_Stopping || !my_Tasks.empty(); });    // Wait until there is no break condition

							if (my_Stopping) { break; }                                                     // Leave if current thread is stopped

							task = std::move(my_Tasks.front());                                             // Set this Task as the first element
							my_Tasks.pop();                                                                 // Remove first element
						}

						render(task);                                                                       // Execute current Task
						int value = counter.fetch_sub(1) - 1;                                               // Downgrade Task index

						if (value == 0) {
							my_done = true;
							my_Release_Master.notify_one();
						}
					}
				});
		}
	}

	void stop() noexcept {

		my_Stopping = true;                          // Set to stopped

		my_Event_Var.notify_all();                   // Wakeup all threads so they can stop

		for (std::thread &my_thread : my_Threads) {
			my_thread.join();                        // Join all threads
		}
	}
};
