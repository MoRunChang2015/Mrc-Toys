#include <iostream>
#include <chrono>
#include <vector>
#include <future>
#include "threadPool.hpp"

using namespace std;

int func(int x) {
    cout << "Testing" << endl;
    this_thread::sleep_for(chrono::seconds(x));
    return x * x;
}

int main() {
    vector<future<int>> f;
    ThreadPool pool(5);
    for (int i = 0; i < 10; ++i) {
        f.push_back(pool.add_job(func, i));
    }
    for (int i = 0; i < 10; ++i) {
        cout << f[i].get() << " ";
    }
    cout << endl;
    return 0;
}
