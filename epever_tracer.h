//
// EPEVER Tracer 太阳能充电板系列控制库
// 版权所有，2019/3/13, Kingron<kingron@163.com>
// Copyright, for commercial usage, please contact Kingron
// 用于商业用途请联系 kingron@163.com
// -----------------------------------------
// 参考文档 《B系列产品通讯协议v2.4.pdf》  http://www.epsolarpv.com.cn/
// Modbus-RTU 协议
// 仅在 EPEVER Tracer3906BP 上测试通过，理论上适合下列产品
// LS-B、VS-B、Tracer-B、Tracer-A、iTracer-B、
// eTracer-B 系列控制器
//

#ifndef BATCOM_EPEVER_TRACER_H
#define BATCOM_EPEVER_TRACER_H

#endif //BATCOM_EPEVER_TRACER_H

#ifdef __linux__

#include <stdint.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/serial.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <termios.h>
#include <android/log.h>

#endif

#ifdef _WIN32
#include <stdint.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <io.h>
#include <winsock2.h>
#include <process.h>
#include <winioctl.h>
#include "termios.h"

#define O_NONBLOCK 00004000
#define O_NOCTTY 00000400

#endif

#define LOGI(tag, fmt, args...) __android_log_print(ANDROID_LOG_INFO, tag, fmt, ##args)
#define LOGD(tag, fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, tag, fmt, ##args)
#define LOGE(tag, fmt, args...) __android_log_print(ANDROID_LOG_ERROR, tag, fmt, ##args)

typedef uint8_t byte;

#ifndef bool
#define bool int
#define false 0
#define true 1
#endif

#pragma pack (1)
#define SLEEP 500000

#define CMD_READ 0x03
#define CMD_WRITE 0x10
#define CMD_READ_ONLY 0x04
#define CMD_WRITE_ONLY = 0x05

typedef struct _RES_HEAD {
    byte id; // 设备ID, Address
    byte cmd; // 功能码, Function
    unsigned char len;  // 数据长度
} RES_HEAD;

typedef struct _MODBUS_HEAD {
    byte id;  // 设备 ID
    byte cmd; // 功能码
    unsigned short address; // 读写地址起始位
    unsigned short count; // 读写地址个数
} MODBUS_HEAD;

typedef struct _TRACER_REQ {
    MODBUS_HEAD head;
    uint16_t crc; // CRC 校验
} TRACER_REQ;

typedef struct _CLOCK_DATA {
    byte minute;                              // 9013，实时时钟，分钟
    byte second;                              // 9013，实时时钟，秒
    byte day;                                 // 9014，实时时钟，日
    byte hour;                                // 9014，实时时钟，小时
    byte year;                                // 9015，实时时钟，年
    byte month;                               // 9015，实时时钟，年
} TRACER_CLOCK_DATA;

typedef struct _CHARGE_SETTINGS {
    unsigned short bat_type;                // 9000，蓄电池类型，100x（放大100倍）！
    unsigned short bat_capacity;            // 9001，蓄电池容量，Ah
    unsigned short temp_index;              // 9002，温度补偿系数，数据范围为0-9单位 mV/℃/2V，100x
    unsigned short v_high;                  // 9003，超压断开电压，V，100x
    unsigned short v_charge;                // 9004，充电限制电压，V，100x
    unsigned short v_recover;               // 9005，超压断开恢复电压，V，100x
    unsigned short v_charge_avg;            // 9006，均衡充电电压，V，100x
    unsigned short v_charge_up;             // 9007，提升充电电压，V，100x
    unsigned short v_charge_float;          // 9008，浮充充电电压，V，100x
    unsigned short v_up_recover;            // 9009，提升恢复电压，V，100x
    unsigned short v_low_recover;           // 900A，低压断开恢复电压，V，100x
    unsigned short v_low_alarm_recover;     // 900B，欠压告警恢复电压，V，100x
    unsigned short v_low_alarm;             // 900C，欠压告警电压，V，100x
    unsigned short v_low;                   // 900D，低压断开电压，V，100x
    unsigned short v_used;                  // 900E，放电限制电压，V，100x
} CHARGE_SETTINGS;

