# Lua APP规范

所有“APP”都是一个文件夹，文件夹名称结尾是.app  
每个app必须存在一个main.lua和conf.lua文件，可以在文件夹下放一个32*32的icon.lbm图像作为APP图标  
conf.lua为app参数配置文件，UTF8编码，包含以下内容  

必须包含：  
title 字符串 表示app在启动器显示的名称  

可选：  
description 字符串 表示对app的描述
wakeupIO1 整数 表示app要求的唤醒IO
wakeupIO2 整数 表示app要求的第二个唤醒IO
noDefaultEvent 布尔型 是否禁用默认按钮事件
