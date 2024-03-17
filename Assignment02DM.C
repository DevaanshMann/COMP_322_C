// Devaansh Mann
// COMP 322/L
// Assignment 2
// March 11, 2024

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int arrival;
    int total_cpu;
    int total_remaining;
    int done;
    int start_time;
    int already_started;
    int end_time;
    int turnaround_time;
} Process;

Process *processes = NULL;
int num_processes = 0;

void enterParameters() {
    printf("Enter total number of processes: ");
    scanf("%d", &num_processes);
  processes = (Process *)malloc(num_processes * sizeof(Process));

    for (int i = 0; i < num_processes; i++) {
    processes[i].id = i;
    printf("Enter arrival cycle for process %d: ", i);
    scanf("%d", &processes[i].arrival);
    printf("Enter total cycles for process %d: ", i);
    scanf("%d", &processes[i].total_cpu);
    processes[i].total_remaining = processes[i].total_cpu;
    processes[i].done = 0;
    processes[i].already_started = 0;
    }
}

void printTable() {
    printf("PID Arrival Total  Start   End Turnaround\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%d      %d      %d      ", processes[i].id, processes[i].arrival, processes[i].total_cpu);
        if (processes[i].done) {
            printf("%d      %d      %d\n", processes[i].start_time, processes[i].end_time, processes[i].turnaround_time);
        } else {
            printf("\n");
        }
    }
}

//  scheduleFIFO algorithm
void scheduleFIFO() {
    int currentTime = 0;
    for (int i = 0; i < num_processes; i++) {
        if (currentTime < processes[i].arrival) {
        currentTime = processes[i].arrival;
        }
    
        processes[i].start_time = currentTime;
        currentTime += processes[i].total_cpu;
        processes[i].end_time = currentTime;
        processes[i].turnaround_time = processes[i].end_time - processes[i].arrival;
        processes[i].done = 1;
    }
}

// scheduleSJF algorithm
void scheduleSJF() {
    int completed[num_processes];
    for (int i = 0; i < num_processes; i++) {
        completed[i] = 0;
    }

    int currentTime = 0, totalCompleted = 0;

    while (totalCompleted < num_processes) {
        int shortestJobIndex = -1;
        int shortestTime = INT_MAX;

        for (int i = 0; i < num_processes; i++) {
            if (!completed[i] && processes[i].arrival <= currentTime && processes[i].total_cpu < shortestTime) {
                shortestJobIndex = i;
                shortestTime = processes[i].total_cpu;
            }
        }

        if (shortestJobIndex == -1) {
            currentTime++;
            continue;
        }

        Process *p = &processes[shortestJobIndex];
        p->start_time = currentTime;
        currentTime += p->total_cpu;
        p->end_time = currentTime;
        p->turnaround_time = p->end_time - p->arrival;
        completed[shortestJobIndex] = 1;
        totalCompleted++;
    }
}

// scheduleSRT algorithm
void scheduleSRT() {
    for (int i = 0; i < num_processes; i++) {
    processes[i].done = 0;
    processes[i].total_remaining = processes[i].total_cpu;
    processes[i].already_started = 0;
    }

    int currentTime = 0, completedProcesses = 0;
    while (completedProcesses < num_processes) {
    int idx = -1;
    int shortestTime = INT_MAX;
    for (int i = 0; i < num_processes; i++) {
        if (!processes[i].done && processes[i].arrival <= currentTime &&
            processes[i].total_remaining < shortestTime) {
        shortestTime = processes[i].total_remaining;
        idx = i;
        }
    }

if (idx != -1) {
    if (!processes[idx].already_started) {
        processes[idx].start_time = currentTime;
        processes[idx].already_started = 1;
    }

    processes[idx].total_remaining--;
    currentTime++;

    if (processes[idx].total_remaining == 0) {
        processes[idx].done = 1;
        processes[idx].end_time = currentTime;
        processes[idx].turnaround_time = processes[idx].end_time - processes[idx].arrival;
        completedProcesses++;
        }
        } else {
        currentTime++;
        }
    }
}

// Quitting and freeing memory
void quitAndFreeMemory() {
    if (processes != NULL) {
        free(processes);
    }
    printf("Quitting program...\n");
}

// Main class
int main() {
    int choice;
    do {
    printf("Batch scheduling\n----------------\n");
    printf("1) Enter parameters\n");
    printf("2) Schedule processes with FIFO algorithm\n");
    printf("3) Schedule processes with SJF algorithm\n");
    printf("4) Schedule processes with SRT algorithm\n");
    printf("5) Quit and free memory\n");
    printf("Enter selection: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
        enterParameters();
        printTable();
        break;
    case 2:
        scheduleFIFO();
        printTable();
        break;
    case 3:
        scheduleSJF();
        printTable();
        break;
    case 4:
        scheduleSRT();
        printTable();
        break;

    case 5:
        quitAndFreeMemory();
        break;

    default:
        printf("Invalid selection. Please enter a number between 1 and 5.\n");
    }
    } while (choice != 5);

    return 0;
}
