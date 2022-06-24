# 33says
为了躲过一些平台的文字屏蔽~或有心人的陷害（？）~，
33说, 一个文字密钥加密解密工具 诞生了！
你是否因为群里有内鬼而不敢畅所欲言，你是否为平台一些奇怪的文字屏蔽所困扰？
**33说**, 你完美的选择（bushi）。

## 实际背景
因为作者在某聊天群里频频被内鬼举报而啥都不敢说, ~其实被举报的仅仅就是一些轻度涩涩内容，但是举报成功了,~ 暑假时打开Qt乱写了一个下午, 写了这么一个东西, 觉得挺好玩的就发布了出来。

## 声明
此项目仅作学习交流。
本人不鼓励本项目用以谈论任何企图颠覆国家政权、干涉他国内政、键政、种族、性别、色情等敏感话题。
同时也不支持本项目用于任何非法活动和通讯上。
如若发现有人将本项目用以不法途径上, 请主动举报对方, 且通知本人, 本人会立即删除此项目。

## 密钥：
1-12位数字字母混合密钥, 字母大小写等价。
（实则一个64位36进制整数，只要不溢出都判合法，因此也支持部分的13位密钥）

## 目前已实现：
1) 文字解密加密。
2) 剪贴板读取写入, 加密解密结果自动写入剪贴板。
2) 当日加密的信息只能当日解密，除非修改系统时间。
3) 密文末尾添加CRC校验码以校验解密成功性。

## 未来可能会有 ~（画大大大饼？看我懒不懒）~ :
1) 图像加密
2) 文档加密

## 预览图
### 每日说
![图片](https://user-images.githubusercontent.com/82701138/175506694-117189f3-a7fe-42a6-9704-375666807b25.png)

![图片](https://user-images.githubusercontent.com/82701138/175506903-04f6557a-f0cc-4997-9f75-ca1b0e1ace98.png)

### 今日说
![图片](https://user-images.githubusercontent.com/82701138/175507199-aea5a434-6a1d-4407-a46f-8c16945bf18f.png)

![图片](https://user-images.githubusercontent.com/82701138/175507278-db2feab6-f73b-4b50-8aa8-e79d1c35fab4.png)
