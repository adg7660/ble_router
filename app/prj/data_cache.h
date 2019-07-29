/**
 */

#ifndef _DATA_CACHE_H
#define _DATA_CACHE_H

#include <stdint.h>

//typedef unsigned int uint32_t;

typedef struct
{
    uint8_t mac[6];
    int16_t temp;
    int16_t vol;
    int16_t rssi;
    int32_t timestamp;
    int32_t mill_secs;
}slv_msg_t;

typedef struct slv_msg_lsg
{
    struct slv_msg_lsg *next;
    int32_t  upload_time;
    int32_t  update_flag;
    slv_msg_t data;
}slv_msg_lst_t;

typedef struct
{
    int32_t len;
    int32_t max_size;
    uint8_t *data;
}xfer_t;

void cache_insert_dev_for_test(void);
int32_t cache_init(void);
int32_t cache_insert_list(slv_msg_t *new_node);
slv_msg_lst_t * cache_search_list(slv_msg_t *msg);
int32_t cache_read_list_by_time(slv_msg_lst_t *buf[], int32_t number);
int32_t cache_list_update_upload_state(slv_msg_lst_t *node[],int node_num);
int32_t cache_delete_all_list(void);
int32_t cache_delete_list(int32_t nodes);

#endif //_DATA_CACHE_H
