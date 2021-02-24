//
// Created by etien on 22/02/2021.
//

#include "JobDispatcher.h"

std::mutex JobDispatcher::queue_mutex;
std::vector<std::thread> JobDispatcher::threads;
ThreadSafeRingBuffer<JobDispatcher::Job, 2048> JobDispatcher::jobs;
bool JobDispatcher::running = true;

void JobDispatcher::thread_function() {
    while (running) {

        Job job;
        if (jobs.pop_front(job)) { // try to grab a job from the jobPool queue

            job(); // execute job

        }
        else {
            // no job, put thread to sleep
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

    }
}

void JobDispatcher::init() {
    unsigned int coreNumber = std::thread::hardware_concurrency();
    std::cout << "Initializing job dispatcher for " << coreNumber << " cores" << std::endl;
    for(int i = 0; i < coreNumber; i++)
        threads.emplace_back(thread_function);
}

void JobDispatcher::stop() {
    running = false;
    for(auto& t : threads)
        t.join();
}

void JobDispatcher::addJob(const JobDispatcher::Job &job) {
    jobs.push_back(job);
}

bool JobDispatcher::hasJobs() {
    return !jobs.empty();
}

void JobDispatcher::clearJobs() {
    jobs.clear();
}
