
import threading
import time

def func():
    print("Start func")
    time.sleep(1)
    print("Exit func")
 
class Pt(threading.Thread):
    lock = threading.Lock()
   
    def __init__(self, counter):
        threading.Thread.__init__(self)
        self.counter = counter;

    def run(self):
        """Thread function run"""
        print("Start " + self.name)
        self.lock.acquire()
        self.printTime(self.name, 1, self.counter)
        self.lock.release()
        print("Exit" + self.name)
   
    def printTime(self, name, delay, counter):
        while counter:
            counter -= 1
            time.sleep(delay)
            print("{}:{}".format(name, time.ctime(time.time())))

if __name__ == "__main__":
    t0 = threading.Thread(target=func)
    t1 = Pt(1)
    t2 = Pt(2)
    t0.start()
    t1.start()
    t2.start()
    t0.join()
    t1.join()
    t2.join()
