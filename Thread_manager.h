#pragma once

#include <iostream>

#include <condition_variable>
#include <functional>
#include <future>
#include <vector>
#include <thread>
#include <queue>


class ThreadPool {
	public:
		const int MAX_NUMBER_OF_THREADS = std::thread::hardware_concurrency();
		
		using Task = std::function<void()>;

		explicit ThreadPool(std::size_t num_Threads) {
			start(num_Threads);
		}

		~ThreadPool() {
			stop();
		}

		template<class T>
		auto enter_queue(T task) -> std::future<decltype(task())> {
			
			auto wrapper = std::make_shared<std::packaged_task<decltype(task()) ()>>(std::move(task));
			{
				std::unique_lock<std::mutex> lock(my_Event_Mutex);
				my_Tasks.emplace(
				[=]() {
					(*wrapper)();
				});
			}

			my_Event_Var.notify_all();

			return wrapper->get_future();
		}

	private:
		std::vector<std::thread> my_Threads;
		
		std::condition_variable my_Event_Var;

		std::mutex my_Event_Mutex;

		bool my_Stopping = false;

		std::queue<Task> my_Tasks;

		void start(std::size_t num_Threads) {

			for (int i = 0; i < num_Threads; i++) {
				
				my_Threads.emplace_back(
					[=]() {
						while (true) {
							
							Task task;

							{
								std::unique_lock<std::mutex> lock(my_Event_Mutex);

								my_Event_Var.wait(lock, [=]() {return my_Stopping || !my_Tasks.empty(); });

								if(my_Stopping && my_Tasks.empty()) { break; }

								task = std::move(my_Tasks.front());
								my_Tasks.pop();
							}

							task();
						}
					});
			}
		}

		void stop() noexcept {
			{
				std::unique_lock<std::mutex> lock(my_Event_Mutex);
				my_Stopping = true;
			}

			my_Event_Var.notify_all();

			for(std::thread &thread : my_Threads) {
				thread.join();
			}
		}
};
