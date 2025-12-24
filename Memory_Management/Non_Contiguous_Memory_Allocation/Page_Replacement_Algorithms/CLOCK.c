#include<stdio.h>

#define NOT_FOUND -1
#define PAGE_HIT 1
#define PAGE_MISS 2

int no_of_ref; // stores no of references
int ref[20];  // stores actual references
int frame_size; // stores size of frame
int frame[20];  // works as current frame
int ref_bit[20]; // stores reference bits
int frame_status[20]; // stores status for every page (PAGE_HIT or PAGE_MISS)
int table[20][20];  // stores the past frames  
int hand = 0;         // works as clock hand
int page_fault;   // count of page fault



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
        ref_bit[i] = 0;
        frame[i] = -1;
    }
    
    
}


// searches a page in current frame
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


// frame position for a page
void move_hand()
{

    for (int i = 0; i < frame_size; i++)
    {
        // -1 means the frame location is empty
        if ( frame[i] == -1 )
        {
            hand = i;
            return;
        }
    }

    // now, rotate the hand for frame
    while (1)
    {
        if (ref_bit[hand] == 0)
        {
            return;
        }

        ref_bit[hand] = 0;
        hand = (hand + 1) % frame_size;
    }
}

void clock()
{
    int pos;
    page_fault = 0;

    for (int i = 0; i < no_of_ref; i++)
    {
        pos = search(ref[i]);

        // page miss
        if ( pos == NOT_FOUND )
        {
            page_fault++;
            frame_status[i] = PAGE_MISS;
            move_hand(); 
            frame[hand] = ref[i];
            ref_bit[hand] = 1;
        }
        else
        {
            frame_status[i] = PAGE_HIT;
            ref_bit[pos] = 1;
        }

        // save current frame in table
        for (int k = 0; k < frame_size; k++)
        {
            table[k][i] = frame[k];
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
    clock();
    display();
    return 0;
}