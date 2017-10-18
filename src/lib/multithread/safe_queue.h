// Based on https://stackoverflow.com/questions/15278343/c11-thread-safe-queue

#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

namespace BiosHomeAutomator {

  template <class T>
  class SafeQueue
  {
    private:
      std::queue<T> queue;
      mutable std::mutex mutex;
      std::condition_variable conditionVariable;

    public:
      SafeQueue<T>(void) {}
      virtual ~SafeQueue(void) {}

    public:
      // Add an element to the queue.
      void enqueue(T element) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(element);
        conditionVariable.notify_one();
        // mutex is automatically released when lock goes out of scope
      }

      // Get the "front"-element.
      // If the queue is empty, wait till an element is avaiable.
      T dequeue(void) {
        std::unique_lock<std::mutex> lock(mutex);
        while(queue.empty()) {
          // release lock as long as the wait and reaquire it afterwards.
          conditionVariable.wait(lock);
        }
        T element = queue.front();
        queue.pop();
        return element;
      }
  };

};
