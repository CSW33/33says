#ifndef __CRC32_H__
#define __CRC32_H__

//CRC32 params defines
#define CRC_POLY 0x04C11DB7						//由CRC-32多项式倒序简化得出
#define CRC_32_Init 0xFFFFFFFF					//CRC32校验初始值
#define CRC_32_OUT  0xFFFFFFFF					//CRC32校验码输出异或值

typedef unsigned char BYTE;					//8位无符号整数
typedef unsigned int WORD;					//32位无符号整数
typedef unsigned long long LONGWORD;        //64位无符号整数

WORD calculate_CRC_32(void* pstart, WORD bytesize);

#endif // __CRC32_H__
