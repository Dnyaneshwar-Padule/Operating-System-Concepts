#include<stdio.h>

#define NOT_FOUND -1
#define PAGE_HIT 1
#define PAGE_MISS 2

int no_of_ref;
int ref[20];
int frame_size;
int frame[20];
int table[20][20];
int page_fault;
int frame_status[20];


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

int search(int page_no)
{
    for (int i = 0; i < frame_size; i++)
    {
        if (frame[i] == page_no)
        {
            return i;
        }
    }
    return NOT_FOUND;
}

int get_pos(int r)
{

    for (int i = 0; i < frame_size; i++)
    {
        if ( frame[i] == -1 )
        {
            return i;
        }
    }
    
    return search( ref[r-1] );
}

void mru()
{
    int pos, found;
    page_fault = 0;

    for (int r = 0; r < no_of_ref; r++)
    {
        found = search(ref[r]);
        
        // if not found, then it's a page Miss, so we have to update the frame
        if ( found == NOT_FOUND )
        {
            page_fault++;
            frame_status[r] = PAGE_MISS;
            
            pos = get_pos(r);
            frame[pos] = ref[r];
        }
        else
        {
            frame_status[r] = PAGE_HIT;
        }

        //Save the current frame
        for (int i = 0; i < frame_size; i++)
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
    mru();
    display();
    return 0;
}