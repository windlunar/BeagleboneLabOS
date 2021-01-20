# arm-os-from-scratch

Hi ,<br><br>
嘗試從頭開始寫一個在arm架構上運行的作業系統 ,這個作業系統只包含kernel的部份 ,系統啟動需要的bootloader是使用U-boot.
<br><br>
之後會更新實做時的方法跟想法, 可以給也想要嘗試實現一個簡單的作業系統, 但不知道從哪下手的朋友, 一個小參考或是交流 
<br><br>
用於測試的實體硬體是使用 Beaglebone black這塊開發板 ,使用TI的AM3358處理器(ARMv7-A architecture).
<br><br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/logo.png)

Feature:
=
1. 有區分kernel模式與使用者模式(user mode) ,一般的task(process)運行在使用者模式 ,呼叫system call之後轉到kernel模式運行<br><br>
1. 基於 Time slice 的 round-robin multitasking.(之後會嘗試基於priority的排程)<br><br>
2. 有簡易的 Memory分配機制(目前還沒有區分虛擬記憶體位址).<br><br>
3. 有一個簡易的command line用於測試.<br>

System call:
=

目前實作了四個 system call ,之後會增加(其實還有一個print_hello,是早期用於測試時第一個system call).<br>

1. yield :Task主動放棄CPU的使用權 ,切換其他的task運行.<br><br>
2. get_tid :獲得現在正在執行的task的task id.<br><br>
3. exit :結束正在運行的task.<br><br>
3. fork :複製task自己 ,創造子task.<br><br>

簡易的 command line:
=

Commands:
<br>

1. test :執行之後鍵盤按1或2, 用於測試 fork 與 multitasking<br>
2. help :列出可用的指令.<br>
3. info :印出一些資訊.<br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/cmd.png)

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/info.png)

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/fork_test.png)

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/test.png)


環境建立:
=

<br>
需要準備:
<br>
1. 一台PC或筆電作為寫程式的電腦,Beaglebone black ,安裝Linux Ubuntu 18.04
<br><br>
2. Beaglebone black 開發板
<br><br>
3. UART 轉 USB的模組
<br><br>
4. SD卡, 裡面只要灌 U-boot(Bootloader for Beaglebone black)
<br>

<br>
5.安裝編譯環境 ,這是我使用的 arm gcc版本: "gcc-arm-none-eabi-6-2017-q2-update" 
<br><br>
6.安裝 "minicom" ,類似putty的終端軟體

        sudo apt-get install minicom

<br>
7. 選擇任意一個喜歡的資料夾.<br><br>
8. clone 這個 project:

        git clone https://github.com/windlunar/arm-os-from-scratch

<br>
9. 移動到這個project的資料夾下 "arm-os-from-scratch folder " , 輸入 make 編譯所有source code.

        make
<br>
10.UART轉USB模組tx ,tx端接 Beaglebone black 的 UART0, 另一端 usb端接上 pc或筆電的 usb port.
<br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/uart0.png)

<br>
11. 在ubuntu打開另一個 terminal ,執行 minicom ,打開minicom後將 baudrate設定為 115200:

        sudo minicom
<br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/minicom.png)

<br>        
12.壓住 Beaglebone black的 "S2" 按鈕(在beaglebone black的usb port那一側) ,beaglebone black上電(透過micro usb或是5V插孔供電). 這時候 Beaglebone black應該會從 SD card的 U-boot開機("S2"按鈕要一直壓著)<br>

<br>
13.當看到 "Press SPACE to abort autoboot in 2 seconds" 這行時按空白鍵 ,這時應該就會進去 Uboot自己的 command line.<br>

<br>
14.輸入下面的指令將 kernel從電腦上載到 beaglebone black的記憶體內 ,放置位址從 0x81000000開始.

        loadx 0x81000000

<br>
15.輸入 "CTRL+A" ,然後輸入 "S" ,應該會看到如下畫面 ,選擇 "xmodem"

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/choose_xmodem.png)


<br>
16.選擇 "[Goto]"
<br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/goto.png)

<br>
17.輸入: "你的路徑/arm-os-from-scratch/bin"

<br><br>
18.選擇 "beaglebonLabOS.elf" 這個執行檔, 然後等待上載完成.

<br>
19.上載完成後, 輸入以下指令 ,代表執行放在0x81000000處的執行檔, 就是剛剛上載的 kernel執行檔:

        bootelf 0x81000000


<br>
接下來就進入 kernel了 ,然後就能看到一開始那張圖的畫面！
因為 watchdog的原因 ,大概50秒之後就會自動reboot.


![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/bbb.jpg)


教學:
=

待更新...