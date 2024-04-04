#include <stdint.h>
#include <stdlib.h>
#include "scheduler.h"
#include "job.h"
#include "linked_list.h"

// PLCFS scheduler info
typedef struct {
    /* IMPLEMENT THIS */
    list_t* job_list;
    job_t* current_job;
    uint64_t current_job_start_time;
} scheduler_PLCFS_t;

// Creates and returns scheduler specific info
void* schedulerPLCFSCreate()
{
    scheduler_PLCFS_t* info = malloc(sizeof(scheduler_PLCFS_t));
    if (info == NULL) {
        return NULL;
    }
    /* IMPLEMENT THIS */
    info->job_list = list_create(NULL);
    info->current_job = NULL;
    info->current_job_start_time = 0;
    return info;
}

// Destroys scheduler specific info
void schedulerPLCFSDestroy(void* schedulerInfo)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    list_destroy(info->job_list);
    free(info);
}

// Called to schedule a new job in the queue
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// job - new job being added to the queue
// currentTime - the current simulated time
void schedulerPLCFSScheduleJob(void* schedulerInfo, scheduler_t* scheduler, job_t* job, uint64_t currentTime)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    
    if (info->current_job != NULL) {
        uint64_t remaining_time_current_job = jobGetRemainingTime(info->current_job)-(currentTime - info->current_job_start_time);
        if (remaining_time_current_job > 0) {
            list_insert(info->job_list, info->current_job);
            jobSetRemainingTime(info->current_job, remaining_time_current_job);
        }
    }
    
    list_insert(info->job_list, job);
    info->current_job = job;
    info->current_job_start_time = currentTime;
    schedulerCancelNextCompletion(scheduler);
    schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(job));
    
}

// Called to complete a job in response to an earlier call to schedulerScheduleNextCompletion
// schedulerInfo - scheduler specific info from create function
// scheduler - used to call schedulerScheduleNextCompletion and schedulerCancelNextCompletion
// currentTime - the current simulated time
// Returns the job that is being completed
job_t* schedulerPLCFSCompleteJob(void* schedulerInfo, scheduler_t* scheduler, uint64_t currentTime)
{
    scheduler_PLCFS_t* info = (scheduler_PLCFS_t*)schedulerInfo;
    /* IMPLEMENT THIS */
    job_t* job = info->current_job;
    list_remove(info->job_list, list_find(info->job_list, job));

    if (list_count(info->job_list) > 0) {
        job_t* next_job = list_data(list_head(info->job_list));
        info->current_job = next_job;
        info->current_job_start_time = currentTime;
        schedulerScheduleNextCompletion(scheduler, currentTime + jobGetRemainingTime(next_job));
    }
    else {
        info->current_job = NULL;
    }

    return job;
}
