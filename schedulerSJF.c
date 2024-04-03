#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// SJF scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* job_list;
    job_t* scheduled_job;
} scheduler_SJF_t;

int compare_job_size(void* data1, void* data2) {
    job_t* job1 = (job_t*)data1;
    job_t* job2 = (job_t*)data2;
    if (jobGetJobTime(job1) < jobGetJobTime(job2)) {
        return -1;
    }
    else if (jobGetJobTime(job1) == jobGetJobTime(job2)) {
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
void* schedulerSJFCreate()
{
    scheduler_SJF_t* info = malloc(sizeof(scheduler_SJF_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->job_list = list_create(compare_job_size);
    info->scheduled_job = NULL;
    return info;
}

// Destroys scheduler specific info
void schedulerSJFDestroy(void* schedulerInfo)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->job_list);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerSJFScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_insert(info->job_list, job);
    if (list_count(info->job_list) == 1) {
        info->scheduled_job = job;
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetJobTime(job));
    }

}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerSJFCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_SJF_t* info = (scheduler_SJF_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    job_t* job = info->scheduled_job;
    list_remove(info->job_list, list_find(info->job_list, job));
    
    if (list_count(info->job_list) > 0) {
        job_t* next_job = list_data(list_head(info->job_list));
        info->scheduled_job = next_job;
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetJobTime(next_job));
    }
    else {
        info->scheduled_job = NULL;
    }

    return job;
}
