
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <linux/i2c.h>
#include <linux/i2c-dev.h>

int main( void )
{
    int handle;

    if( ( handle = open( "/dev/i2c-3", O_RDWR ) ) == -1 ) {
        perror( "Unable to open the i2c bus" );
        return EXIT_FAILURE;
    }

    if( ioctl( handle, I2C_SLAVE, 0x4A ) == -1 ) {
        perror( "Unable to specify sensor address" );
        close( handle );
        return EXIT_FAILURE;
    }

    unsigned char command[2] = { 0x24, 0x00 };
    if( write( handle, command, 2 ) == -1 ) {
        perror( "Unable to write command to sensor" );
        close( handle );
        return EXIT_FAILURE;
    }

    // Wait 50 ms for the conversion.
    usleep(50000);

    unsigned char result[3];
    ssize_t read_count;
    if( (read_count = read( handle, result, 3 ) ) == -1 ) {
        perror( "Unable to read result from sensor" );
        close( handle );
        return EXIT_FAILURE;
    }

    printf("Result: %02X %02X %02X\n", result[0], result[1], result[2] );

    close( handle );
    return EXIT_SUCCESS;
}