typedef struct _TRACER_CHARGE_SETTINGS {
    byte id;
    unsigned short bat_type;  // 蓄电池类型
    float bat_capacity; // 蓄电池容量，放大100倍，最终需要除 100，下同
    float temp_index; // 温度补偿系数
    float v_high; // 超压断开电压
    float v_charge; // 充电限制电压
    float v_recover; // 超压断开恢复电压
    float v_charge_avg; // 均衡充电电压
    float v_charge_up; // 提升充电电压
    float v_charge_float; // 浮充充电电压
    float v_up_recover; // 提升恢复电压
    float v_low_recover; // 低压断开恢复电压
    float v_low_alarm_recover; // 欠压告警恢复电压
    float v_low_alarm; // 欠压告警电压
    float v_low; //低压断开电压
    float v_used; // 放电限制电压
} TRACER_CHARGE_SETTINGS;

typedef struct _TEMP_DATA {
    unsigned short temp_alarm_high;         // 9017，蓄电池温度告警上限值，℃，100x
    unsigned short temp_alarm_low;          // 9018，蓄电池温度告警下限值，℃，100x
    unsigned short temp_stop_charge;        // 9019，设备内温度上限，℃，100x
    unsigned short temp_recover;            // 901A，设备内温度超温后，低于此值系统恢复正常，℃，100x
} TEMP_DATA;

typedef struct _LIGHT_DATA {
    unsigned short light_on_vol;            // 901E，光控启动电压，V， 100倍率，
    unsigned short light_on_delay;          // 901F，光控启动延时, 分钟
    unsigned short light_off_vol;           // 9020，光电池电压高于此电压视为接近白天，V，100x
    unsigned short light_off_delay;         // 9021，光控关闭延时，单位，分钟
} LIGHT_DATA;

#define LOAD_MODE_MANUAL         0x0000
#define LOAD_MODE_LIGHT          0x0001
#define LOAD_MODE_LIGHT_TIME    0x0002
#define LOAD_MODE_TIME           0x0003

typedef struct _LOAD_SETTINGS {
    unsigned short load_mode_value;        // 903D，负载输出控制模式，00=手动，01=光控，02=光控+多时段，03=定时
    byte load_output_time1_hour;              // 903E，负载输出第一段时间长度，小时，用于光控+定时
    byte load_output_time1_minute;            // 903E，负载输出第一段时间长度，分钟，用于光控+定时
    byte load_output_time2_hour;              // 903F，负载输出第二段时间长度，小时，用于光控+定时
    byte load_output_time2_minute;            // 903F，负载输出第二段时间长度，分钟，用于光控+定时
} LOAD_SETTINGS;

typedef struct _TIME_SETTINGS {
    unsigned short power_on_time1_second;   // 9042，定时开时刻1的秒值
    unsigned short power_on_time1_minute;   // 9043，定时开时刻1的分钟
    unsigned short power_on_time1_hour;     // 9044，定时开时刻1的小时
    unsigned short power_off_time1_second;  // 9045，定时关时刻1的秒值
    unsigned short power_off_time1_minute;  // 9046，定时关时刻1的分钟
    unsigned short power_off_time1_hour;    // 9047，定时关时刻1的小时

    unsigned short power_on_time2_second;   // 9048，定时开时刻2的秒值
    unsigned short power_on_time2_minute;   // 9049，定时开时刻2的分钟
    unsigned short power_on_time2_hour;     // 904A，定时开时刻2的小时
    unsigned short power_off_time2_second;  // 904B，定时关时刻2的秒值
    unsigned short power_off_time2_minute;  // 904C，定时关时刻2的分钟
    unsigned short power_off_time2_hour;    // 904D，定时关时刻2的小时
} TIME_SETTINGS;


typedef struct _BAT_INFO {
    float v_solar;                 // 3100，阵列电压，V, 100x
    float a_solar;                 // 3101，阵列电流，A，100x
    float p_solar;                 // 3102，3103，发电功率低位和高位，W，100x
    float v_load;                  // 310C，负载电压，V，100x
    float a_load;                  // 310D，负载电流，A，100x
    float p_load;                  // 310E，310F，负载功率低位+高位，W，100x
    float temp_bat;                // 3110，蓄电池温度，℃，100x
    float temp_box;                 // 3111，机箱温度
    float percent_left;             // 311A，剩余电量，% 百分比
    unsigned short v_bat_level;           // 311D，蓄电池电压等级，V，100x
    unsigned short bat_status;          // 3200，蓄电池状态
    unsigned short charging_status;     // 3201，充电设备状态
    unsigned short used_status;         // 3202，放电设备状态
    float v_bat_max;               // 3302，当日最高蓄电池电压
    float v_bat_min;               // 3303，当日蓄电池最低电压
    float v_bat_cur;               // 331A，当前蓄电池的电压
    float a_bat_cur;                // 331B, 331C当前蓄电池电流低位、高位
} BAT_INFO;

