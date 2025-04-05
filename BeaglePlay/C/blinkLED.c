////////////////////////////////////////
// blinkLED.c
// Blinks the USR3 LED.
// Wiring: None
//
// Heavily edited by Peter Chapin to include error handling on file opening and improved
// variable names.
////////////////////////////////////////
//  Tested: rcn-ee: 2021.12.15 - BBGG - 5.15.6-bone14

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OFF 0
#define ON  1

int main( )
{
	FILE *trigger;
	FILE *brightness;
	int return_code = EXIT_SUCCESS;

	if( ( trigger = fopen( "/sys/class/leds/beaglebone:green:usr3/trigger", "w" ) ) == NULL ) {
		printf( "Error: Unable to open the USR3 LED trigger file!\n" );
		return_code = EXIT_FAILURE;
	}
	else if ( ( brightness = fopen( "/sys/class/leds/beaglebone:green:usr3/brightness", "w" ) ) == NULL ) {
		printf( "Error: Unable to open the USR3 LED brightness file!\n" );
		fclose( trigger );
		return_code = EXIT_FAILURE;
	}
	else {
		int led_state = OFF;
		
		fprintf( trigger, "none\n" );
		fclose( trigger );

		while( 1 ) {
			fprintf( brightness, "%d\n", led_state );
			fflush( brightness );
			if( led_state == OFF )
				led_state = ON;
			else
				led_state = OFF;
			usleep( 500000 );
		}

		fclose( brightness );
	}

	return return_code;
}
