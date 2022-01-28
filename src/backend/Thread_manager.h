#pragma once

#include <iostream>

#include <condition_variable>
#include <functional>

#include <vector>

#include <future>
#include <thread>
#include <atomic>

#include <queue>

#include "Bucket.h"
#include "Settings.h"

void render(const Bucket &my_bucket_task);

class ThreadPool {
	public:
		explicit ThreadPool(std::size_t num_threads) {    // Explicit: size_t to size_t only

			Logger::getDefaultLogger() << "\rStart Thread Pool: " << num_threads << "\n";

			#ifdef DEBUG
				std::cerr << "\rStart Thread Pool: " << num_threads << "\n";
			#endif

			start(num_threads);
		}

		~ThreadPool() {
			stop();
		}

		void enter_queue(const Bucket &my_bucket_task) {              // Enter task in the queue
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

			while (!my_Done) {
				my_Release_Master.wait(lock, [=]() { return my_Stopping || my_Done; });    // Wait until there is no break condition
			}
			my_Done = false;
		}

		void clear() {
			while(!my_Tasks.empty()) {
				my_Tasks.pop();
			}
		}

	private:
		void start(std::size_t num_threads) {
			for (int i = 0; i < num_threads; i++) {

				/* Add new thread with a task at the end of the vector */
				my_Threads.emplace_back(
					[=]() {
						for(;;) {

							Bucket task;
							{
								std::unique_lock<std::mutex> lock(my_Event_Mutex);                              // Lock to make it single threaded
								my_Event_Var.wait(lock, [=]() { return my_Stopping || !my_Tasks.empty(); });    // Wait until there is no break condition

								if (my_Stopping) { break; }                                                     // Leave if current thread is stopped

								task = std::move(my_Tasks.front());                                             // Set this Task as the first element
								my_Tasks.pop();                                                                 // Remove first element
							}

							render(task);                                                                       // Execute current Task

							if (--counter == 0) {
								my_Done = true;
								my_Release_Master.notify_one();
							}
						}
					}
				);
			}
		}

		void stop() noexcept {

			my_Stopping = true;                            // Set to stopped

			my_Event_Var.notify_all();                     // Wakeup all threads so they can stop

			for (std::thread &my_thread : my_Threads) {
				my_thread.join();                          // Join all threads
			}
		}

	public:
		// using Task = std::function<Bucket()>;         // Task is used as void funtion(Bucket)

	private:
		std::vector<std::thread> my_Threads;          // Vector to hold all the threads

		std::queue<Bucket> my_Tasks;                  // Queue with the Tasks

		std::condition_variable my_Event_Var;         // Variable for current thread condition
		std::condition_variable my_Release_Master;    // Variable for master thread condition

		std::mutex my_Event_Mutex;                    // Current event mutex
		std::mutex my_Master_Mutex;                   // Master mutex

		bool my_Stopping = false;                     // Boolen for stopped thread
		bool my_Done = false;                         // Boolen for all Task are done

		std::atomic<int> counter = {0};               // Task-in index
};