// 如果数据是字节，则高位在前 D15~D8在前，D7~D0后
typedef struct _TRACER_FULL_DATA {          // 从 0x9000 地址开始，直接可以读写所有数据！
    CHARGE_SETTINGS bat_status;

    unsigned short dummy900F;               // 900F，未知
    unsigned short dummy9010;               // 未知
    unsigned short dummy9011;               // 未知
    unsigned short dummy9012;               // 未知

    TRACER_CLOCK_DATA clock;

    unsigned short dummy9016;               // 未知

    TEMP_DATA temp;

    unsigned short dummy901B;               // 901B, 未知
    unsigned short dummy901C;               // 901C，未知
    unsigned short dummy901D;               // 901D，未知

    LIGHT_DATA light;

    unsigned short dummy9022_903C[27];      // 9022-903C 未知

    LOAD_SETTINGS load_settings;

    unsigned short dummy9040;               // 9040，未知
    unsigned short dummy9041;               // 9041，未知

    TIME_SETTINGS time_settings;

    unsigned short dummy904E_9062[21];      // 904E~9062，未知

    unsigned short led_time;                // 9063，LED背光超时，单位秒
    unsigned short dummy9064;               // 未知
    byte night_length_hour;                   // 9065，夜长时长小时数
    byte night_length_minute;                 // 9065，夜长时长分钟
    unsigned short dummy9066;               // 未知
    unsigned short sys_vol_level;           // 9067，系统额定电压等级
                                                // 0，自动识别。1-9对应，12V，24V，36V，
                                                // 48V，60V，110V，120V，220V，240V，
                                                // 指定后则不进行自识别，以后可以继续扩展
    unsigned short dummy9068;               // 未知
    unsigned short used_time_range;         // 9069，定时控制时间段选择
    unsigned short manual_switch;           // 906A，手动默认开关， 0 = 关， 1=开
    unsigned short duration_charge_equalizing; // 906B，均衡充电维持时长，单位，分钟
    unsigned short duration_charge_keep;    // 906C，提升维持时间
    unsigned short max_discharge;           // 906D，放电深度，%，百分比
    unsigned short max_charge;              // 906E，充电深度，20-100%， % 百分比
    unsigned short dummy906F;               // 未知
    unsigned short charge_mode;             // 9070，电池充放电管理模式，0=电压补偿，1：SOC
} TRACER_FULL_DATA;

/**
 * 打开EPEVER Tracer设备
 * @param device 设备名称，485串口，例如 "/dev/ttyCOM0"
 * @return 返回打开句柄，后续给其他函数使用，-1表示失败
 */
int ep_open(char *device);

/**
 * 返回设备ID，当读取的时候，串口上只能有一个设备连接！
 * @param fd 打开的串口句柄
 * @return 成功返回ID（1~247），失败返回 -1
 */
int ep_get_id(int fd);

/**
 * 设置设备ID，设置ID后，必须断电重启设备
 * @param fd 打开的句柄
 * @param id 要设置的设备ID
 * @return 成功返回 true，失败返回 false
 */
int ep_set_id(int fd, byte id);

/**
 * CRC 16校验，通用modbus校验算法
 * @param data 待校验的数据
 * @param length 数据长度
 * @return 校验值
 */
int ep_crc16(uint8_t* data, int length);

/**
 * 返回设备的当前时钟信息，数据返回在clock中，格式: yyyy-mm-dd hh:nn:ss
 * @param fd 打开设备的句柄
 * @param id 需要查询设备的id
 * @param clock 返回的时钟信息，必须外部申请, 20字符数组，需要初始化为0
 * @return 成功返回 true，失败返回false
 */
int ep_get_clock(int fd, byte id, char clock[20]);

/**
 * 设置设备的时钟
 * @param fd 设备句柄
 * @param id 设备ID，1~247
 * @param year 年份，例如2019
 * @param month 月份 1~12
 * @param day 日期 0~31
 * @param hour 小时，24小时格式，0~24
 * @param minute 分钟 00~60
 * @param second 秒 00~60
 * @return
 */
