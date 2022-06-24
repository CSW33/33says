#ifndef __MAINWIN_H__
#define __MAINWIN_H__

#include "main.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWin; }
QT_END_NAMESPACE

class MainWin : public QMainWindow
{
    Q_OBJECT

public:
    MainWin(QWidget *parent = nullptr);
    void Init();
    void ShuffleDictByKey();
    void ShuffleDict(void* shuffleList, unsigned int length);
    ~MainWin();

private slots:
void ChangeKey(const QString &text);
void Encrypt();
void Decrypt();
void PasteEncrypt();
void PasteDecrypt();

private:
    Ui::MainWin *ui;
    QClipboard* clipboard = QGuiApplication::clipboard();
    LONGWORD key = 111;
#if TIME_OFFSET
    LONGWORD dateOffset = 0;
#endif
    QString shuffledDict = dict;
    bool isKeyChanged = 1;
};

inline LONGWORD getDateOffset();

#endif // __MAINWIN_H__
