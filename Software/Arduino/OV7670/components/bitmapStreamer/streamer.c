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
#include <streamer.h>

static const char *TAG = "TCP Streamer";

esp_err_t streamer_init(streamer stream){
    /*Check if structure is ready*/
    if(stream.init_done){
        return ESP_ERR_BITMAP_STREAM_ALREADY_INIT;
    }

    #ifdef CONFIG_STREAM_IPV6
        return ESP_ERR_BITMAP_STREAM_FUN_NOT_IMPLEM;
    #else // IPV4
        stream.socketAddress.sin_addr.s_addr = inet_addr(HOST_IP_ADDR);
        stream.socketAddress.sin_family = AF_INET;
        stream.socketAddress.sin_port = htons(PORT);
        addr_family = AF_INET;
        ip_protocol = IPPROTO_IP;
        inet_ntoa_r(stream.socketAddress.sin_addr, addr_str, sizeof(addr_str) - 1);
    #endif

}