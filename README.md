# arm-os-from-scratch

Hi ,<br><br>
嘗試從頭開始寫一個在arm架構上運行的作業系統 ,這個作業系統只包含kernel的部份 ,系統啟動需要的bootloader是使用U-boot.
<br><br>
用於測試的實體硬體是使用 Beaglebone black這塊開發板 ,使用TI的AM3358處理器(ARMv7-A architecture).
<br><br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/logo.png)

Feature:
=
1. 有區分kernel模式與使用者模式(user mode) ,一般的task(process)運行在使用者模式 ,呼叫system call之後轉到kernel模式運行<br><br>
2. 基於 Time slice 切換不同 task.<br><br>
3. Priority-based round-robin multitasking.預設有0~4 ,5個priority.<br><br>
4. 有簡易的 Memory分配機制(目前還沒有區分虛擬記憶體位址).<br><br>
5. 有一個簡易的command line用於測試.<br>
6. 有一個存在於記憶體中的類似檔案系統的小系統<br>

System call:
=

目前有這些 system call (其實還有一個print_hello,是早期用於測試時第一個system call).<br>

1. __yield :Task主動放棄CPU的使用權 ,切換其他的task運行.<br>
2. __gettid :獲得現在正在執行的task的task id.<br>
3. __exit :結束正在運行的task.<br>
4. __fork :複製task自己 ,創造子task.<br>
5. __do_taskCreate :建立task.<br>
6. __malloc_blk :分配一個block的記憶體空間.<br>
7. __mfree_blk :歸還使用__malloc_blk獲得的空間.<br>
8. __get_task_priority :獲得現在正在執行的task的priority.<br>
9. __write :寫資料到file(如tty0).<br>
10. __read :從file(如tty0)讀資料.<br>
11. __open :打開file ,目前每個task打開consile_in 跟 out (fd =0 and 1).可以打開uart0(tty0)<br>
12. __getcwd :獲得當前目錄.<br>
13. __getsubdir :獲得當前目錄的所有子目錄<br>
14. __getfdir :獲得當前目錄的所有file<br>
15. __chdir :改變當前目錄<br>
16. __getfullpath :獲得當前目錄的完整路徑<br>


簡易的 command line:
=

Commands:
<br>

1. test :執行之後鍵盤按1或2或3或4, 測試 fork 與 multitasking ,priority multitasking ,tty0 open tese.<br>
2. help :列出可用的指令.<br>
3. info :印出一些資訊.<br>
4. ls :列出當前目錄下的子目錄與檔案.<br>
5. cd :切換目錄.<br>
6. pwd :列出當前路徑.<br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/cmd.png)

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/info.png)

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/fork_test.png)

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/test.png)


環境準備:
=

需要準備:
<br><br>
1. 一台PC或筆電作為寫程式的電腦 ,安裝Linux Ubuntu 18.04
<br><br>
2. Beaglebone black 開發板
<br><br>
3. UART 轉 USB的模組
<br><br>
4. SD卡
<br>
安裝img映像檔 :
<br><br>
如要自行編譯u-boot與製作boot ,rootfs分割區的話可以參考下面這本書:
<br>
https://github.com/PacktPublishing/Mastering-Embedded-Linux-Programming-Second-Edition

<br><br>
或是用已經製作好的：<br>
將sdcard-img資料夾內的bos-boot16M-rootfs48M.zip解壓縮後 ,會得到 bos-boot16M-rootfs48M.img 映像檔,
<br><br>
該映像檔有兩個分割區 ,boot分割區放 MLO,u-boot.img 與 OS的elf檔bos ,另一個分割區為rootfs ,為保留用目前沒有使用到
<br><br>
使用 sd卡燒錄軟體將bos-boot16M-rootfs48M.img燒進sd卡中





<br>
5.安裝編譯環境 ,這是我使用的 arm gcc版本: "gcc-arm-none-eabi-9-2020-q2-update" 
<br>
確定該編譯器安裝在 ubuntu系統的 /opt 下<br> 
如果不是的話可以到這個project的 obj/MakeFile中更改<br>
/opt/gcc-arm-none-eabi-9-2020-q2-update/lib/gcc/arm-none-eabi/9.3.1 成實際的路徑

<br><br>
6.安裝 "minicom" ,類似putty的終端軟體 ,或使用putty,等其他終端軟體

        sudo apt-get install minicom

<br>
7. 選擇任意一個喜歡的資料夾.<br><br>
8. clone 這個 project:

        git clone https://github.com/windlunar/arm-os-from-scratch

<br>
9. 移動到這個project的資料夾下 "arm-os-from-scratch folder " , 輸入 make 編譯所有source code.

        make
<br>
10.UART轉USB模組tx ,rx接 Beaglebone black 的 UART0, 另一端 usb端接上 pc或筆電的 usb port.
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
這時可以選擇從sd卡載入kernel, 或是透過uart將kernel從電腦端傳到beaglebone black


從sd卡載入:
=

<br>
14.輸入下面的指令可以看到 boot分割區下的檔案 ,應該可以看到一個名叫bos的檔案

        fatls mmc 0:1 


15.下面的指令將bos從sd卡中載入到beaglebone black的記憶體內 放置位址從 0x81000000開始.

        fatload mmc 0:1 0x81000000 bos


16.上載完成後, 輸入以下指令 ,代表執行放在0x81000000處的執行檔, 就是剛剛上載的 kernel執行檔:

        bootelf 0x81000000

接下來就進入 kernel了 ,然後就能看到一開始那張圖的畫面！
<br>



透過uart上傳:
=

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


紀錄:
=

待更新...