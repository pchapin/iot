#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <libpq-fe.h>
#include <math.h>
#include <unistd.h>
#include "sensor.h"

/* This is the main file for a short program to log temperature and location data and upload to
 * a db Written by Nick LeClair for software engineering project Last edited 3/20/24
 */

int main( void )
{
    char query_string[1024 + 1];
    //int return_code =0;
    PGconn *local_connection;
    char *local_connection_string =
        "host=localhost port=5432 user=debian password=temppwd dbname=debian connect_timeout=10";
 
    local_connection = PQconnectdb( local_connection_string );
    if( PQstatus( local_connection ) != CONNECTION_OK ) {
        char *message = PQerrorMessage( local_connection );
        printf( "Failed to connect to the local PostgreSQL server!\n" );
        printf( "Message from server: %s\n", message );
        PQfinish( local_connection );   // Must be called even if the connection fails.
        return EXIT_FAILURE;
    }
    else {
        printf( "Local connection succeeded!\n" );
    
        while( 1 ) {
            printf( "About to collect a (fake) reading.\n" );
            //14.50  3.85  0.91 Waterbury Center values

            struct GPSLocation temp_location = get_location( );
            double temp_temperature = get_temperature( 14.5, 3.85, 0.91 );
            //char *temp_time_stamp = get_timestamp( );
            
            sprintf(query_string,
                    "INSERT INTO reading2 (system_id, reading_time, latitude, longitude, temperature) VALUES (%ld, '%s', %8.6f, %9.6f, %6.2f);\n",
                    get_system_id( ),
                    get_timestamp( ),
                    temp_location.latitude,
                    temp_location.longitude,
                    temp_temperature);
            
            printf("The query string has been made (hopefully)\n");
            
            PGresult *result = PQexec( local_connection, query_string ); 
            if( PQresultStatus( result )!= PGRES_COMMAND_OK ) {
                char* message = PQerrorMessage( local_connection );
                printf( "Bad result from server! (ultra sad)");
                printf( "Message from server: %s\n", message );
                return EXIT_FAILURE;
            }

            printf("Reading recorded successfully!");
            printf("Program will sleep for one minute.\n");
            sleep(60);  // Sleep for 60 secondS.
            printf("Awake now!\n");
        }
    }      
    //system("psql --file=temperature-tables.sql");
    printf("The program has finished");
    return EXIT_FAILURE;
}
