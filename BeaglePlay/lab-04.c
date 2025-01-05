
// Skeleton for Lab #4

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <libpq-fe.h>

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
    // This array holds query text of up to 1024 characters.
    char query_string[1024 + 1];

    PGconn *local_connection;
    char *local_connection_string =
        "host=localhost port=5432 user=debian password=temppwd dbname=debian connect_timeout=10";
 
    PGconn *cloud_connection;
    char *cloud_connection_string =
        "host=springtail.postgres.database.azure.com port=5432 user=observer password=frenchfry dbname=temperature connect_timeout=10";

    local_connection = PQconnectdb( local_connection_string );
    if( PQstatus( local_connection ) != CONNECTION_OK ) {
        char *message = PQerrorMessage( local_connection );
        printf( "Failed to connect to the local PostgreSQL server!\n" );
        printf( "Message from server: %s\n", message );
        PQfinish( local_connection );   // Must be called even if the connection fails.
        return EXIT_FAILURE;
    }
    printf( "Local connection succeeded!\n" );

    cloud_connection = PQconnectdb( cloud_connection_string );
    if( PQstatus( cloud_connection ) != CONNECTION_OK ) {
        char *message = PQerrorMessage( cloud_connection );
        printf( "Failed to connect to the cloud PostgreSQL server!\n" );
        printf( "Message from server: %s\n", message );
        PQfinish( local_connection );
        PQfinish( cloud_connection );
        return EXIT_FAILURE;
    }
    printf( "Cloud connection succeeded!\n" );

    // The connections have been made. Now query the database(s)!

    int return_code = EXIT_SUCCESS;
    PGresult *result;

    // To get the latest timestamp use:
    //   SELECT MAX(reading_time) FROM reading WHERE system_id = whatever_id
    //
    // To get all records after a certain timestamp use:
    //   SELECT * FROM reading WHERE reading_time > 'whatever_timestamp'
    //
    
    // <Get your system ID (by calling your earlier function)>
    // <Use sprintf to construct the first query in the query_string (using your system id)>
    // <Execute the first query on the cloud connection>
    // IF <A blank row is returned> THEN
    //     <Use sprintf to construct the second query without the WHERE clause>
    // ELSE
    //     <Use sprintf to construct the second query with the WHERE clause>
    // END
    // <Execute the second query on the local connection>
    // FOR <Every row returned> LOOP
    //     <Use sprintf to construct an INSERT INTO command>
    //     <Execute the constructed query on the cloud connection>
    // END

    // The code below is sample code for reference.
    // You may want to eventually delete it or make major changes to it.

    // Find the latest timestamp on any record in the local database.
    sprintf( query_string, "SELECT * FROM reading" );
    result = PQexec( local_connection, query_string );
    if( PQresultStatus( result ) != PGRES_TUPLES_OK ) {
        char *message = PQerrorMessage( local_connection );
        printf( "Bad result from server!\n" );
        printf( "Message from server: %s\n", message );
        return_code = EXIT_FAILURE;
    }
    else {
        int row_count = PQntuples( result );
        for( int i = 0; i < row_count; ++i ) {
            char *id_text = PQgetvalue( result, i, 0 );
            char *temperature_text = PQgetvalue( result, i, 3 );
            printf( "Reading ID: %s, Temperature: %s\n", id_text, temperature_text );
        }
    }
    PQclear( result );   // Must be called even if there is an error result.        
    PQfinish( local_connection );
    PQfinish( cloud_connection );
    return return_code;
}
