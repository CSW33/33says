#ifndef __MAIN_H__
#define __MAIN_H__

/* Qt In-cludes */
#include <QMainWindow>
#include <QString>
#include <QClipboard>
#include <QDate>


/* Interfiles In-cludes */
#include "ui_mainwin.h"
#include "config.h"
#include "dict.h"
#include "crc32.h"

/* Functional Defines */
#define ShowMessage(text, time) ui->statusBar->showMessage(QString(text), time)
#define ClearMessage() ui->statusBar->clearMessage()

#endif // __MAIN_H__
