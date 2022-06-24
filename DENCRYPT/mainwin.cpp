#include "mainwin.h"

MainWin::MainWin(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWin)
{
    ui->setupUi(this);
#if TIME_OFFSET
    this->setWindowTitle(QString("33今日说"));
#else
    this->setWindowTitle(QString("33每日说"));
#endif
    Init();
}

void MainWin::Init(){
    /* Instruction */
    ui->srcTxt->setText(QString(
"欢迎使用33说！\n\
加密解密前，请先确保密钥相同且合法哦！\n\
密钥支持12字及以内的数字字母混合密钥，然鹅字母大小写是等价的捏！\n\
（因为是64位36进制整数，所以13个字也有一部分可以接受哦）\n\
加密解密后的文字将自动复制到粘贴板的！\n\
每日版和今日版的加密解密不混用。今日版只能解密今天加密的文本哦！\n\
（当然。。。你也可以改系统时间，我会当看不到的捏！）"
    ));
    /* Slots and Signals connecting */
    connect(ui->keyEdit, &QLineEdit::textChanged, this, &MainWin::ChangeKey);
    connect(ui->decryptButton, &QPushButton::clicked, this, &MainWin::Decrypt);
    connect(ui->encryptButton, &QPushButton::clicked, this, &MainWin::Encrypt);
    connect(ui->pasteDecrypt, &QPushButton::clicked, this, &MainWin::PasteDecrypt);
    connect(ui->pasteEncrypt, &QPushButton::clicked, this, &MainWin::PasteEncrypt);
    /* Shuffle the dict for the first time */
    ShuffleDictByKey();
    Encrypt();

#if TIME_OFFSET
    /* Set the date offset */
    dateOffset = getDateOffset();
#endif

}

void MainWin::ShuffleDictByKey(){

#if TIME_OFFSET
    //If key is not changed and date not changed
    if(!isKeyChanged && !(dateOffset - getDateOffset())) return;
    dateOffset = getDateOffset();
#else
    //If key is not changed
    if(!isKeyChanged) return;
#endif
    shuffledDict = dict;
    /* Find the shuffles */
    //8 -> 32
    WORD firstShuffleKeys[sizeof(key)] = { 0 };
#if TIME_OFFSET
    LONGWORD invKey = ~(key + dateOffset);                      //8 byte key inversed to avoid too many zero at front
#else
    LONGWORD invKey = ~(key);                                   //8 byte key inversed to avoid too many zero at front
#endif
    BYTE* pInvKey = (BYTE*)&invKey;
    for(LONGWORD i = 0; i < sizeof(invKey); ++i){               //for every bytes of key, generate a new 4 byte by crc32
        firstShuffleKeys[i] = calculate_CRC_32(pInvKey++, 1);
    }
    //32 -> 128
    WORD secondShuffleKeys[sizeof(firstShuffleKeys)] = { 0 };
    pInvKey = (BYTE*)firstShuffleKeys;
    for(LONGWORD i = 0; i < sizeof(firstShuffleKeys); ++i){     //for every bytes of the prevois array
        secondShuffleKeys[i] = calculate_CRC_32(pInvKey++, 1);
    }
    //128 -> 512
    WORD thirdShuffleKeys[sizeof(secondShuffleKeys)] = { 0 };
    pInvKey = (BYTE*)secondShuffleKeys;
    for(LONGWORD i = 0; i < sizeof(secondShuffleKeys); ++i){     //for every bytes of the prevois array
        thirdShuffleKeys[i] = calculate_CRC_32(pInvKey++, 1);
    }

    /* Shuffles the dict */
    ShuffleDict(thirdShuffleKeys, sizeof(thirdShuffleKeys));

    /* Set the KeyChanged state back to false */
    isKeyChanged = 0;
}

void MainWin::ShuffleDict(void* shuffleList, unsigned int length){
    QChar temp;
    BYTE* seq = (BYTE*)shuffleList;
    bool isOdd = length % 2;
    QChar* pShuffledDict = shuffledDict.data();
    for(LONGWORD i = 0; i < length - isOdd; i += 2){
            temp = pShuffledDict[*seq];
            pShuffledDict[*seq] = pShuffledDict[*(seq + 1)];
            pShuffledDict[*(seq + 1)] = temp;
            seq += 2;
    }
}

void MainWin::ChangeKey(const QString &text){
    bool legal = 1;
    LONGWORD newKey = text.toULongLong(&legal, 36);

    //If key is changed and is legal
    if(key - newKey && legal){
        isKeyChanged = 1;
        key = newKey;   //Set the new key
        ClearMessage();
    }
    //If key is not legal
    else if(!legal){
        ShowMessage("Invalid Key", 1000);
    }
}

void MainWin::PasteDecrypt(){
    ui->srcTxt->setText(clipboard->text());
    Decrypt();
}

void MainWin::PasteEncrypt(){
    ui->srcTxt->setText(clipboard->text());
    Encrypt();
}

void MainWin::Encrypt(){
    ShuffleDictByKey();
    QByteArray srcTxtContent = ui->srcTxt->toPlainText().toUtf8();
    WORD crcChecked = calculate_CRC_32(srcTxtContent.data(), srcTxtContent.size());
    QString showTxt = "";
    /* check the dict */
    for(long long i = 0; i < srcTxtContent.size(); i++){
        showTxt += shuffledDict[srcTxtContent[i] + 128];
    }
    /* adding the crc check */
    BYTE* crcdata = (BYTE*)&crcChecked;
    for(int i = 0; i < 4; ++i){
        showTxt += shuffledDict[*(crcdata + i)];
    }
    /* Text Output */
    ui->outTxt->setText(showTxt);
    clipboard->setText(ui->outTxt->toPlainText());
}

void MainWin::Decrypt(){
    ShuffleDictByKey();
    QString srcTxtStr = ui->srcTxt->toPlainText();
    QByteArray showTxtContent;
    /* check the dict */
    for(long long i = 0; i < srcTxtStr.size() - 4; i++){
        showTxtContent.append(shuffledDict.indexOf(srcTxtStr[i]) - 128);
    }
    /* crc check */
    WORD crcChecked = 0;
    BYTE* crcdata = (BYTE*)&crcChecked;
    for(long long i = srcTxtStr.size() - 4; i < srcTxtStr.size(); i++){
        *(crcdata++) = shuffledDict.indexOf(srcTxtStr[i]);
    }
    if(crcChecked - calculate_CRC_32(showTxtContent.data(), showTxtContent.size())){
#if TIME_OFFSET
        ui->outTxt->setText(QString("解密错误，请确保密钥正确且为今日加密信息"));
#else
        ui->outTxt->setText(QString("解密错误，请确保密钥正确"));
#endif
        return;
    }
    /* Text Output */
    ui->outTxt->setText(QString().fromUtf8(showTxtContent));
    clipboard->setText(ui->outTxt->toPlainText());
}

MainWin::~MainWin()
{
    delete ui;
}

#if TIME_OFFSET
inline LONGWORD getDateOffset(){
    return QDate::currentDate().year() * 366 + QDate::currentDate().month() * 32 + QDate::currentDate().day();
}
#endif

