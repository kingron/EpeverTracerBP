#include "epever_tracer.h"

void main()
{
    srand(time(NULL));
    char s[500] = {0x00};
    int id, r;

    int fd = ep_open("/dev/ttyMT1");
    if (fd <= 0) return false;
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "打开设备成功，句柄： %d", fd);

    for (int i = 1; i<=3; i++) {
        id = ep_get_id(fd);
        if (id > 0) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "获取设备ID: %d", id);
    if (id == -1) {
        close(fd);
        return false;
    }
    sleep(1); // 避免太快丢包

    bool onoff = rand() % 2;
    r = (onoff == 0) ? ep_power_off(fd, id) : ep_power_on(fd, id);
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "手动开关电源 %d，结果： %d", onoff, r);
    sleep(1);

    for (int i=1; i<=3; i++) {
        r = ep_get_clock(fd, id, s);
        if (r == true) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "获取设备时钟： %s，结果: %d", s, r);
    sleep(1); // 避免太快丢包

    for (int i=1; i<=3; i++) {
        r = ep_set_clock(fd, id, 2018, 11, 22, 12, 34, 00);
        if (r == true) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "设置设备时钟： %d", r);
    sleep(1); // 避免太快丢包

    TRACER_CHARGE_SETTINGS res = {0x00};
    for (int i = 1; i<=3; i++) {
        r = ep_get_status(fd, id, &res);
        if (r == true) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "设备状态： \n 蓄电池类型: %d\n提升充电电压: %f\n结果： %d", res.bat_type, res.v_charge_up, r);
    sleep(1); // 避免太快丢包

    byte h, m;
    for (int i=1; i<=3; i++) {
        r = ep_get_night(fd, id, &h, &m);
        if (true == r) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "获取夜间时长: hour: %d, minute: %d，结果：%d", h, m, r);
    sleep(1); // 避免太快丢包

    h = rand() % 24;
    m = rand() % 60;
    for (int i=1; i<=3; i++) {
        r = ep_set_night(fd, id, h, m);
        if (true == r) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "设置夜间时长: hour: %d, minute: %d，结果: %d", h, m, r);

    unsigned short mode = -1;
    for (int i=1; i<=3; i++) {
        if (true == ep_get_load_mode(fd, id, &mode)) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "获取负载模式： %d", mode);
    sleep(1); // 避免太快丢包

    mode = LOAD_MODE_TIME;
    for (int i=1; i<=3; i++) {
        r = ep_set_load_mode(fd, id, mode);
        if (true == r) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "设置负载模式： %d，结果： %d", mode, r);
    sleep(1); // 避免太快丢包


    unsigned short t = -1;
    for (int i=1; i<=3; i++) {
        r = ep_get_time_used(fd, id, &t);
        if (true == r) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "获取定时时段数(0-1)： %d，结果： %d", t, r);
    sleep(1); // 避免太快丢包

    t = rand() % 2;
    for (int i=1; i<=3; i++) {
        r = ep_set_time_used(fd, id, t);
        if (true == r) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "设置定时时段数(0-1)： %d，结果： %d", t, r);
    sleep(1); // 避免太快丢包

    TIME_SETTINGS ts = {0x00};
    for (int i=1; i<=3; i++) {
        r = ep_get_time_range(fd, id, &ts);
        if (r == true) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "获取定时设置: 定时1 = (%d:%d:%d 到 %d:%d:%d)， 定时2 = (%d:%d:%d 到 %d:%d:%d) 结果: %d",
                        ts.power_on_time1_hour, ts.power_on_time1_minute, ts.power_on_time1_second,
                        ts.power_off_time1_hour, ts.power_off_time1_minute, ts.power_off_time1_second,
                        ts.power_on_time2_hour, ts.power_on_time2_minute, ts.power_on_time2_second,
                        ts.power_off_time2_hour, ts.power_off_time2_minute, ts.power_off_time2_second,
                        r);
    sleep(1);

    ts.power_on_time1_hour = rand() % 4;
    ts.power_on_time1_minute = rand() % 60;
    ts.power_on_time1_second = rand() % 60;
    ts.power_off_time1_hour = 4 + rand() % 8;
    ts.power_off_time1_minute = rand() % 60;
    ts.power_off_time1_second = rand() % 60;

    ts.power_on_time2_hour = ts.power_off_time1_hour + rand() % 4;
    ts.power_on_time2_minute = rand() % 60;
    ts.power_on_time2_second = rand() % 60;
    ts.power_off_time2_hour = ts.power_on_time2_hour + rand() % 4;
    ts.power_off_time2_minute = rand() % 60;
    ts.power_off_time2_second = rand() % 60;
    t = rand() % 2 + 1;

    for (int i=1; i<=3; i++) {
        r = ep_set_time_range(fd, id, ts);
        if (true == r) break;
        sleep(1);
    }
    __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, "设置定时设置: 时段数=%d, 定时1 = (%d:%d:%d 到 %d:%d:%d)， 定时2 = (%d:%d:%d 到 %d:%d:%d) 结果: %d", t,
                        ts.power_on_time1_hour, ts.power_on_time1_minute, ts.power_on_time1_second,
                        ts.power_off_time1_hour, ts.power_off_time1_minute, ts.power_off_time1_second,
                        ts.power_on_time2_hour, ts.power_on_time2_minute, ts.power_on_time2_second,
                        ts.power_off_time2_hour, ts.power_off_time2_minute, ts.power_off_time2_second,
                        r);

    close(fd);
    return true;
   }
