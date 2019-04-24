#include <reg52.h>

sbit DS1302_SCLK = P2^3;
sbit DS1302_IO = P2^4;
sbit DS1302_RST = P2^5;

void DS1302ByteWrite(unsigned char dat)
{
    unsigned char mask;

    for (mask = 0x01; mask != 0; mask <<= 1)
    { //低位在前，逐位移出
        if ((mask & dat) != 0)
        { //首先输出该位数据
            DS1302_IO = 1;
        }
        else
        {
            DS1302_IO = 0;
        }
        DS1302_SCLK = 1; //然后拉高时钟
        DS1302_SCLK = 0; //再拉低时钟，完成一个位的操作
    }
    DS1302_IO = 1; //最后确保释放 IO 引脚
}
unsigned char DS1302ByteRead()
{
    unsigned char mask;
    unsigned char dat = 0;

    for (mask = 0x01; mask != 0; mask <<= 1)
    { //低位在前，逐位读取
        if (DS1302_IO != 0)
        { //首先读取此时的 IO 引脚，并设置 dat 中的对应位
            dat |= mask;
        }
        DS1302_SCLK = 1; //然后拉高时钟
        DS1302_SCLK = 0; //再拉低时钟，完成一个位的操作
    }
    return dat; //最后返回读到的字节数据
}
void DS1302SingleWrite(unsigned char reg, unsigned char dat)
{
    DS1302_RST = 1;                     //使能片选信号
    DS1302ByteWrite((reg << 1) | 0x80); //发送写寄存器指令
    DS1302ByteWrite(dat);               //写入字节数据
    DS1302_RST = 0;                     //除能片选信号
}
unsigned char DS1302SingleRead(unsigned char reg)
{
    unsigned char dat;
    DS1302_RST = 1;                     //使能片选信号
    DS1302ByteWrite((reg << 1) | 0x81); //发送读寄存器指令
    dat = DS1302ByteRead();             //读取字节数据
    DS1302_RST = 0;                     //除能片选信号
    return dat;
}
void InitDS1302()
{
    unsigned char i;
    unsigned char code InitTime[] = {//2013 年 10 月 8 日 星期二 12:30:00
                                     0x00, 0x30, 0x12, 0x08, 0x10, 0x02, 0x13};

    DS1302_RST = 0; //初始化 DS1302 通信引脚
    DS1302_SCLK = 0;
    i = DS1302SingleRead(0); //读取秒寄存器

    if ((i & 0x80) != 0)
    {                               //由秒寄存器最高位 CH 的值判断 DS1302 是否已停止
        DS1302SingleWrite(7, 0x00); //撤销写保护以允许写入数据
        for (i = 0; i < 7; i++)
        { //设置 DS1302 为默认的初始时间
            DS1302SingleWrite(i, InitTime[i]);
        }
    }
}