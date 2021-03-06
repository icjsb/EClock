#include "include/DHT.h"

void DHTStart()
{
    Data = 1;
    delay_us(2);
    Data = 0;
    delay_ms(1); //延时800us以上
    Data = 1;
    delay_us(30);
}

uchar DHTByteRead() //接收一个字节
{
    uchar i, dat = 0;
    for (i = 0; i < 8; i++) //从高到低依次接收8位数据
    {
        while (!Data)
            ;          //等待50us低电平过去
        delay_us(8);   //延时60us，如果还为高则数据为1，否则为0
        dat <<= 1;     //移位使正确接收8位数据，数据为0时直接移位
        if (Data == 1) //数据为1时，使dat加1来接收数据1
            dat += 1;
        while (Data)
            ; //等待数据线拉低
    }
    return dat;
}