#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TRUE 1
#define FALSE 0

//Structure to store process information
typedef struct 
{
    char name[12];        // Process name
    int arriving_time;   // process Arriving time
    int burst_time;      //Process burst time
    int temp_burst_time;  // Reamining burst time
    int priority;         // Priority of the process
    int completion_time;  //Completion time
    int turnaround_time;  // Turnaround time
    int waiting_time;     // waiting time
}process;


//Structure to store gantt chart info
typedef struct 
{
    int start_time;
    char p_name[20];
    int end_time;
}gantt_chart;

process processes[10];  // process array, which stores process information (Max 10 processes)
int n = 0;              // number of processes

gantt_chart g[50];  //array to store gantt chart entries
int g_cnt = 0;      // gantt chart array length

int total_tat = 0;   // total turn around time
int total_wt = 0;    // total waiting time

// This function accept process info
void accept()
{
    printf("How many Processes you have : ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++)
    {
        printf("\nAccepting information for process : %d\n", i);
        printf("Enter process  name : ");
        scanf("%s", processes[i].name);

        printf("Enter arriving time : ");
        scanf("%d", &processes[i].arriving_time);
        
        printf("Enter  burst  time  : ");
        scanf("%d", &processes[i].burst_time);
        
        //temperory burst time is like remaining burst time,
        // it will be used for calculated instead of actual burst time (although it is actual burst time)
        processes[i].temp_burst_time = processes[i].burst_time;
        
        printf("Enter the  priority : ");
        scanf("%d", &processes[i].priority);
    }
    
}

// This function checks, if all processes completed their execution 
int is_done()
{
    for (int i = 0; i < n; i++)
    {
        if (processes[i].temp_burst_time)
        {
            return FALSE;
        }
    }
    return TRUE;
}


//This function sorts processes by their Arriving time
void sort_at()
{
    process temp;

    for (int i = n - 1; i > 0; i--)
    {
        for (int j = 0; j < i ; j++)
        {
            if (  processes[i].arriving_time < processes[j].arriving_time )
            {
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;    
            }
            
        }
        
    }
    
}

//This function gives a process, which is available at current time
// and has highest priority
//This algorithm assumes, lower the number higher the priority
int get_arrived(int time)
{
    int pos = -1;
    int smallest = __INT32_MAX__;

    for (int i = 0; i < n; i++)
    {
        //Processes are already sorted by the arriving time
        // If process is available/arrived and does not completed it's execution
        if ( processes[i].arriving_time <= time && processes[i].temp_burst_time != 0 )
        {
            // If the process priority is greater than the priority of previous process which also available for execution
            //select the one with higher priority
            if (smallest > processes[i].priority)
            {
                pos = i;   // get the position of that higher priority process
                smallest = processes[i].priority;  // and also store it's priority
            }   
        }
    }
    return pos;
}


//This function performs pre-emptive priority scheduling 
void priority_preemptive()
{
    int time = 0; // initial time, starts from 0
    int i;

    //schedule until, all processes does not complete their execution
    while ( ! is_done() )
    {
        // Choose a process which is arrived as well as has highest priority
        int i = get_arrived(time);

        if ( i != -1)
        {
            // create a entry in the gantt chart
            g[g_cnt].start_time = time;   // Starting time is current time
            strcpy(g[g_cnt].p_name, processes[i].name);  // get the process name
            
            //This is pre-emptive scheduling so we execute a process just for 1 unit of time
            // and check if there is any high priority process
            g[g_cnt].end_time = time + 1;
            time = time + 1;
            processes[i].temp_burst_time = processes[i].temp_burst_time -  1;

            g_cnt++; // update gantt chart length
        }
        else
        {
            g[g_cnt].start_time = time;
            strcpy(g[g_cnt].p_name, "CPU-IDLE");
            g[g_cnt].end_time = ++time;
            g_cnt++;
        }
        
    }
    
}

//Gives the completion time of a process
int get_ct(const char* p_name)
{
    /*
        Oberved ?
        The loop is iterating reverse, and it makes sense, 
        as the last entry of a process in the gantt chart has the completion time of that process
    */
    for (int i = g_cnt - 1; i >= 0; i--)
    {
        if ( strcmp(p_name, g[i].p_name) == 0 )
        {
            return g[i].end_time;
        }
    }
    return 0;
}

//Calculate total Turnaround time and total waiting time
// as well as TAT and Wt for individual process
void calculate()
{
    total_tat = 0;
    total_wt = 0;

    for (int i = 0; i < n; i++)
    {
        processes[i].completion_time = get_ct(processes[i].name);
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arriving_time; // TAT = CT - AT
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;       // WT = TAT - BT

        total_tat += processes[i].turnaround_time;
        total_wt += processes[i].waiting_time;
    }
}


/*
    This function isn't much important  
    We execute a process only one unit of time at a time
    So, in gantt chart, it creates multiple entries of a same process one after another
    This function, merges those multiple entries and create only one entry 

    for example:
        Suppose, in gantt chart there are entries like this
       ST    ET   (Start time    End time)
        0 P1 1
        1 P1 2
        2 P1 3
        3 P1 4

    So this function removes these types of entries and merges it
      ST    ET
       0 P1 4  <--- Only one entry, so that we can understand clearly

*/
void filter_gantt_chart()
{
    int i, j;

    for(i = 0, j = 1; j < g_cnt; j++)
    {
        if ( strcmp(g[i].p_name, g[j].p_name) )
        {
            i++;
            g[i] = g[j];
        }
        else
        {
            g[i].end_time = g[j].end_time;
        }
    }

    g_cnt = i + 1;
}


void display()
{

    //Display Process information
    printf("\nProcess Information...\n");
    printf("+--------------+----+----+----+-----+----+\n");
    printf("| Process name | AT | BT | CT | TAT | WT |\n");
    printf("+--------------+----+----+----+-----+----+\n");
    
    for (int i = 0; i < n; i++)
    {
        printf("| %-12s | %-2d | %-2d | %-2d | %-3d | %-2d |\n", processes[i].name, processes[i].arriving_time, processes[i].burst_time, processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("+--------------+----+----+----+-----+----+\n");

    printf("\nGantt Chart...\n");
    printf("+------------+--------------+----------+\n");
    printf("| Start Time | Process Name | End Time |\n");
    printf("+------------+--------------+----------+\n");
    
    for (int i = 0; i < g_cnt; i++)
    {
        printf("| %-10d | %-12s | %-8d |\n", g[i].start_time, g[i].p_name, g[i].end_time);
    }
    
    printf("+------------+--------------+----------+\n");

    printf("\nTotal Turnaround Time : %d\n", total_tat);
    printf("Total Waiting Time : %d\n", total_wt);

    printf("\nAverage Turnaround Time : %.2f\n", (float)total_tat / (float)n );
    printf("Average Waiting Time : %.2f\n", (float) total_wt / (float) n);
}

void show_gantt_chart()
{
    printf("\n+");
    for (int i = 0; i < g_cnt; i++)
    {
        printf("----------+");
    }
    printf("\n|");

    for (int i = 0; i < g_cnt; i++)
    {
        printf(" %8s |", g[i].p_name);
    }
    printf("\n0");

    for (int i = 0; i < g_cnt; i++)
    {
        printf("---------%02d",g[i].end_time);
    }
    printf("\n");
}

int main()
{
    accept();
    sort_at();
    priority_preemptive();
    calculate();
    filter_gantt_chart();
    display();
    show_gantt_chart();
    return 0;
}