#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAMPLE_SIZE 10

unsigned long get_system_id( void )
{
    char raw_address[17+1];  // Extra space for the null character.
    FILE *input_file;
    unsigned long ID;

    if( (input_file = fopen("/sys/class/net/eth0/address", "r")) == NULL ) {
        ID = 0UL;
    }
    else {
        int i, j;

        // TODO: Verify that a string was read.
        // TODO: Protect against overly long input.
        fscanf( input_file, "%s", raw_address);
        fclose( input_file );

        i = 0;
        while( raw_address[i] != '\0' ) {
            if( isxdigit( raw_address[i] ) ) {
                ++i;
            }
            else {
                j = i;
                while( 1 ) {
                    raw_address[j] = raw_address[j + 1];
                    if( raw_address[j] == '\0' ) break;
                    ++j;
                }
            }
        }
        sscanf( raw_address, "%lX", &ID );
    }
    return ID;
}


// Return the current date/time.
char *get_timestamp( void )
{
    time_t now = time( NULL );
    struct tm *cooked_now = gmtime( &now );

    static char text[19+1];
    strftime( text, 19+1, "%Y-%m-%d %H:%M:%S", cooked_now );
    return text;
}


float get_temperature( void )
{
    // Later this function will do something real.
    return 25.0F;
}


int main( void )
{
    FILE *output_file;

    // <Open temperature-data.sql for writing>
    if( (output_file = fopen("temperature-data.sql", "w")) == NULL ) {
        printf( "Unable to open temperature-data.sql for writing!\n" );
        return EXIT_FAILURE;
    }
 
    fprintf( output_file, "INSERT INTO reading (system_id, reading_time, temperature) VALUES\n" );
    for( int i = 0; i < SAMPLE_SIZE; ++i ) {
        fprintf( output_file, "  (%ld, '%s', %6.2f)", get_system_id( ), get_timestamp( ), get_temperature( ) );
        if( i != SAMPLE_SIZE - 1 ) {
            fprintf( output_file, ",\n" );
        }
        else {
            fprintf( output_file, "\n" );
        }
    }
    fprintf( output_file, ";\n" );
    fclose( output_file );

    return EXIT_SUCCESS;
}
