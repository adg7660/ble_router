/**
 * @brief 
 * 
 * @file init.c
 * @date 2018-09-28
 * @author Chenfei
 * @copyright (c) 2016-2018 by Druid Technology Co., Ltd.  All Rights Reserved.
 */

#include "app_freertos.h"
#include "init.h"
#include "debug.h"
#include "hal_pin.h"
//#include "app_log.h"
#include "pca10056.h"
#include "ble_s_obj.h"
#include "ble_c_app.h"
#include "ble_s_app.h"
#include "net_manage.h"
#include "setting_rights.h"
#include "crc16.h"
#include "HexStr.h"
#include "hal_wdt.h"
#include "hal_rtc.h"
#include "hal_cpu.h"
#include "data_mgt.h"

#define R_RUN_INDEX     NRF_GPIO_PIN_MAP(1,9)
#define G_RUN_INDEX     NRF_GPIO_PIN_MAP(0,6)
DBG_SET_LEVEL(DBG_LEVEL_I);
//DBG_LOG_ENABLE(true);

static TaskHandle_t init_handle = NULL;
//static TaskHandle_t ble_c_handle = NULL;
static TaskHandle_t wdt_handle = NULL;
//static ble_s_t      *ble_s = NULL;


static void wdt_callback(void)
{
    DBG_D("watch dog callback");
    //uint32_t time_stamp = hal_rtc_get_time();
    //save_time();
    hal_cpu_reset();

}
static void wdt_handle_task(void *arg)
{
    static hal_wdt_cfg_t cfg = 
    {
      .timeout = 10000,
      .stop_when_sleep = 0,
      .stop_when_debug = 0,
    };
    hal_err_t ret = hal_wdt_init(&cfg,wdt_callback);
    DBG_I("watch dog init %d",ret);
    while(1)
    {
        DBG_D("...watch dog feed...");
        hal_wdt_feed();
        vTaskDelay(8000);
    }
}
void create_wdt_task(void)
{
    if(xTaskCreate( wdt_handle_task, "wdt", 64, NULL, 1, &wdt_handle ) != pdPASS)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }
}
void creat_app_task(void)
{
    //create_setting_rights_task();
    create_ble_c_task();
    //create_ble_s_task();
    create_net_manage_task();
    create_data_mgt_task();
    create_wdt_task();
}
void init_handle_task(void *arg)
{
    DBG_I("init_handle_task startup");
    hal_pin_set_mode(G_RUN_INDEX,HAL_PIN_MODE_OUT);
    hal_pin_write(G_RUN_INDEX,HAL_PIN_LVL_LOW);
    //static task_trig_t task_notify;
    //hal_rtc_set_time(1562422578);
    creat_app_task();
    TickType_t time = xTaskGetTickCount();
    while(1)
    {
        hal_pin_write(G_RUN_INDEX,HAL_PIN_LVL_LOW);
        vTaskDelay(pdMS_TO_TICKS(10));
        hal_pin_write(G_RUN_INDEX,HAL_PIN_LVL_HIGH);
        vTaskDelay(pdMS_TO_TICKS(1000));
        if((xTaskGetTickCount() - time)/1000 >= 600)
        {
            hal_rtc_save();
            time = xTaskGetTickCount();
        }
    }
}
void creat_init_task(void)
{ 
    if(xTaskCreate( init_handle_task, "actv", 64, NULL, 1, &init_handle ) != pdPASS)
    {
        APP_ERROR_HANDLER(NRF_ERROR_NO_MEM);
    }  
}