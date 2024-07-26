# Scheduling Policies Lab: Simulator in C

## Lab Description

In this lab, I simulated various scheduling policies within a discrete event simulation framework. I worked on implementing scheduling algorithms and a linked list to manage job queues. The focus was on event-driven programming, where events such as job arrivals and completions are handled at specific points in simulated time.

## Key Features

1. **Linked List Implementation**: Developed a linked list to support the scheduling policies, with support for a sorted mode based on a user-defined comparison function. The linked list operations were designed to be O(n), suitable for the requirements of this assignment.

2. **Scheduling Policies**: Implemented the following scheduling policies by modifying the corresponding scheduler files:

   - **First-Come, First-Served (FCFS)**
   - **Last-Come, First-Served (LCFS)**
   - **Shortest Job First (SJF)**
   - **Preemptive Last-Come, First-Served (PLCFS)**
   - **Preemptive Shortest Job First (PSJF)**
   - **Shortest Remaining Processing Time (SRPT)**
   - **Processor Sharing (PS)**
   - **Feedback (FB)**

   For each policy, I implemented the following functions:

   - **Create**: Initialized the scheduler info struct and added any necessary fields.
   - **Destroy**: Cleaned up and destroyed the scheduler info struct.
   - **ScheduleJob**: Handled new job arrivals and added them to the queue.
   - **CompleteJob**: Managed job completions and removed them from the queue.

3. **Event-Driven Programming**: Focused on handling discrete events (job arrivals and completions) within the simulator framework. The `ScheduleJob` function managed job arrivals, while the `CompleteJob` function dealt with job completions. The simulator framework ensured that these functions were called at the appropriate times.

4. **Completion Time Calculation**: Calculated the completion time for the next job under each scheduling policy based on the current job queue. Used `schedulerScheduleNextCompletion` and `schedulerCancelNextCompletion` functions to manage completion events.

5. **Preemption Handling**: Implemented preemption in relevant scheduling policies, allowing for the cancellation and rescheduling of completion events as needed.

## Files Modified

- **linked_list.c**: Implemented the core linked list functionality.
- **linked_list.h** (optional): Adjusted struct definitions and added additional functions as needed.
- **schedulerFCFS.c**: Implemented the FCFS scheduling policy.
- **schedulerLCFS.c**: Implemented the LCFS scheduling policy.
- **schedulerSJF.c**: Implemented the SJF scheduling policy.
- **schedulerPLCFS.c**: Implemented the PLCFS scheduling policy.
- **schedulerPSJF.c**: Implemented the PSJF scheduling policy.
- **schedulerSRPT.c**: Implemented the SRPT scheduling policy.
- **schedulerPS.c**: Implemented the PS scheduling policy.
- **schedulerFB.c**: Implemented the FB scheduling policy.

Through this lab, I developed a deep understanding of scheduling algorithms, event-driven programming, and the challenges of managing job queues in a discrete event simulation framework. This experience has enhanced my skills in systems programming and simulation.
