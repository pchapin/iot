
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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


int main( void )
{
    int return_code  = EXIT_SUCCESS;
    unsigned long ID = get_system_id( );

    if( ID == 0 ) {
        printf( "Error: Unable to generate a unique system ID (no eth0 network interface?)\n" );
        return_code = EXIT_FAILURE;
    }
    else {
        printf( "System ID: %012lX\n", ID );
    }

    return return_code;
}