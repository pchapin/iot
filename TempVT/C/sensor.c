#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "sensor.h"

#define M_PI 3.14159265358979323846


unsigned long get_system_id( void )
{
    char raw_address[17+1];
    char* filename = "/sys/class/net/eth0/address";
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return 1;
    }
    fscanf( input_file, "%s", raw_address);
    printf( "I read: %s\n", raw_address);
    if( raw_address != 0 ) {
        int i = 0;
        while( raw_address[i] != '\0' ){
            if( isxdigit( raw_address[i] ) ){
                //printf("%c", raw_address[i]);i++;
            }
            else {
                int j = i; 
                while( 1 ){
                    raw_address[j] = raw_address[j+1];
                    if( raw_address[j] == '\0' ) {
                        break;                  
                    }
                    j++;   
                } 
            }
        }
    }  
    else {
        printf("Failed to read address file(No address found)");
    }     
    unsigned long result;
    sscanf( raw_address, "%lX", &result );
    return result;
}



// Return the current date/time.
char *get_timestamp( void )
{
    printf("timestamp\n");

    time_t now = time( NULL );
    struct tm *cooked_now = gmtime( &now );
    static char text[19+1];
    strftime( text, 19+1, "%Y-%m-%d %H:%M:%S", cooked_now );
    return text;
}


float get_temperature( float baseline, float amplitude, float fuzz )
{
    printf("temperature\n");

    time_t now = time( NULL );
    struct tm *cooked_now = gmtime( &now );
    //take time and convert to sin wave 
    float temp = baseline + amplitude * sin(cooked_now->tm_hour * (2 * M_PI / 24));

    // Generate fuzz
    float fuzz_value = (2.0 * fuzz * (float)rand() / RAND_MAX) - fuzz;

    // Apply the random fuzz to the calculated temperature
    temp += fuzz_value;

    return temp;
}


//44.376567  -72.718479  14.50  3.85  0.91 (Waterbury Center)
struct GPSLocation get_location(void)
{
    printf("location\n");

    struct GPSLocation location;

    // Assign static values to the location
    location.latitude = 44.376567;   
    location.longitude = -72.718479; 

    return location;
}