int ep_set_clock(int fd, byte id, int year, byte month, byte day, byte hour, byte minute, byte second);

/**
 * 返回设备的当前状态
 * @param fd 打开的句柄
 * @param id 设备ID
 * @param status 返回的状态信息
 * @return 成功返回 true，失败返回 false
 */
int ep_get_status(int fd, byte id, TRACER_CHARGE_SETTINGS* status);

/**
 * 读取设备的所有配置
 * @param fd 打开的句柄
 * @param id 设备ID
 * @param load_settings 返回的设置数据
 * @return 成功返回 true, 失败返回false
 */
int ep_get_settings(int fd, byte id, TRACER_FULL_DATA *load_settings);

/**
 * 设置负载模式
 * @param fd
 * @param id
 * @param load_settings
 * @return
 */
int ep_set_load_mode(int fd, byte id, unsigned short mode);

/**
 * 读取负载设置
 * @param fd
 * @param id
 * @param load_settings
 * @return
 */
bool ep_get_load_mode(int fd, byte id, unsigned short *mode);

/**
 * 写入一个地址数据，value以主机字节序
 * @param fd 句柄
 * @param id 设备ID
 * @param address 写入的地址，例如0x901A
 * @param value 写入的数据，如果是两字节的整数，则用主机顺序，即无需特殊处理内部会自动转换
 * @return 返回成功与否
 */
unsigned short ep_write_one(int fd, byte id, unsigned short address, unsigned short value);

/**
 * 读取一个地址的数据，返回short数据，返回的数据是主机字节序，内部会自动转换网络字节序为主机字节序
 * @param fd 句柄
 * @param id 设备ID
 * @param address 读写的地址
 * @return 返回主机字节序的short数据（2字节）
 */
bool ep_read_one(int fd, byte id, unsigned short address, unsigned short *value);

/**
 * 获取光控定时的夜间时长
 * @param fd
 * @param id
 * @param hour
 * @param minute
 * @return
 */
int ep_get_night(int fd, byte id, byte* hour, byte* minute);

/**
 * 设置光控定时的夜间时长
 * @param fd
 * @param id
 * @param hour
 * @param minute
 * @return
 */
int ep_set_night(int fd, byte id, byte hour, byte minute);

/**
 * 设置时间范围，如果要使用定时控制，必须设置负载模式为定时！切记！
 * @param fd
 * @param id
 * @param settings
 * @return
 */
int ep_set_time_range(int fd, byte id, TIME_SETTINGS settings);

/**
 * 获取当前的定时设置
 * @param fd
 * @param id
 * @param settings
 * @return
 */
int ep_get_time_range(int fd, byte id, TIME_SETTINGS* settings);

/**
 * 获取当前定时设置的时段个数
 * @param fd
 * @param id
 * @param used
 * @return
 */
bool ep_get_time_used(int fd, byte id, unsigned short *used);

/**
 * 设置当前的定时设置的时段个数，只能为0或1，0=1个时段，1=2个时段
 * @param fd
 * @param id
 * @param used
 * @return
 */
bool ep_set_time_used(int fd, byte id, unsigned short used);

/*******************************************************************
* 名称：                UART0_Set
* 功能：                设置串口数据位，停止位和效验位
* 入口参数：        fd        串口文件描述符
*                              speed     串口速度
*                              flow_ctrl   数据流控制
*                           databits   数据位   取值为 7 或者8
*                           stopbits   停止位   取值为 1 或者2
*                           parity     效验类型 取值为N,E,O,,S
*出口参数：          正确返回为1，错误返回为0
*******************************************************************/
int set_port(int fd,int speed,int flow_ctrl,int databits,int stopbits,int parity);

/**
 * 清空串口缓冲区
 * @param fd
 */
void ep_clear(int fd);

/**
 * 手动控制打开负载，记住：手动开关会强行设定模式为手动，否则可能不起作用
 * @param fd
 * @param id
 * @param powerOn
 * @return
 */
bool ep_power_on(int fd, byte id);

/**
 * 手动控制关闭负载，记住：手动开关会强行设定模式为手动，否则可能不起作用
 * @param fd
 * @param id
 * @param powerOn
 * @return
 */
bool ep_power_off(int fd, byte id);

void ep_read(char data[], int len);

void ep_write(char data[], int len);

BAT_INFO ep_get_batinfo(int fd, byte id);

#pragma pack ()
