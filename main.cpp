#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <string>

using namespace std;

mutex mtx;

condition_variable cvPing;   // ждёт ping-поток
condition_variable cvPong;   // ждёт pong-поток

bool pingReady = false;      // ход ping-потока
bool pongReady = false;      // ход pong-потока

const int ROUNDS = 5;        // сколько раз обменяться

void pingThread() {
    for (int i = 0; i < ROUNDS; ++i) {
        unique_lock<mutex> lock(mtx);
        cvPing.wait(lock, [] { return pingReady; });

        cout << "ping" << endl;
        pingReady = false;
        pongReady = true;

        cvPong.notify_one();
    }
}

void pongThread() {
    for (int i = 0; i < ROUNDS; ++i) {
        unique_lock<mutex> lock(mtx);
        cvPong.wait(lock, [] { return pongReady; });

        cout << "pong" << endl;
        pongReady = false;
        pingReady = true;

        cvPing.notify_one();
    }
}

int main() {
    cout << "Ping-Pong (" << ROUNDS << " раундов)" << endl;

    pingReady = true;

    thread t1(pingThread);
    thread t2(pongThread);

    t1.join();
    t2.join();

    return 0;
}