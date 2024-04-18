#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// FB scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* job_list;
    uint64_t current_job_start_time;
    uint64_t unaccounted_time;
} scheduler_FB_t;

int compare_remaining_job_size_FB(void* data1, void* data2) {
    job_t* job1 = (job_t*)data1;
    job_t* job2 = (job_t*)data2;
    if (jobGetRemainingTime(job1) < jobGetRemainingTime(job2)) {
        return -1;
    }
    else if (jobGetRemainingTime(job1) == jobGetRemainingTime(job2)) {
        if (jobGetId(job1) < jobGetId(job2)) {
            return -1;
        }
        else {
            return 1;
        }
    }
    else {
        return 1;
    }
}


// Creates and returns scheduler specific info
void* schedulerFBCreate()
{
    scheduler_FB_t* info = malloc(sizeof(scheduler_FB_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->job_list = list_create(compare_remaining_job_size_FB);
    info->current_job_start_time = 0;
    info->unaccounted_time = 0;
    return info;
}

// Destroys scheduler specific info
void schedulerFBDestroy(void* schedulerInfo)
{
    scheduler_FB_t* info = (scheduler_FB_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->job_list);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerFBScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_FB_t* info = (scheduler_FB_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    if(list_count(info->job_list) != 0){
        uint64_t elapsed_time = (currentTime - info->current_job_start_time) + info->unaccounted_time;
        uint64_t work_done_per_job = elapsed_time / list_count(info->job_list);
        info->unaccounted_time = elapsed_time % list_count(info->job_list);
        for(list_node_t* node = list_head(info->job_list); node != list_end(info->job_list); node = list_next(node)){
            job_t* node_job = list_data(node);
            jobSetRemainingTime(node_job, jobGetRemainingTime(node_job) - work_done_per_job);
        }
        list_insert(info->job_list, job);
        schedulerCancelNextCompletion(scheduler);
        job_t* smallest_RemainingTime_job = list_data(list_head(info->job_list));
        info->current_job_start_time = currentTime;

        if(jobGetRemainingTime(smallest_RemainingTime_job) <= 0){
            schedulerScheduleNextCompletion(scheduler, currentTime);
        }
        else{
            schedulerScheduleNextCompletion(scheduler, currentTime + ((jobGetRemainingTime(smallest_RemainingTime_job) * list_count(info->job_list)) - info->unaccounted_time));
        }
    }
    else{
        list_insert(info->job_list, job);
        info->current_job_start_time = currentTime;
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(job));
    }
    
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerFBCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_FB_t* info = (scheduler_FB_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    job_t* job = list_data(list_head(info->job_list));
    list_remove(info->job_list, list_head(info->job_list));
    if (jobGetRemainingTime(job) > 0){
        info->unaccounted_time = 0;
    }

    for(list_node_t* node = list_head(info->job_list); node != list_end(info->job_list); node = list_next(node)){
        job_t* node_job = list_data(node);
        jobSetRemainingTime(node_job, jobGetRemainingTime(node_job) - jobGetRemainingTime(job));
    }
    if(list_count(info->job_list) != 0){
        job_t* smallest_RemainingTime_job = list_data(list_head(info->job_list));
        info->current_job_start_time = currentTime;
        schedulerScheduleNextCompletion(scheduler, currentTime + ((jobGetRemainingTime(smallest_RemainingTime_job) * list_count(info->job_list))-info->unaccounted_time));
    }

    return job;
}
