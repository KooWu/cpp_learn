#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include "MWPLog.h"

// 定义一个结构体来传递参数
typedef struct {
    int count;
    int sleepTime;
} ThreadParams;

// 线程函数
void* logFunction(void* args) {
    ThreadParams* params = (ThreadParams*)args;
    int ret = 32;
    pthread_t id = pthread_self();
    for (int i = 0; i <= params->count; i++) {
        MWP_TRACE("Thread %lu: count: %d, ret: %d, %s, %f", id, i, ret, "test", 3.1415926)
        MWP_DEBUG("Thread %lu: count: %d, ret: %d, %s, %f", id, i, ret, "test", 3.1415926);
        MWP_INFO("Thread %lu: count: %d, ret: %d, %s, %f", id, i, ret, "test", 3.1415926);
        MWP_WARN("Thread %lu: count: %d, ret: %d, %s, %f", id, i, ret, "test", 3.1415926);
        char buf[999] = "11234";
        MWP_WARN("Thread %lu: buf: %s", id, buf);

        char buf1[1056];
        memset(buf1, '1', sizeof(buf1) - 2);
        buf1[sizeof(buf1) - 1] = '\0';
        MWP_WARN("Thread %lu: buf1: %s", id, buf1);
        usleep(params->sleepTime * 1000);
    }
    printf("Thread %lu: Exiting\n", id);
    return NULL;
}

// 创建线程的C接口函数
void createAndRunThreads(int count, int sleepTime, int threadCount) {
    pthread_t threads[threadCount];

    ThreadParams params;
    params.count = count;
    params.sleepTime = sleepTime;
    
    for (int i = 0; i < threadCount; i++) {
        int result = pthread_create(&threads[i], NULL, logFunction, &params);
        if (result != 0) {
            fprintf(stderr, "线程创建失败\n");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < threadCount; i++) {
        printf("Waiting for thread %d to finish\n", i);
        pthread_join(threads[i], NULL);
    }
}

void SignalHandlerLogLevel(int signalNum) {
    printf("Caught signal %d\n", signalNum);
    if (signalNum == SIGUSR1) {
    } else if (signalNum == SIGUSR2) {
    }
}

int main(int argc, char* argv[]) {
    struct sigaction sa;

    sa.sa_handler = &SignalHandlerLogLevel;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    // Register the signal handler for SIGUSR1 using sigaction
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    const char* appName = "test_log_app";
    if (argc > 1) {
        appName = argv[1];
    }

    int count = 0;
    if (argc > 2) {
        count = atoi(argv[2]);
    }

    int sleepTime = 1000;
    if (argc > 3) {
        sleepTime = atoi(argv[3]);
        if (sleepTime < 0 || sleepTime > 10 * 1000) {
            sleepTime = 100;
        }
    }
    log_system_init(appName);
    const int threadCount = 5; // 创建5个线程
    createAndRunThreads(count, sleepTime, threadCount);

    return 0;
}
