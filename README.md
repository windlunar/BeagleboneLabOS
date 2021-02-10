# arm-os-from-scratch

Hi ,<br><br>
嘗試從頭開始寫一個在 arm 架構上運行的 operating system ,這個 os 只包含 kernel 的部份 ,系統啟動需要bootloader(在此使用U-boot).
<br><br>
用於測試的實體開發板為 Beaglebone black ,處理器為 TI 的 AM3358 (ARMv7-A architecture).
<br><br>

![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/logo.png)

Feature:
=
1. 區分kernel模式與使用者模式(user mode) ,一般的task(process)運行在使用者模式 ,呼叫system call之後轉到kernel模式運行<br>
2. 基於 Time sharing 切換不同 task.<br>
3. Priority-based round-robin multitasking. 預設有0~4 ,5個priority.<br>
4. MMU Enable.<br>
5. 簡易的 Memory分配機制.<br>
6. 簡易的 IPC (task之間互相溝通).<br>
7. 有一個存在於記憶體中的類似檔案系統的小系統<br>
8. 有一個簡易的command line用於測試.<br>

System call:
=

目前有這些 system call (其實還有一個print_hello,是早期用於測試時第一個system call).<br>

1. yield :Task主動放棄CPU的使用權 ,切換到其他的task運行.<br>
2. gettid :獲得現在正在執行的task的task id.<br>
3. exit :結束正在運行的task.<br>
4. fork :複製task自己 ,創造子task.<br>
5. do_taskCreate :建立task.<br>
6. malloc_blk :分配一個block的記憶體空間.<br>
7. mfree_blk :歸還使用__malloc_blk獲得的空間.<br>
8. get_task_priority :獲得現在正在執行的task的priority.<br>
9. write :寫資料到file(如tty0).<br>
10. read :從file(如tty0)讀資料.<br>
11. open :打開file ,目前每個task打開consile_in 跟 out (fd =0 and 1).可以打開uart0(tty0)<br>
12. getcwd :獲得當前目錄.<br>
13. getsubdir :獲得當前目錄的所有子目錄<br>
14. getfdir :獲得當前目錄的所有file<br>
15. chdir :改變當前目錄<br>
16. getfullpath :獲得當前目錄的完整路徑<br>
17. restart :重新啟動<br>
18. close :關閉file<br>

簡易的 command line:
=

Commands:
<br>

1. test :執行之後鍵盤按1~5, 分別測試 :<br>
	fork<br>
	multitasking(同樣的priority)<br>
	priority multitasking<br>
	tty0 open test<br>
	ipc test.<br>
2. help :列出可用的指令.<br>
3. info :印出一些資訊.<br>
4. ls :列出當前目錄下的子目錄與檔案.<br>
5. cd :切換目錄.<br>
6. pwd :列出當前路徑.<br>
7. reboot :重新啟動.<br>


環境準備:
=

安裝img映像檔 :
<br><br>
如要自行編譯 u-boot 與製作 boot ,rootfs 分割區的話可以參考下面這本書:
<br>
https://github.com/PacktPublishing/Mastering-Embedded-Linux-Programming-Second-Edition


或是使用已經製作好的：
<br>
將sdcard-img資料夾內的bos-sd.zip解壓縮後 ,會得到 bos-sd.img 映像檔,
<br>
<br>
該映像檔有兩個分割區 ,boot 分割區存放 MLO,u-boot.img 與 OS 的 elf 檔 bos ,另一個分割區為 root ,保留用目前沒有使用到
<br>
<br>
使用 sd卡燒錄軟體將 bos-sd.img 燒進sd卡中
<br>
<br>
5.安裝編譯環境 : "gcc-arm-none-eabi-9-2020-q2-update" 
<br><br>
確定該編譯器安裝在 ubuntu系統的 /opt 下.<br> 
如果不是的話可以到 obj/MakeFile 中更改此行為實際路徑.<br>

	/opt/gcc-arm-none-eabi-9-2020-q2-update/lib/gcc/arm-none-eabi/9.3.1
<br>

6.安裝 "minicom" ,類似putty的終端軟體 ,或使用putty,等其他終端軟體

	sudo apt-get install minicom


7.選擇任意一個喜歡的資料夾.<br><br>
8.clone 這個 project:

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
18.選擇 "bos" 這個執行檔, 然後等待上載完成.

<br>
19.上載完成後, 輸入以下指令 ,代表執行放在0x81000000處的執行檔, 就是剛剛上載的 kernel執行檔:

	bootelf 0x81000000


<br>
接下來就進入 kernel了 ,然後就能看到一開始那張圖的畫面！


![image](https://github.com/windlunar/arm-os-from-scratch/blob/master/pictures/bbb.jpg)


DOC:
=

待更新...