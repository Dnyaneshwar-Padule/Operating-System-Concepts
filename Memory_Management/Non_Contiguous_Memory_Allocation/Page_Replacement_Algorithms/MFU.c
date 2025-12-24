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
int page_fault;
int count[20];


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


// search a page in current frame
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

int find(int page_no, int temp_frame[])
{
    for (int i = 0; i < frame_size; i++)
    {
        if (page_no == temp_frame[i])
        {
            return i;
        }
    }
    
    return NOT_FOUND;
}

void set_count(int r)
{
    for (int i = 0; i < frame_size; i++)
    {
        count[i] = 0;
    }

    for (int i, k = r- 1; k >= 0; k--)
    {
        i = search(ref[k]);
        
        if (i != NOT_FOUND)
        {
            count[i]++;
        }
        
    }

}

int get_max()
{
    int max = count[0];
    for (int i = 0; i < frame_size; i++)
    {
        if (max < count[i])
        {
            max = count[i];
        }
    }

    return max;
}

int get_pos(int r)
{
    int pos, temp_pos, max, temp_frame[20];

    for (int i = 0; i < frame_size; i++)
    {
        if (frame[i] == -1)
        {
            return i;
        }

        temp_frame[i] = frame[i];
    }

    set_count(r);
    max = get_max();

    for (int i = 0; i < frame_size; i++)
    {
        if ( count[i] != max )
        {
            temp_frame[i] = -1;
        }
        
    }
    
    for (int k = r - 1 ; k >= 0; k--)
    {
        temp_pos = find(ref[k], temp_frame);
        
        if (temp_pos != -1)
        {
            pos = temp_pos;
            temp_frame[temp_pos] = -1;
        }
        
    }
    
    return pos;
}

void mfu()
{
    int found, pos;
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
    mfu();
    display();
    return 0;
}