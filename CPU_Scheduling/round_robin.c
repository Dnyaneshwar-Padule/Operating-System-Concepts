#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TRUE 1
#define FALSE 0

typedef struct 
{
    char name[16];
    int arriving_time;
    int burst_time;
    int temp_burst_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
}process;

typedef struct 
{
    int start_time;
    char p_name[16];
    int end_time;
}gantt_chart;

process processes[10];
int n = 0;

gantt_chart g[50];
int g_cnt = 0;

int time_slice;

int total_tat = 0;
int total_wt = 0;

void accept()
{
    printf("What is the time slice : ");
    scanf("%d", &time_slice);

    if (time_slice < 1)
    {
        printf("Invalid input !");
        exit(0);
    }

    printf("How many processes you have (Max:10) : ");
    scanf("%d", &n);

    if(n < 1)
    {
        printf("Invalid input !");
        exit(0);
    }

    for(int i = 0; i < n; i++)
    {
        printf("\nAccepting information for Process : %d\n", i);
        printf("Enter Process  name : ");
        //fgets(processes[i].name, sizeof(processes[i].name), stdin);
        scanf("%s", &processes[i].name); 

        printf("Enter arriving time : ");
        scanf("%d", &processes[i].arriving_time); 
        
        printf("Enter   burst  time : ");
        scanf("%d", &processes[i].burst_time);
        processes[i].temp_burst_time = processes[i].burst_time;
    }
}

void sort_AT()
{
    process temp;

    for (int i = n - 1; i > 0; i--)
    {
        for (int j = 0; j < i ; j++)
        {
            if( processes[i].arriving_time < processes[j].arriving_time){
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }   
    }   
}

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

void round_robin()
{
    int time = 0;
    int i = 0;
    g_cnt = 0;

    while ( ! is_done() )
    {
        if ( processes[i].arriving_time <= time && processes[i].temp_burst_time != 0 )
        {
            g[g_cnt].start_time = time;
            strcpy(g[g_cnt].p_name, processes[i].name);

            if ( processes[i].temp_burst_time > time_slice  )
            {
                g[g_cnt].end_time = time + time_slice;
                processes[i].temp_burst_time -= time_slice;
                time += time_slice; 
            }
            else
            {
                g[g_cnt].end_time = time + processes[i].temp_burst_time;
                time += processes[i].temp_burst_time;
                processes[i].temp_burst_time = 0;
            }

            i = (i + 1) % n;
            g_cnt++;
        }
        else if( processes[i].temp_burst_time == 0 )
        {
            i = (i + 1) % n;
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

int get_ct(const char* p_name)
{
    for (int i = g_cnt - 1; i >= 0; i--)
    {
        if( strcmp(p_name, g[i].p_name) == 0 )
        {
            return g[i].end_time;
        }
    }
    
    return 0;
}

void calculate()
{
    total_tat = 0;
    total_wt = 0;

    for (int i = 0; i < n; i++)
    {
        processes[i].completion_time = get_ct(processes[i].name);
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arriving_time;  // TAT = CT - AT
        processes[i].waiting_time =  processes[i].turnaround_time - processes[i].burst_time;    // WT = TAT - BT

        total_tat = total_tat + processes[i].turnaround_time;
        total_wt = total_wt + processes[i].waiting_time;

    }    
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
    sort_AT();
    round_robin();
    calculate();
    display();
    show_gantt_chart();
    return 0;
}