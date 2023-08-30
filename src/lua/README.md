# Lua APP

## APP编程规范  
app名称最多32个字符  

所有“APP”都是一个文件夹，文件夹名称结尾是.app  
app运行时，当前目录自动切换为app文件夹  
每个app必须存在一个main.lua和conf.lua文件，可以在文件夹下放一个32*32的icon.lbm图像作为APP图标，会自动读取  

---

conf.lua为app参数配置文件，UTF8编码，包含以下内容  

### 必须包含：  
title 字符串 表示app在启动器显示的名称  

### 可选：  
wakeupIO1 整数 表示app要求的唤醒IO  
wakeupIO2 整数 表示app要求的第二个唤醒IO  
noDefaultEvent 布尔型 是否禁用默认按钮事件  
peripherals_requested 整数 请求的外设，详见peripherals.h  

---

main.lua为app源代码，包含以下内容： 

### 可选： 
setup 函数 表示app加载时运行的函数  
lightsleep 函数 进入lightsleep前执行  
wakeup 函数 lightsleep唤醒后执行  
exit 函数 App退出时执行  
deepsleep 函数 进入deepsleep前执行，注意如果没有此函数，会尝试执行exit函数  

---

app间可用appManager.gotoApp(appname)进行跳转  

## APP安装与卸载
所有在LittleFS文件系统根目录下的.app“文件夹”都被视作app，在启动时读取  
app可直接在TF卡中运行，但如果需要显示在应用列表，必须安装  
安装时会将app目录下的全部文件复制到LittleFS根目录下，包括子文件夹  
