#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::vector<int> list_1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
int hub = 0; // Shared variable
//v
std::condition_variable condition;
std::vector<int> processed_list; // just another list

class Monitor_Class {
public:
    Monitor_Class() : mutex_() {} // Constructor to initialize the mutex

    void consumer() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex);
            while (hub == 0) {
                if (condition.wait_for(lock, std::chrono::milliseconds(1000) == std::cv_status::timeout) {
                    // to handle timeout if needed
                    std::cout << "Consumer timeout!" << std::endl;
                    break;
                }
            }
            int item = hub;
            hub = 0;
            lock.unlock();

            if (item != 0) {
                std::cout << "Consumer consumed item: " << item << std::endl;
            } else {
                //to handle the situation where hub is empty
                std::cout << "Consumer received no item." << std::endl;
            }
        }
    }

    void notify(int a) {
        std::unique_lock<std::mutex> lock(mutex);
        std::cout << "Sent " << a << "   thread id " << std::this_thread::get_id() << std::endl;
        hub = a;
        condition.notify_all();
    }

    void provide() {
        for (int item : list_1) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::unique_lock<std::mutex> lock(mutex);
            std::cout << "Provider provided item: New Item" << std::endl;
            notify(item);
        }
    }

    void use() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex);
            while (hub == 0) {
                if (condition.wait_for(lock, std::chrono::milliseconds(1000) == std::cv_status::timeout) {
                    // to handle timeout
                    std::cout << "Use thread timeout!" << std::endl;
                    break;
                }
            }
            if (!list_1.empty()) {
                int item_from_list_1 = list_1.back();
                list_1.pop_back();
                hub = item_from_list_1;
                std::cout << "Received " << hub << "   thread id " << std::this_thread::get_id() << std::endl;

                // to move the item to the processed_list
                processed_list.push_back(hub);
            } else {
                std::cout << "List_1 is empty. Exiting thread." << std::endl;
                break;
            }
            lock.unlock();
        }
    }


private:
    std::mutex mutex_; // Member variable for the mutex
};

int main() {

    Monitor_Class myObject; // here to create an instance of the Monitor_Class for use

    std::thread t1(&Monitor_Class::provide, &myObject);
    std::thread t2(&Monitor_Class::use, &myObject);
    std::thread t3(&Monitor_Class::consumer, &myObject);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
