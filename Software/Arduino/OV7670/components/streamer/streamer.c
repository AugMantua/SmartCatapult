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

/*
* set IP adress and port for data streaming
*/
esp_err_t streamer_config(
    struct streamer *stream,
    const char *ip,
    uint16_t streamerPort,
    )
{
    stream->socketAddress.sin_addr.s_addr = inet_addr(ip);
    stream->socketAddress.sin_addr.s_addr = htons(streamerPort);
    stream->socketAddress.sin_family = AF_INET;
}

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
    if (stream->socketAddress.sin_addr.s_addr == 0 ||
        stream->socketAddress.sin_port == 0)
    {
        return ESP_ERR_SOCKET_MISSING_CONFIG;
    }
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

/* Send the packet divided into header
*  packet header
*  bitmap header
*  bitmap stream
*  EOF
*  Eof can be removed considering the size of the packet that is set into the headers -> TODO
*/

esp_err_t streamer_send(struct streamer *stream)
{
    char buffer[512];
    if (stream->packet.header.counter == 0 || stream->packet.header.frameTick == 0 )
    {
        return ESP_ERR_STREAM_HEADER_ERROR;
    }
    ESP_LOGI(TAG, "Sending packet to target");
    int err = 0;

    ESP_LOGI(TAG, "Packet dimensions : dataHeaderBitmap = %d, data = %d", stream->packet.header.sizeofBitmapHeader, stream->packet.header.sizeofBuffer);
    err = send(stream->sock, &stream->packet.header, sizeof(stream->packet.header), 0);
    //Invio bitmapHeader
    err = send(stream->sock, stream->packet.data.bitmapHeaderPointer, stream->packet.header.sizeofBitmapHeader, 0);
    //Invio frame
    err = send(stream->sock, stream->packet.data.bufferPointer, stream->packet.header.sizeofBuffer, 0);
    //Invio EOF
    strcpy(buffer,EOF);
    err = send(stream->sock, &buffer, sizeof(buffer), 0);
    /*Verificare*/

    if (err < 0)
    {
        ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
        stream->connection_error = true;
        stream->connected = false;
        return streamer_init(stream);
    }
    ESP_LOGI(TAG, "Packet sended to target");
    return ESP_OK;
}

esp_err_t streamer_prepare_packet_data(struct streamer *stream, void* data, void* bitmapHeader, uint32_t sizeOfData, uint32_t sizeOfHeader )
{
    if(data == NULL || bitmapHeader == NULL)
    {
        return ESP_ERR_STREAM_BUFFER_ERROR;
    }
    /*FrameData*/
    stream->packet.data.bufferPointer = data;
    stream->packet.header.sizeofBuffer  = sizeOfData;
    /*BitmapHeader*/
    stream->packet.data.bitmapHeaderPointer = bitmapHeader;
    stream->packet.header.sizeofBitmapHeader  = sizeOfHeader;

    return ESP_OK;
}



