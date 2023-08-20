#pragma once

//时间同步间隔设置
/*
0:禁用时间同步
1:2小时
2:4小时
3:6小时
4:12小时
5:24小时
6:36小时
7:48小时
*/
#define SETTINGS_PARAM_NTP_INTERVAL "p1"

/*
外设存储
详见peripherals.h
*/
#define SETTINGS_PARAM_PHERIPHERAL_BITMASK "p2"

/*
屏幕方向（1或3）
*/
#define SETTINGS_PARAM_SCREEN_ORIENTATION "p3"