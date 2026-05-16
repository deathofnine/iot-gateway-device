#ifndef LINUX_H
#define LINUX_H
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "led.h"
#include <QDebug>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>      // 添加：for gettimeofday
#include <linux/input.h>   // 添加：for input_event, EV_SYN, SYN_REPORT
#endif // LINUX_H
