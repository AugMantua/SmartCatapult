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
#include "esp_err.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include <bitmap.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define ESP_ERR_STREAM_BASE 0x30000
#define ESP_ERR_STREAM_SOCKET_ERROR      (ESP_ERR_STREAM_BASE + 1)
#define ESP_ERR_STREAM_SEMAPHORE_ERROR   (ESP_ERR_STREAM_BASE + 2)
#define ESP_ERR_STREAM_BUFFER_ERROR      (ESP_ERR_STREAM_BASE + 3)
#define ESP_ERR_STREAM_ALREADY_INIT      (ESP_ERR_STREAM_BASE + 4)
#define ESP_ERR_STREAM_FUN_NOT_IMPLEM    (ESP_ERR_STREAM_BASE + 5)
#define ESP_ERR_STREAM_HEADER_ERROR      (ESP_ERR_STREAM_BASE + 6)


#ifndef HOST_IP_ADDR
#define HOST_IP_ADDR "192.168.1.17"
#endif
/*Standard TCP port for the streaming*/
#ifndef CONFIG_STREAM_PORT
#define CONFIG_STREAM_PORT 11000
#endif

#define EOF "<EOF>"


 /**
 * 
 * header structure definition
 * this header must contain timing information of the bitmap
 * master will reply to the stream with position of the target if found
 * and time of the original frame in order to understand in case speed/acceleration ecc
 * If there is a target into the frame master will set targetInFrame and give % of confident
 */
    struct streamerPacketHeader
    {
        /**/
        uint32_t counter;
        uint32_t frameTick;
        /**/
        bool targetInFrame;
        uint32_t targetConfident;
        /**/
        uint32_t  sizeofBuffer;
        uint32_t  sizeofBitmapHeader;
    } ;

 /**
 * Packet data
 * contains the pointer to the buffer and the size of the frame + rgb565 header
 */
    struct streamerPacketData
    {
        void      *bitmapHeaderPointer;
        void      *bufferPointer;
    } ;

 /**
 * 
 * Packet 
 * contains header, data and total size
 * where total size is header size + data size(where data size is bitmap header + frame data)
 * 
 */
    struct streamerPacket
    {
        struct streamerPacketHeader header;
        struct streamerPacketData   data;
        uint32_t sizeofPacket;
    } ;

/*
* defines a struct in order to have only one object
*/
    struct streamer
    {
        bool                  init_done;
        bool                  connection_error;
        bool                  connected;
        bool                  pointerDefined;
        struct sockaddr_in    socketAddress;
        struct streamerPacket packet;
        int                   sock;
        char                  addr_str[128];
        int                   addr_family;
        int                   ip_protocol;
    } ;

/**
 * @brief Init the streamer, prepares the semaphore and prepare the socket
 * 
 * @return ESP_OK on success
 * 
 * @param stream Streamer structure, all the logic into one place
*/
    esp_err_t streamer_init(struct streamer *stream);

/**
 * @brief TCP send of bitmap stream
 * 
 * @return ESP_OK on success
 * 
 * @param stream Streamer structure, all the logic into one place
 */
    esp_err_t streamer_send(struct streamer *stream);

/**
 * @brief Prepares data to be send
 * 
 * @return ESP_OK on success
 * 
 * @param stream Streamer structure, all the logic into one place
 */
    esp_err_t streamer_prepare_frame_data(struct streamer *stream, void* data, void* bitmapHeader, uint32_t sizeOfData, uint32_t sizeOfHeader);



#ifdef __cplusplus
}
#endif
