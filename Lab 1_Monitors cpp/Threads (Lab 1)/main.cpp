#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;  // Mutex for synchronization
std::condition_variable cv;  // Condition variable for signaling

bool isThread1Turn = true;  // Variable to track whose turn it is

void thread1Function() {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        while (!isThread1Turn) {
            cv.wait(lock);
        }

        // Task for thread 1
        std::cout << "Thread 1: Task " << i + 1 << std::endl;

        isThread1Turn = false;
        cv.notify_all();  // Notify other thread
    }
}

void thread2Function() {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> lock(mtx);
        while (isThread1Turn) {
            cv.wait(lock);
        }

        // Task for thread 2
        std::cout << "Thread 2: Task " << i + 1 << std::endl;

        isThread1Turn = true;
        cv.notify_all();  // Notify other thread
    }
}

int main() {
    std::thread thread1(thread1Function);
    std::thread thread2(thread2Function);

    thread1.join();
    thread2.join();

    return 0;
}
