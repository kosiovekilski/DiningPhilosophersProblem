#include <iostream> // cout
#include <cstdlib> // srand(), rand()
#include <unistd.h> // sleep(miliseconds)
#include <time.h> // time()
#include <thread> // thread
#include <mutex>

using namespace std;

mutex forks[5];

class Philosopher {
public:
	int id_;
	mutex& leftFork_;
	mutex& rightFork_;
	
	Philosopher(int id, mutex& l, mutex& r);
	void think();
	void eat();
	void getLeftFork();
	void getRightFork();
	void putDownForks();
	void run();
};

int rand_ms(){
	return rand() % 100 + 100;
}

Philosopher::Philosopher(int id, mutex& l, mutex& r) : leftFork_(l), rightFork_(r){
	id_ = id;
}

void Philosopher::think(){
	cout << id_ << ": thinking..." << endl;
	usleep(rand_ms());
}

void Philosopher::eat(){
	cout << id_ << ": eating..." << endl;
	usleep(rand_ms());
}

void Philosopher::getLeftFork(){
	leftFork_.lock();
	cout << id_ << ": left fork" << endl;
}

void Philosopher::getRightFork(){
	rightFork_.lock();
	cout << id_ << ": right fork" << endl;
}

void Philosopher::putDownForks(){
	leftFork_.unlock();
	rightFork_.unlock();
	cout << id_ << ": forks down" << endl;
}

void Philosopher::run() {
	while(1){
		think();
		getLeftFork();
		getRightFork();
		eat();
		putDownForks();
	}
}

int main() {
	srand(time(NULL));
	
	Philosopher p1(1, forks[0], forks[1]);
	Philosopher p2(2, forks[1], forks[2]);
	Philosopher p3(3, forks[2], forks[3]);
	Philosopher p4(4, forks[3], forks[4]);
	Philosopher p5(5, forks[4], forks[0]);
	
	thread t1(&Philosopher::run, &p1);
	thread t2(&Philosopher::run, &p2);
	thread t3(&Philosopher::run, &p3);
	thread t4(&Philosopher::run, &p4);
	thread t5(&Philosopher::run, &p5);
	
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	
	return 0;
}
