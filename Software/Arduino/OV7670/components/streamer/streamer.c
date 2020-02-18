/**
 * This file is part of AUGMAN smart catapult project.
 * Copyright (c) 2020 Pietro Marchini <pietro.marchini94@gmail.com>
 * This work is licensed under the MIT license, see the file LICENSE for details.
 *
 * TCP bitmap streamer.
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "streamer.h"

static const char *TAG = "TCP Bitmap Streamer";

esp_err_t streamer_init(struct streamer *stream)
{
    /*Check if structure is ready*/
    if (stream->init_done && !stream->connection_error)
    {
        return ESP_ERR_STREAM_ALREADY_INIT;
    }

    stream->connection_error = false;
    stream->connected = false;
    stream->init_done = false;

#ifdef CONFIG_STREAM_IPV6
    return ESP_ERR_STREAM_FUN_NOT_IMPLEM;
#else // IPV4
    stream->socketAddress.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
    stream->socketAddress.sin_family = AF_INET;
    stream->socketAddress.sin_port = htons(CONFIG_STREAM_PORT);
    stream->addr_family = AF_INET;
    stream->ip_protocol = IPPROTO_IP;
    inet_ntoa_r(stream->socketAddress.sin_addr, stream->addr_str, sizeof(stream->addr_str) - 1);
#endif

    stream->sock = socket(stream->addr_family, SOCK_STREAM, stream->ip_protocol);
    if (stream->sock < 0)
    {
        ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
        return ESP_ERR_STREAM_SOCKET_ERROR;
    }
    ESP_LOGI(TAG, "Socket created, connecting to %s:%d", HOST_IP_ADDR, CONFIG_STREAM_PORT);

    int err = connect(stream->sock, (struct sockaddr *)&stream->socketAddress, sizeof(stream->socketAddress));
    if (err != 0)
    {
        ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
        return ESP_ERR_STREAM_SOCKET_ERROR;
    }
    ESP_LOGI(TAG, "Successfully connected");

    stream->connected = true;
    stream->init_done = true;
    return ESP_OK;
}

esp_err_t streamer_send(struct streamer *stream)
{
    if (stream->packet.header.counter == 0 || stream->packet.header.frameTick == 0 || stream->packet.header.frameSize == 0)
    {
        return ESP_ERR_STREAM_HEADER_ERROR;
    }
    //Verificare cosa viene inviato puntando una struttura contenente puntatori ai buffer
    int err = send(stream->sock, &stream->packet, stream->packet.sizeofPacket, 0);

    if (err < 0)
    {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        stream->connection_error = true;
        stream->connected = false;
        return streamer_init(stream);
    }
    return ESP_OK;
}

esp_err_t streamer_prepare_packet_data(struct streamer *stream, void* data, void* bitmap, uint32_t sizeOfData, uint32_t sizeOfHeader )
{
    if(data == NULL || bitmap == NULL)
    {
        return ESP_ERR_STREAM_BUFFER_ERROR;
    }
    /*FrameData*/
    stream->packet.data.bufferPointer = data;
    stream->packet.data.sizeofBuffer  = sizeOfData;
    /*BitmapHeader*/
    stream->packet.data.bitmapHeader  = bitmap;
    stream->packet.data.sizeofHeader  = sizeOfHeader;

    return ESP_OK;
}



