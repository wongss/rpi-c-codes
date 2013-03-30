Displays eth0 and wlan0 IP address at the LCD which is connected through an i2c io expander.

The script in "lcdshowip" can be used to have it run upon bootup.
See instructions here to do that: http://www.stuffaboutcode.com/2012/06/raspberry-pi-run-program-at-start-up.html

The connection of the LCD is found in the powerpoint slides "00RPi gpio conn.pptx" found in the root.

IMPORTANT: The lastest utility files like pi2c.c pi2c.h plcd.c plcdutl.c plcd.h plcdlowlevel.c plcdlowlevel.h are found in "utilities" main folder.
