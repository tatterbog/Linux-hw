#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <chrono>

class tdata {
public:
    pthread_t id;
    int* arr;
    long long start;
    long long end;
    long long res;
};

void* sum(void* arg){
    tdata* td = (tdata*)arg;
    td->res = 0;
    for(long long i = td->start; i < td->end; i++){
        td->res += td->arr[i];
    }
    return nullptr;
}

int main(int argc, char** argv)
{
    if(argc != 3){
        std::cerr << "Only N and M allowed\n";
        return 1;
    }

    long long N = atoll(argv[1]);
    int M = atoi(argv[2]);

    if (M <= 0 || N <= 0){
        std::cerr << "N and M must be positive\n";
        return 1;
    }

    int* arr = new int[N];
    for(long long i = 0; i < N; i++){
        arr[i] = rand();
    }


    auto start1 = std::chrono::high_resolution_clock::now();

    long long sum1 = 0;
    for(long long i = 0; i < N; i++){
        sum1 += arr[i];
    }

    auto end1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration1 = end1 - start1;



    long long chunk = N / M;
    tdata* tds = new tdata[M];
    
    auto start2 = std::chrono::high_resolution_clock::now();

    for(int i = 0; i < M; i++){
        tds[i].arr = arr;
        tds[i].start = i * chunk;
        tds[i].end = (i == M - 1) ? N : (i + 1) * chunk;
        tds[i].res = 0;

        if(pthread_create(&tds[i].id, nullptr, sum, &tds[i]) != 0){
            std::cerr << "Thread creation failed\n";
            delete[] tds;
            delete[] arr;
            return 1;
        }
    }

    long long sum2 = 0;
    for(int i = 0; i < M; i++){
        pthread_join(tds[i].id, nullptr);
        sum2 += tds[i].res;
    }

    auto end2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration2 = end2 - start2;

    std::cout << "Single-thread sum: " << sum1 
              << "  time: " << duration1.count() << '\n';

    std::cout << "Multi-thread sum:  " << sum2 
              << "  time: " << duration2.count() << '\n';

    delete[] tds;
    delete[] arr;
    return 0;
}


