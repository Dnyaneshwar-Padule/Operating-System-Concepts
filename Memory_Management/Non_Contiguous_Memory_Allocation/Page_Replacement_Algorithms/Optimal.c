#include<stdio.h>

#define NOT_FOUND -1
#define PAGE_HIT 1
#define PAGE_MISS 2

int no_of_ref;
int ref[20];
int frame_size;
int frame[20];
int table[20][20];
int frame_status[20];
int page_fault = 0;


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

//Search a page reference in current frame
int search(int page_no)
{
    for (int i = 0; i < frame_size; i++)
    {
        if (page_no == frame[i])
        {
            return i;
        }
        
    }
    
    return NOT_FOUND;
}


// Returns the position of frame to store current page reference
int get_pos(int current_ref_index)
{
    int cnt, temp_frame[20], temp_pos;
    cnt = 0;

    // Copy frame in
    for (int i = 0; i < frame_size; i++)
    {
        // -1 means the frame is empty
        if ( frame[i] == -1 )
        {
            return i;
        }
        temp_frame[i] = frame[i];
    }

    // See forward for Optimal Page Replacement
    for (int k = current_ref_index + 1 ; k < no_of_ref ; k++)
    {
        temp_pos = search( ref[k] );
     
        if ( temp_pos != NOT_FOUND )
        {
            if ( temp_frame[temp_pos] != -1 )
            {
                cnt++;
            }

            temp_frame[temp_pos] = -1;

            if ( cnt == frame_size )
            {
                return temp_pos;
            }
            
        }
        
    }

    // If we don't get location in forward, see backward
    for (int k = current_ref_index - 1; k >= 0; k--)
    {
        // search the ref in main frame and get the index
        temp_pos = search(ref[k]);

        if (temp_pos != NOT_FOUND)
        {

            // Check if, the page reference in the temp_frame is already found 
            // if not found, increment the cnt
            if ( temp_frame[temp_pos] != -1)
            {
                cnt++;
            }

            temp_frame[temp_pos] = -1; // indicates it's found

            if ( cnt == frame_size )
            {
                return temp_pos;
            }
            
        }
        
    }
    
}

void optimal()
{
    int pos, found;
    page_fault = 0;

    for (int r = 0; r < no_of_ref; r++)
    {
        found = search(ref[r]);

        if (found == NOT_FOUND)
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

        for (int i = 0; i < frame_size; i++)
        {
            printf("%d\n", ref[r]);
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
    optimal();
    display();
    return 0;
}