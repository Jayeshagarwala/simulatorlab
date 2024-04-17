#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// SRPT scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* job_list;
    job_t* current_job;
    uint64_t current_job_start_time;
} scheduler_SRPT_t;

int compare_remaining_job_size(void* data1, void* data2) {
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
void* schedulerSRPTCreate()
{
    scheduler_SRPT_t* info = malloc(sizeof(scheduler_SRPT_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->job_list = list_create(compare_remaining_job_size);
    info->current_job = NULL;
    info->current_job_start_time = 0;
    return info;
}

// Destroys scheduler specific info
void schedulerSRPTDestroy(void* schedulerInfo)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->job_list);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerSRPTScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
    /* IMPLEMENT THIS */

    if(info->current_job != NULL){
        uint64_t remaining_time_current_job = jobGetRemainingTime(info->current_job)-(currentTime - info->current_job_start_time);
        if (remaining_time_current_job > jobGetRemainingTime(job)){
            jobSetRemainingTime(info->current_job, remaining_time_current_job);
            list_insert(info->job_list, info->current_job);
            info->current_job = job;
            info->current_job_start_time = currentTime;
            schedulerCancelNextCompletion(scheduler);
            schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(job));
        }
        else{
            list_insert(info->job_list, job);
        }
    }
    else{
        info->current_job = job;
        info->current_job_start_time = currentTime;
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(job));
    }
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerSRPTCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_SRPT_t* info = (scheduler_SRPT_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    job_t* job = info->current_job;
    
    if (list_count(info->job_list) > 0) {
        job_t* next_job = list_data(list_head(info->job_list));
        info->current_job = next_job;
        info->current_job_start_time = currentTime;
        list_remove(info->job_list, list_find(info->job_list, next_job));
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(next_job));
    }
    else {
        info->current_job = NULL;
    }

    return job;
}
