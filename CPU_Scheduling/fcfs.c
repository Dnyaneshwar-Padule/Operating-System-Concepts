#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0

typedef struct 
{
    char name[16];      // process name
    int arriving_time;  // arriving time of the process
    int burst_time;     // burst time of the process
    int temp_burst_time;  // remaining burst time
    int completion_time;  // completion time
    int turnaround_time;  // turnaround time 
    int waiting_time;     // waiting time
}process;


typedef struct 
{
    int start_time;  // start time
    char p_name[32]; // process name
    int end_time;    // end time 
}gantt_chart;

process processes[10]; // This will store process info, (for MAX 10 processes) 
int n = 0;            // number of processes

gantt_chart g[50];     // This array will store the gantt chart
int g_cnt = 0;        // gantt_chart array length

int total_wt = 0;    // total waiting time
int total_tat = 0;  //total turnaround time

// Function to accept, process info
void accept()
{
    printf("How many processes you have (Max:10) : ");
    scanf("%d", &n);

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

//Sort process by their arriving time
void sort_AT()    // sort arriving time, STABLE SORT
{
    int i,j;
    process temp;

    for ( i = n-1; i > 0 ; i--)
    {
        for( j = 0; j < i ; j++ ){

            if(processes[i].arriving_time < processes[j].arriving_time){
                temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
    
}


// This function checks if, all processes has completed their execution 
// i.e. there no burst time remaining (temperory burst time)
int is_done()
{
    for (int i = 0; i < n; i++)
    {
        if ( processes[i].temp_burst_time  )
        {
            return FALSE;
        }
    }
    return TRUE;
}


//This function performs, FCFS 
void fcfs()
{
    int i,time;
    
    i = 0;
    time = 0;   // starting time....
    g_cnt = 0;  // starting gantt chart length

    //Calculate until, all processes complete execution
    while ( ! is_done() )
    {
        //Processes are already sorted by their arriving time....
        if( processes[i].arriving_time <= time && processes[i].temp_burst_time != 0 )
        {
            // add a new entry into gantt chart
            g[g_cnt].start_time = time;      // starting time for current entry, will be current time
            strcpy(g[g_cnt].p_name, processes[i].name);  // the current scheduled process name

            time = time + processes[i].temp_burst_time;  // Update current time
            processes[i].temp_burst_time = 0;   // This is FCFS, so once a process got CPU, it will fully execute....
        
            g[g_cnt].end_time = time;   // end for current entry

            i++;
            g_cnt++;  // update gantt chart length
        }
        else if( processes[i].temp_burst_time == 0 )
        {
            //In case, there is false process entry   
            i++;   
        }
        else
        {
            // There isn't any process arrived at current time, keep CPU-IDLE
            // Making any entry into the gantt chart for IDLE CPU
            g[g_cnt].start_time = time;
            strcpy(g[g_cnt].p_name, "CPU-IDLE");
            time++;
            g[g_cnt].end_time = time;

            g_cnt++;
        }
    }

}

//This function gives the completion time, of a process
int get_ct(const char* p_name)
{
    int j;
    int temp_ct = 0;

    /*
        Oberved ?
        The loop is iterating reverse, and it makes sense, 
        as the last entry of a process in the gantt chart has the completion time of that process
    */
    for ( j = g_cnt - 1; j >= 0; j--)
    {
        if(strcmp(p_name, g[j].p_name) == 0)
        {
            temp_ct = g[j].end_time; 
            break;
        }     
    }
    return temp_ct;
}


//This function calculates total turnaround time and total waiting time
// and also calculates TAT and WT for individual process
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
    printf("\n\033[32mProcess Information...\033[0m\n");
    printf("+--------------+----+----+----+-----+----+\n");
    printf("| Process name | AT | BT | CT | TAT | WT |\n");
    printf("+--------------+----+----+----+-----+----+\n");
    
    for (int i = 0; i < n; i++)
    {
        printf("| %-12s | %-2d | %-2d | %-2d | %-3d | %-2d |\n", processes[i].name, processes[i].arriving_time, processes[i].burst_time, processes[i].completion_time, processes[i].turnaround_time, processes[i].waiting_time);
    }
    printf("+--------------+----+----+----+-----+----+\n");

    printf("\n\033[32mGantt Chart...\033[0m\n");
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
        if(strcmp(g[i].p_name, "CPU-IDLE") == 0)
            printf("\033[32m");
        else
            printf("\033[33m");
            
        printf(" %8s \033[0m|", g[i].p_name);
    }
    printf("\n\033[36m0\033[0m");

    for (int i = 0; i < g_cnt; i++)
    {
        printf("---------\033[1;96m%02d\033[0m",g[i].end_time);
    }
    printf("\n");
}

int main()
{
    accept();
    sort_AT();
    fcfs();
    calculate();
    display();
    show_gantt_chart();
    return 0;
}


/*

    +--------------+----+----+----+-----+----+
    | Process name | AT | BT | CT | TAT | WT |
    +--------------+----+----+----+-----+----+
    |     P0       |  0 |  5 |  5 |  5  |  0 |
    +--------------+----+----+----+-----+----+


    +------------+--------------+----------+
    | Start Time | Process Name | End Time |
    +------------+--------------+----------+

    +----------+----------+----------+
    |       P0 |       P1 |       P2 |
    0---------05---------10---------07


*/

/*

void show()
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
        printf("---------%-02d",g[i].end_time);
    }
    printf("\n");
}
*/