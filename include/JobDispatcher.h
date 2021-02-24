//
// Created by etien on 22/02/2021.
//

#ifndef CLOTHSIM_JOBDISPATCHER_H
#define CLOTHSIM_JOBDISPATCHER_H

#include <vector>
#include <thread>
#include <functional>
#include <mutex>
#include <queue>
#include <iostream>

#include "ThreadSafeRingBuffer.h"

class JobDispatcher {

public:

    typedef std::function<void(void)> Job;

    static void init();
    static void stop();

    static void addJob(const Job& job);
    static bool hasJobs();

    static void clearJobs();

private:

    static std::mutex queue_mutex;
    static std::vector<std::thread> threads;
    static ThreadSafeRingBuffer<Job, 2048> jobs;
    static bool running;

    static Job nextJob();
    static void thread_function();



};


#endif //CLOTHSIM_JOBDISPATCHER_H
