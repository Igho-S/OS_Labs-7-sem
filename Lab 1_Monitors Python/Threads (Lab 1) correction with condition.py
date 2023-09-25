import threading
import time

shared_resource = []  # Shared resource
condition = threading.Condition()

hub = 0  # Shared variable
list_1 = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

def consumer():
    with condition:
        while not hub:
            print("Consumer is waiting...")
            condition.wait()
        item = hub
        hub = 0
        print("Consumer consumed item:", item)

def notify(a):
    global hub
    print(f"Sent {a}   thread id {threading.current_thread().ident}")
    hub = a
    condition.notify()

def provide(x):
    for item in x:
        with condition:
            item = "New Item"
            print("Provider provided item:", item)
            time.sleep(1)
            notify(item)

def use():
    global hub
    while True:
        with condition:
            while not hub:
                pass
            if list_1:
                item_from_list_1 = list_1.pop()
                hub = item_from_list_1
                print(f"Received {hub}   thread id {threading.current_thread().ident}")
            else:
                break

t1 = threading.Thread(target = provide, args = (list_1,))
t2 = threading.Thread(target = use)

t1.start()
t2.start()

t1.join()
t2.join()
