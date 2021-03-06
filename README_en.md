# arm-os-from-scratch

Hi ,<br>
I'm trying to make a simple operating system from scratch on ARMv7-A architecture. 
<br>
Beaglebone black is the testing hardware.
<br><br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/logo.png)

Feature:
=

1. Task execute at user mode ,kernel at svc mode<br>
2. Base on time sharing to switch to another task.<br>
3. Priority-based round-robin multitasking.<br>
4. MMU Enable.<br>
5. Simple memory management.<br>
6. Simple command line interface.<br>
7. A simple file system located in RAM.<br>
8. IPC.

System call:
=
  
1. __yield :Give up the usage of CPU ,switch to another task.<br>
2. __gettid :Get the current running task id.<br>
3. __exit :Terminate the current running task.<br>
4. __fork :Create a new task by duplicate the task itself.<br>
5. __do_taskCreate :Create another task.<br>
6. __malloc_blk :Allocate a block of memory space.<br>
7. __mfree_blk :Free the memory space which was allocated by __malloc_blk.<br>
8. __get_task_priority :Get the priority of current running task priority.<br>
9. __write :Write data to file(ex:tty0).<br>
10. __read :Read data from file(ex:tty0).<br>
11. __open :Open file ,Every task will open consile_in and out (fd =0 and 1) by default.
12. __getcwd :Get current dir.<br>
13. __getsubdir :<br>
14. __getfdir :<br>
15. __chdir :<br>
16. __getfullpath :<br>
17. __restart :reboot<br>
18. __close :close file<br>
<br>
Simple command line interface:
=

Commands:
<br>

1. test :Press '1' for fork function test ,'2' for multitasking test ,'3' for priority-based multitasking test ,'4' for tty0 open test ,'5' for ipc test<br>
2. help :List available command.<br>
3. info :Print some info.<br>
4. ls :List file.<br>
5. cd :Change dir.<br>
6. pwd :show current dir.<br>
7. reboot :Restart system.<br>


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
Install u-boot on sd card :
<br><br>
Use "MLO" and "u-boot.img" in folder u-boot
<br>
Plug  your sd card reader into PC
<br>
And check which is your sd card reader ,for example "sdb"

        lsblk

<br>
Go to "arm-os-from-scratch/u-boot"   ,execute the following script to format sd card
<br>
Make sure your sd card is allocate at "sdb" ,otherwise the wrong device would be formatted.

        ./format-sdcard.sh <your sd card device>

The script format-sdcard.sh comes from this book:
<br>
https://github.com/PacktPublishing/Mastering-Embedded-Linux-Programming-Second-Edition

<br>
At  arm-os-from-scratch/u-boot folder:

        cp MLO u-boot.img /media/<Your user name>/boot



<br>
2.Install "gcc-arm-none-eabi-9-2020-q2-update" to compile source code
<br><br>
3.Install "minicom"

        sudo apt-get install minicom

<br>
4. Move to your working space or any directory you like.<br><br>
5. clone this project:

        git clone https://github.com/windlunar/arm-os-from-scratch

<br>
6. Move to the arm-os-from-scratch folder , and comile all.

        make
<br>
7.Connect Beaglebone's UART0 to TTL-USB converter ,and plug TTL-USB converter to PC's usb port.
<br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/uart0.png)

<br>
8. Open another terminal ,execute minicom and set the baudrate to 115200:

        sudo minicom
<br>
Then you should see:
<br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/minicom.png)

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

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/choose_xmodem.png)


<br>
13.Choose "[Goto]"
<br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/goto.png)

<br>
14.Go to : "your workspace/arm-os-from-scratch/bin"

<br><br>
15.Choose the "beaglebonLabOS.elf" and wait for upload complete.

<br>
16.After upload successfully, type the command to execute the kernal:

        bootelf 0x81000000


<br>


![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/bbb.jpg)