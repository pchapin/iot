
#include <stdio.h>
#include <stdlib.h>

int main( )
{
    FILE *input_file;
    char word_1[128];  // The size of these arrays is arbitrary, but plenty large enough for now.
    char word_2[128];
    char word_3[128];

    input_file = fopen( "/proc/version", "r" );
    if( input_file == NULL ) {
        printf( "Error: Unable to open /proc/version!\n" );
        return EXIT_FAILURE;
    }

    // TODO: We should verify that the input words are not too long! (security issue)
    fscanf( input_file, "%s%s%s", word_1, word_2, word_3 );
    printf( "%s\n", word_3 );

    fclose( input_file );
    return EXIT_SUCCESS;
}