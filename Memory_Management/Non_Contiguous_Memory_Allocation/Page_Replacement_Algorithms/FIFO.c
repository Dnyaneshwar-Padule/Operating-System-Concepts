#include<stdio.h>

#define NOT_FOUND -1
#define PAGE_HIT 1
#define PAGE_MISS 2

int no_of_ref; // number of references
int ref[20];   // reference string
int frame_size;  // no of frames
int frame[20];   // frames
int table[20][20];   // to store snapshot of memory
int page_fault;  // to count page fault
int frame_status[20]; // To store Page Miss or Page Hit for every frame

void accept()
{
    printf("How many references you have : ");
    scanf("%d", &no_of_ref);

    printf("\nEnter the page references :\n");
    for (int i = 0; i < no_of_ref; i++)
    {
        printf("Enter page reference : ");
        scanf("%d", &ref[i]);
    }

    printf("\nHow many frames : ");
    scanf("%d", &frame_size);


    // Initilize all frames to -1
    for (int i = 0; i < frame_size; i++)
    {
        frame[i] = -1;
    }
}


/**
 * @brief This function searches a page reference in current frame
 */
int search(int page_no)
{
    for (int i = 0; i < frame_size; i++)
    {
        if ( frame[i] == page_no )
        {
            return i;
        }
    }
    
    return NOT_FOUND;
}


/**
 * @brief This function performs FIFO algorithm
 */
void fifo()
{
    int pos, r, found;

    pos = 0;
    page_fault = 0;

    for ( r = 0; r < no_of_ref; r++)
    {
        found = search(ref[r]);

        if ( found == NOT_FOUND )
        {
            page_fault++;
            frame[pos] = ref[r];
            pos = (pos + 1) % frame_size;
            frame_status[r] = PAGE_MISS;
        }
        else
        {
            frame_status[r] = PAGE_HIT;
        }

        // Save current frame 
        for ( int i = 0; i < frame_size; i++)
        {
            table[i][r] = frame[i];
        }
        
    }
    
}

void display()
{
    printf("\n\nPage references : ");
    for (int i = 0; i < no_of_ref; i++)
    {
        printf("%3d", ref[i]);
    }
    printf("\n");

    printf("Number of frames : %d\n", frame_size);

    printf("\nFrames\n+");
    for (int i = 0; i < no_of_ref; i++)
        printf("-----+");
        
    printf("\n|");
    for (int i = 0; i < no_of_ref; i++)
        printf(" %3d |", ref[i]);
    
        printf("\n+");
    for (int i = 0; i < no_of_ref; i++)
        printf("-----+");
    
    printf("\n");
    for (int i = 0; i < frame_size; i++)
    {
        printf("|");
        for (int j = 0; j < no_of_ref; j++)
        {
            if( table[i][j] != -1 )
            {
                printf(" %3d |", table[i][j]);
            }
            else
            {
                printf("     |");
            }
        }
        printf("\n");
    }

    printf("+");
    for (int i = 0; i < no_of_ref; i++)
        printf("-----+");

    printf("\n|");
    for (int i = 0; i < no_of_ref; i++)
    {
        if ( frame_status[i] == PAGE_HIT )
        {
            printf(" %3c |", 'H');
        }
        else
        {
            printf(" %3c |", 'M');
        }
    }

    printf("\n+");
    for (int i = 0; i < no_of_ref; i++)
        printf("-----+");

    printf("\n\n");
    printf("Page Fault : %d\n\n", page_fault);
}

int main()
{
    accept();
    fifo();
    display();
    return 0;
}

/*

    +----+----+----+----+----+----+----+----+----+----+----+----+
    |  1 |  2 |  3 |  4 |  5 |  5 |  4 |  6 |  7 |  2 |  4 |  3 |
    +----+----+----+----+----+----+----+----+----+----+----+----+
    |  1 |  1 |  1 |  4 |  4 |  4 |  4 |  4 |  7 |  7 |  7 |  3 |
    |    |  2 |  2 |  2 |  5 |  5 |  5 |  5 |  5 |  2 |  2 |  2 |
    |    |    |  3 |  3 |  3 |  3 |  3 |  6 |  6 |  6 |  4 |  4 |
    +----+----+----+----+----+----+----+----+----+----+----+----+
    |  T |  T |  T |  T |  T |  F |  F |  T |  T |  T |  T |  T |                     
    +----+----+----+----+----+----+----+----+----+----+----+----+
    

*/