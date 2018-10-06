#include <iostream>
#include <map>
#include <functional>
#include <chrono>
#include <thread>
#include <unistd.h>
#include "../maprefresher.h"

void print_map(std::map<char, int> m) {
    printf("\n--------------");
    for(std::pair<char, int> p : m) {
        printf("\nx[%c]=%d", p.first, p.second);
    }
    printf("\n--------------");
}

int main() {
    std::map<char,int> x;

    x['a']=10;
    x['b']=30;
    x['c']=50;

    std::function<int(char, int)> f = [](char c, int i) {
        printf("\nx[%c]=%d", c, i*2);
        if (c == 'c')
            printf("\n");
        return i * 2;
    };
    refresher::map_refresher<char, int> mr(&x, f, 5000);
    mr.start();

    while (x['a'] < 100) {
        printf("\rwaiting...");
        fflush(stdout);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    mr.stop();
    print_map(x);
    printf("\n");
    printf("Waiting 10s to check if the refresher has really stopped");
    sleep(10);
    print_map(x);
    printf("\n");
    printf("Sync forced refreshing");
    mr.sync_refres_now();
    printf("\n");
}