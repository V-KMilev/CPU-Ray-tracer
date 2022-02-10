#pragma once

#include <iostream>

#include <condition_variable>
#include <functional>

#include <vector>
#include <queue>

#include <future>
#include <thread>
#include <atomic>

#include "Settings.h"
#include "Bucket.h"

void render(const Bucket &my_bucket_task);

class ThreadPool {
	public:
		/* Explicit: size_t to size_t only. */
		explicit ThreadPool(std::size_t num_threads) {

			std::cerr << "\rStart Thread Pool: " << num_threads << "\n";

			start(num_threads);
		}

		~ThreadPool() {
			stop();
		}

		/*
		 * Function - enter_queue
		 *
		 * Parameters:
		 * [p] const Bucket &my_bucket_task
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling enter_queue you add new 
		 * bucket task to your queue of tasks.
		 */

		void enter_queue(const Bucket &my_bucket_task) {
			{
				/* Lock to make it single threaded. */
				std::unique_lock<std::mutex> lock(my_Event_Mutex);
				/* Add new Task at the end of the queue. */
				my_Tasks.emplace(std::move(my_bucket_task));
				counter++;
			}
			/* notify all threads for the new tasks. */
			my_Event_Var.notify_all();
		}

		/*
		 * Function - master_wait
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling master_wait you make 
		 * master thread wait until all thread workers 
		 * are done with there tasks.
		 */

		void master_wait() {
			/* Lock to make it single threaded. */
			std::unique_lock<std::mutex> lock(my_Master_Mutex);

			while (!my_Done) {
				/* Wait until there is no break condition. */
				my_Release_Master.wait(lock, [=]() { return my_Stopping || my_Done; });
			}
			my_Done = false;
		}

		/*
		 * Function - clear
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling clear you clear 
		 * all tasks from the queue.
		 */

		void clear() {
			while(!my_Tasks.empty()) {
				my_Tasks.pop();
			}
		}

	private:
		/*
		 * Function - start
		 *
		 * Parameters:
		 * [p] std::size_t num_threads
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling start you set your thread(s) 
		 * to work. Thread(s) take task from the queue 
		 * while there is non left.
		 */

		void start(std::size_t num_threads) {
			for (int i = 0; i < num_threads; i++) {

				/* Add new thread with a task at the end of the vector */
				my_Threads.emplace_back(
					[=]() {
						for(;;) {

							Bucket task;
							{
								/* Lock to make it single threaded. */
								std::unique_lock<std::mutex> lock(my_Event_Mutex);
								/* Wait until there is no break condition. */
								my_Event_Var.wait(lock, [=]() { return my_Stopping || !my_Tasks.empty(); });

								/* If stop has been called all work is stopped and left. */
								if (my_Stopping) { break; }

								/* Set current task first element in queue. */
								task = std::move(my_Tasks.front());
								/* Remove current task from queue. */
								my_Tasks.pop();
							}

							/* Execute current task. */
							render(task);

							/* If there is no more tasks left, notify and release master thread. */
							if (--counter == 0) {
								my_Done = true;
								my_Release_Master.notify_one();
							}
						}
					}
				);
			}
		}

		/*
		 * Function - stop
		 *
		 * Parameters:
		 * none
		 * 
		 * Return type:
		 * void
		 * 
		 * Use:
		 * By calling stop you call all 
		 * threads to stop there tasks and join.
		 */

		void stop() noexcept {
			/* Set to stopped. */
			my_Stopping = true;
			
			/* Wakeup all threads so they can stop. */
			my_Event_Var.notify_all();

			for (std::thread &my_thread : my_Threads) {
				/* Wait for all threads to join. */
				my_thread.join();
			}
		}

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
