import sys
import time

# Open the trigger file and specify no triggering. This should be the default on the BeaglePlay.
# Check by doing:
#
# cat /sys/class/leds/beaglebone\:green\:usr3/trigger
#
# Look for the trigger condition enclosed in square brackets. That indicates the current setting
# ("none" is the first in the list).
#
# If you are curious, look at the trigger conditions for the other green user LEDs. Note that
# all of the user LEDs other than LED #3 have some trigger condition pre-assigned. That's why we
# are using LED #3 for this lab.
#
# You can change the trigger conditions for an LED by writing a different trigger condition into
# its trigger file. For example:
#
# cat kbd-numlock | sudo tee /sys/class/leds/beaglebone\:green\:usr3/trigger
#
# This command sets the trigger for user LED #3 so that it follows the numlock key on the
# keyboard (the keyboard connected to the BeaglePlay's USB port, if any).
try:
    # If an exception is raised inside the `with` block, the file is automatically closed.
    with open('/sys/class/leds/beaglebone:green:usr3/trigger', 'w') as led_trigger:
        led_trigger.write("none")
except OSError as error:
    print(f"[Error(trigger file)]: {error}")
    sys.exit(1)   # Abort the program with status code 1 (anything nonzero means failure).

# Open the brightness file. The user LEDs on the BeaglePlay are on/off only. Their maximum
# brightness is 1 (check the max_brightness file for the LED to confirm this).
try:
    led_brightness = open('/sys/class/leds/beaglebone:green:usr3/brightness', 'w')
except OSError as error:
    print(f"[Error(brightness file)]: {error}")
    sys.exit(1)

# Nice names for the LED states.
ON  = 1
OFF = 0

# Initial state.
led_state = OFF

# The program runs forever, turning the LED on and off to make it blink.
while True:
    # Write the current led_state (either 0 (OFF) or 1 (ON)).
    led_brightness.write(str(led_state))  # Convert integer to a string.
    led_brightness.flush()                # Force buffered output to be flushed.

    # Wait for 1/2 second.
    time.sleep(0.5)

    # Toggle the led_state.
    led_state = OFF if led_state == ON else ON
