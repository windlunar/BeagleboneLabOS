# BeagleboneLabOS

Hi ,<br>
I'm trying to make a simple operating system on armv7a. Beaglebone black is the testing hardware.
<br><br>

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/logo.png)

Feature:
=

Time slice based round-robin multitasking.
<br>
Simple memory management function.
<br>
Simple command line interface.
<br>


System call:
=
  
yield<br>
get_tid<br>
exit<br>
fork<br>

Simple command line interface:
=
  
test :Press '1' for fork function test ,'2' for multitasking test
<br>               
help :List available command.
<br>          
info :Print some info.

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/cmd.png)

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/info.png)

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/fork_test.png)

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/test.png)


Setup:
=

<br>
Need to prepare:
<br>
1. A PC with Ubuntu 18.04
<br>
2. Beaglebone black
<br>
3. TTL to USB converter
<br>
4. SD card with U-boot
<br>

<br>
2.Install "gcc-arm-none-eabi-6-2017-q2-update" to compile source code
<br><br>
3.Install "minicom"

        sudo apt-get install minicom

<br>
4. Move to your working space or any directory you like.<br><br>
5. clone this project:

        git clone https://github.com/windlunar/BeagleboneLabOS

<br>
6. Move to the BeagleboneLabOS folder , and comile all.

        make
<br>
7.Connect Beaglebone's UART0 to TTL-USB converter ,and plug TTL-USB converter to PC's usb port.
<br>

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/uart0.png)

<br>
8. Open another terminal ,execute minicom and set the baudrate to 115200:

        sudo minicom
<br>
Then you should see:
<br>

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/minicom.png)

<br>        
9.Hold the button "S2" ,and power on beaglebone(through usb or 5V).
Then the board should boot up by U-boot which is inside SD card.<br>

<br>
10.Press space while you see "Press SPACE to abort autoboot in 2 seconds".
Then you will see the U-boot's command line.<br>

<br>
11.Type the following command at U-boot's command line to upload binary file form pc to beaglebone through UART0.
And the elf will be uploaded to beaglebone's memory ,and place to 0x81000000.

        loadx 0x81000000

<br>
12.Type "CTRL+A" and then "S" ,you should see:
<br>
13.Choose "xmodem"

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/choose_xmodem.png)


<br>
13.Choose "[Goto]"
<br>

![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/goto.png)

<br>
14.Go to : "your workspace/BeagleboneLabOS/bin"

<br><br>
15.Choose the "beaglebonLabOS.elf" and wait for upload complete.

<br>
16.After upload successfully, type the command to execute the kernal:

        bootelf 0x81000000


<br>
About 50 seconds latter ,beaglebone black will reboot automatically because of watchdog.


![image](https://github.com/windlunar/BeagleboneLabOS/blob/master/pictures/bbb.jpg)