//
// Created by LYH on 2019/3/13.
// 版权所有，2019, Kingron<kingron@163.com>
// For commercial usage, please contact Kingron
//  
//  Full source code, please contact Kingron
//
#include "epever_tracer.h"

#pragma pack (1)

int ep_crc16(uint8_t *data, int length) {
    int Reg_CRC = 0xffff;
    int temp;
    int i, j;

    for (i = 0; i < length; i++) {
        temp = data[i];
        if (temp < 0) temp += 256;
        temp &= 0xff;
        Reg_CRC ^= temp;

        for (j = 0; j < 8; j++) {
            if ((Reg_CRC & 0x0001) == 0x0001)
                Reg_CRC = (Reg_CRC >> 1) ^ 0xA001;
            else
                Reg_CRC >>= 1;
        }
    }
    return (Reg_CRC & 0xffff);
}

bool ep_can_read(int fd, int timeout) {
    fd_set read_fds, write_fds, except_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_ZERO(&except_fds);
    FD_SET(fd, &read_fds);
    struct timeval to;
    to.tv_sec = 1;
    to.tv_usec = 0;
    if (select(fd + 1, &read_fds, &write_fds, &except_fds, &to) != 1)
        return false;
    else
        return true;
}

/**
 * 从文件设备中读取数据，按给定的长度读取，并在指定的时间内超时返回
 * 本函数适合从串口读取到给定长度的数据，一般情况下串口读取不一定读到完整数据
 * @param fd 文件设备句柄
 * @param buf 缓冲区
 * @param size 读取的字节数
 * @param useconds 超时返回微妙数
 * @return
 */
bool readex(int fd, void *buf, size_t size, int useconds) {
    int left = size;
    int readsize = 0;
    long long interval = 0;
    struct timeval tvBegin, tvNow;
    gettimeofday(&tvBegin, NULL);
    do {
        readsize = read(fd, buf, left);
        if (readsize > 0) {
            buf += readsize;
            left -= readsize;
        }
        gettimeofday(&tvNow, NULL);
        interval = (tvNow.tv_sec - tvBegin.tv_sec) * 1000000 + tvNow.tv_usec - tvBegin.tv_usec;
        //LOGD("BatInfo", "读取到 %d 字节，超时: %d", readsize, interval);
        if (interval > useconds) break;
        usleep(8);  // 115200波特率 = 1000000(1s) / 115200 = 8 微秒， 100字节一次提高效率
    } while (left > 0);

    return size - left;
}
