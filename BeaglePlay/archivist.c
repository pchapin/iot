// Skeleton for Lab #4
#include "sensor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libpq-fe.h>


int main( void )
{
    // This array holds query text of up to 1024 characters.
    char query_string[1024 + 1];
    int return_code =0;
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
    

    else{
        // The connections have been made. Now query the database(s)!
        printf( "Cloud connection succeeded!\n" );
        return_code = 0;
        PGresult *result;
        PGresult *uploadResult = NULL;
        

    //Get the latest timestamp 
    //get all records after that timestamp 


    unsigned long id = get_system_id();          //get the id 
    sprintf(query_string, "SELECT MAX(reading_time) FROM reading2 WHERE system_id = %lu", id);  //get the latest reading time from your system id
    result = PQexec( cloud_connection, query_string);
    if(PQresultStatus( result)!= PGRES_TUPLES_OK ){     //checking to make sure wuery string works 
        char* message = PQerrorMessage( cloud_connection ); 
        printf( "Bad result from server! (ultra sad)");
        printf( "Message from server: %s\n", message );
        return_code = 1;
        }
    else {
        char *maxTimestamp = PQgetvalue( result, 0, 0);
        if(strlen(maxTimestamp) == 0){
            sprintf(query_string, "SELECT * FROM reading2;");
        }
        else{
             sprintf(query_string, "SELECT * FROM reading2 WHERE reading_time > '%s'", maxTimestamp);
        }
        result = PQexec(local_connection, query_string); 
        if(PQresultStatus( result)!= PGRES_TUPLES_OK ){     //checking to make sure wuery string works 
            char* message = PQerrorMessage( local_connection); 
            printf( "Bad result from localDB! (ultra sad)");
            printf( "Message from localDB: %s\n", message );
            return_code = 1;
            }
        
        printf("Result obtained!\n");
        int row_count = PQntuples( result );
        printf("%d \n", row_count);
        for( int i = 0; i < row_count; ++i ) {
            char *timeStamp_text = PQgetvalue( result, i, 2 );
            char *temperature_text = PQgetvalue( result, i, 5 );
            char *latitude_text = PQgetvalue( result, i, 3 );
            char *longitude_text = PQgetvalue( result, i, 4 );
            char insert_query[150]; // assuming max query length is 150   
            sprintf(insert_query, "INSERT INTO reading2 (system_id, reading_time, latitude, longitude, temperature) VALUES (%ld, '%s', '%s', '%s', %s)",id, timeStamp_text, latitude_text, longitude_text, temperature_text); 
           
            uploadResult = PQexec(cloud_connection, insert_query); //execte the query 
            if(PQresultStatus( uploadResult)!= PGRES_COMMAND_OK ){     //checking to make sure wuery string works 
            char* message = PQerrorMessage( cloud_connection); 
            printf( "Bad result from server! (ultra sad)");
            printf( "Message from server: %s\n", message );
            return_code = 1;
            }
        }
        

    }
    PQclear( result );   // Must be called even if there is an error result.
    if(uploadResult != NULL)PQclear( uploadResult );
}
PQfinish( local_connection );
PQfinish( cloud_connection );
return return_code;
}


   
   