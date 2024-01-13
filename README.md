# <center>LiClock 墨水屏天气时钟

## 关于后续的支持
感谢大家对本项目的支持，软件继续更新并修复bug，直到2024年7月31日。大家提交bug记得附上复现方法  

### <center>一种兼具易用性与扩展性的多功能墨水屏天气时钟 
![封面](images/封面.png)
## <center>硬件购买注意事项
屏幕型号：`E029A01`  
ESP32：wroom或者其它封装和引脚兼容的模组，建议Flash选大一点  
### **尽量不要买esp32-solo-1，虽然能用，但价格没有任何优势，除非用拆机件**  
其它照着BOM买就行，买之前请认真阅读开源平台下面的DIY注意事项  
### 元器件购买相关说明[请看Wiki](https://github.com/diylxy/LiClock/wiki/%E5%85%83%E5%99%A8%E4%BB%B6%E8%B4%AD%E4%B9%B0)
---
## <center>软件使用说明

### 程序烧录[请看Wiki](https://github.com/diylxy/LiClock/wiki/%E5%9B%BA%E4%BB%B6%E7%83%A7%E5%BD%95)  

### 手动编译固件[请看Wiki](https://github.com/diylxy/LiClock/wiki/%E6%89%8B%E5%8A%A8%E7%BC%96%E8%AF%91%E5%9B%BA%E4%BB%B6)  

---
### 拨轮开关使用说明
|  按键   | 短按功能  | 长按功能 |
|  ----  | ----  | ---- |
| 左键  | 输入数字/时间：当前位-1 | 返回上个App<br/>输入数字/时间：光标右移<br/>电子书：上一页 |
| 右键  | 输入数字/时间：当前位+1 | 输入数字/时间：光标右移<br/>电子书：下一页|
| 中键  | 确认 | 重置输入为默认值 |
---
### 点此查看[Lua App编写规范](src/lua/README.md)  

## <center>Blockly
~~因为现在的Lua语言与Blockly并未完全适配，有些“积木”后续会进行修改，其中包括：~~  

### Blockly使用教程  
暂无，挂一张照片在这吧  
![封面](images/Blockly屏幕截图.png)

## <center>其它
### midi转buz[请看Wiki](https://github.com/diylxy/LiClock/wiki/midi%E8%BD%ACbuz)  

### 图像转lbm[请看Wiki](https://github.com/diylxy/LiClock/wiki/%E5%9B%BE%E5%83%8F%E8%BD%AClbm)  

## <center> 开源协议
### 因为用了彩云天气的API，仅供学习研究，如果需要商用，则不得包含此源代码或由其产生的二进制文件  
### 本项目src路径下的代码没有使用其它任何与墨水屏相关的项目代码  
源代码开源协议为**GPL-3.0**，允许开源情况下商用，但请标明原作者和工程链接，不得售卖源代码或作为闭源项目发布  
另外，按照GPL-3.0协议要求，由此项目衍生出的代码也需要以GPL3.0开源  
此处的开源指使任何人可以自由且免费地获得、修改**源代码**和（或）**硬件工程源文件**  
