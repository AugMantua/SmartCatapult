/**
 * This file is part of AUGMAN smart catapult project.
 * Copyright (c) 2020 Pietro Marchini <pietro.marchini94@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * TCP bitmap streamer.
 *
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "time.h"
#include "sys/time.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "rom/lldesc.h"
#include "esp_intr_alloc.h"
#include "esp_log.h"
#include "camera.h"
#include "esp_netif.h"


#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>


#ifdef __cplusplus
extern "C" {
#endif

#define ESP_ERR_BITMAP_STREAM 0x30000
#define ESP_ERR_BITMAP_STREAM_SOCKET_ERROR      (ESP_ERR_BITMAP_STREAM + 1)
#define ESP_ERR_BITMAP_STREAM_SEMAPHORE_ERROR   (ESP_ERR_BITMAP_STREAM + 2)
#define ESP_ERR_BITMAP_STREAM_BUFFER_ERROR      (ESP_ERR_BITMAP_STREAM + 3)
#define ESP_ERR_BITMAP_STREAM_ALREADY_INIT      (ESP_ERR_BITMAP_STREAM + 4)
#define ESP_ERR_BITMAP_STREAM_FUN_NOT_IMPLEM    (ESP_ERR_BITMAP_STREAM + 5)

/*Standard TCP port for the streaming*/
#ifndef CONFIG_STREAM_PORT
    #define CONFIG_STREAM_PORT 8585
#endif

/*
* defines a struct in order to have only one object
* defines also pointers to function
*/
typedef struct{
    bool            init_done;
    void*           streamer_init(streamer);
    void*           sendPacket(streamer);
    uint32_t*       bufferPointer;
    bool            pointerDefined;
    sockaddr_in*    socketAddress;
    int             socket;  
    char            rx_buffer[128];
    char            addr_str[128];
    int             addr_family;
    int             ip_protocol;   
} streamer;

/**
 * 
 * header structure definition
 * this header must contain timing information of the bitmap
 * master will reply to the stream with position of the target if found
 * and time of the original frame in order to understand in case speed/acceleration ecc
 * If there is a target into the frame master will set targetInFrame and give % of confident
 */
typedef struct 
{
    uint32_t counter;
    uint32_t frameTick;
    bool     targetInFrame;
    uint32_t targetConfident;
} streamHeader;


/**
 * @brief Init the streamer, prepares the semaphore and prepare the socket
 * 
 * @return ESP_OK on success
 * 
 * @param stream Streamer struture, all the logic into one place
*/
esp_err_t streamer_init(struct streamer* stream);

/**
 * @brief TCP send of bitmap stream
 * 
 * @return ESP_OK on success
 * 
 * @param stream Streamer struture, all the logic into one place
 */
esp_err_t streamer_send(struct streamer* stream);
