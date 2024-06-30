#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initialize ( int *frames, int frames_num ) {
    for ( int *i = frames; i < frames + frames_num; ++i )
        *i = -1;
}

int in ( int *frames, int frames_num, int key ) {
    for ( int *i = frames; i < frames + frames_num; ++i )
        if ( *i == key )
            return 1;
    return 0;
}

int fifo ( int *pages, int pages_num, int frames_num, int **memory_state, char *page_fault_map ){
    int frames[frames_num],
        page_fault = 0,
        *flag = (int*) calloc ( frames_num, sizeof(int) );
    initialize( frames, frames_num );
    for ( int i = 0, k = 0; i < pages_num; ++i ){
        if ( !in( frames, frames_num, pages[i] ) ) {
            frames[k++] = pages[i];
            flag[k]++;
            page_fault++;
            page_fault_map[i] = 'x';
        }
        
        for ( int j = 0; j < frames_num; ++j )
            memory_state[j][i] = frames[j];
        
        if ( k >= frames_num )
            k = 0;
    }
    return page_fault;
}

void StrToInt ( int *pages, int len, char *buffer ){
    for ( int i = 0; i < len; ++i )
        pages[i] = buffer[i] - '0';
}

void memdisplay ( int *pages, int pages_num, int frames_num, int **memory_state, char *page_fault_map ){
    printf("%8c", '|');
    for( int *i = pages; i < pages + pages_num; ++i )
        printf(" %d |", *i);
    puts("");
    
    for( int i = 0; i < frames_num; ++i ){
        printf("page %d %c", i, '|');
        for( int j = 0; j < pages_num; ++j )
            if ( memory_state[i][j] > -1 )
                printf(" %d |", memory_state[i][j]);
            else
                printf("   |");
        puts("");    
    }
    printf("PF %5c", '|');
    for( char *i = page_fault_map; i < page_fault_map + pages_num; ++i )
        if( *i )
            printf(" %1c |", *i);
        else
            printf(" %1c |", ' ');
}

int main() {
    int *pages,
        pages_num,
        frames_num,
        **memory_state,
        memory_size,
        page_size;
    char *buffer,
         *page_fault_map;
    long temp = 0;
    
    printf(" enter the reference string (sequence of page numbers): ");
    getline(&buffer, &temp, stdin);
    
    pages_num = strlen(buffer)-1;
    pages = (int*) malloc (pages_num * sizeof(int));
    page_fault_map = (char*) calloc (pages_num, sizeof(char));
    
    printf(" enter memory size: ");
    scanf("%d", &memory_size);
    
    printf(" enter page size: ");
    scanf("%d", &page_size);
    
    frames_num = memory_size / page_size;
    
    memory_state = (int**) malloc (frames_num * sizeof(int*));
    for( int **i = memory_state; i < memory_state + frames_num; ++i )
        *i = (int*) malloc (pages_num * sizeof(int));
    
    StrToInt( pages, pages_num, buffer );
    
    printf(" FIFO page fault is %d\n", fifo( pages, pages_num, frames_num, memory_state, page_fault_map ));
    
    memdisplay( pages, pages_num, frames_num, memory_state, page_fault_map );
}
