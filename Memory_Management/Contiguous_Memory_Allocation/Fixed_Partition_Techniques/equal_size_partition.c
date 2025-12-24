#include<stdio.h>
#include<stdlib.h>

int memory_size;       // Size of entire memory
int total_partitions;  // No of all partitions
int partition_size;    // size of single partition
int processes[20];    // size of processes
int allocated_processes[20];  // it will store index of processes
int n = 0;   // no of processes
int an = 0;  // no of allocated processes
int internal_fragmentation = 0;

void accept()
{
    printf("\nWhat is the size of memory : ");
    scanf("%d", &memory_size);

    if (memory_size < 1)
    {
        printf("Invalid input !");
        exit(0);
    }
    
    printf("How many partitions : ");
    scanf("%d", &total_partitions);
    partition_size = memory_size / total_partitions;
    printf("\nSize of single partition : %d\n\n", partition_size);

    printf("How many processes (Max:20) : ");
    scanf("%d", &n);

    if (n > 20 || n < 1)
    {
        printf("Invalid input !");
        exit(0);
    }
    
    for (int i = 0; i < n; i++)
    {
        printf("Enter the size of Process %d : ", i);
        scanf("%d", &processes[i]);
    }
    
}

int is_allocated(int process_idx)
{
    for (int i = 0; i < an; i++)
    {
        if ( allocated_processes[i] == process_idx )
            return i;
    }

    return -1;
}

void calculate()
{
    int i;
    for ( i = 0; i < n; i++)
    {
        
        if (processes[i] <= partition_size){
            allocated_processes[an] = i;
            an++;
            internal_fragmentation += (partition_size) - processes[i];
        }

        //memory full
        if (an == total_partitions)
            break;
    }
    
}

void display()
{
    printf("\nMemory size :  %d\n", memory_size);
    printf("Total Partitions : %d\n", total_partitions);
    printf("Size of single partition : %d\n", partition_size);
    
    printf("\nProcesses who got memory\n");
    for (int i = 0; i < an; i++)
    {
        printf("Process : %d, Size : %d\n", allocated_processes[i], processes[allocated_processes[i]]);
    }
    
    printf("\nProcesses who don't get memory \n");
    for (int i = 0; i < n; i++)
    {
        if ( is_allocated(i) == -1)
        {
            printf("Process : %d, Size : %d\n", i, processes[i]);
        }
    }

    printf("\nInternal Fragmentation : %d\n", internal_fragmentation);
}

int main()
{
    accept();
    calculate();
    display();
    return 0;
}
