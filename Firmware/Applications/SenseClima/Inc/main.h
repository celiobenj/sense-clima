/*

           _           _       _  _    ___  
          | |         | |     | || |  / _ \ 
          | |     __ _| |__   | || |_| | | |
          | |    / _` | '_ \  |__   _| | | |
          | |___| (_| | |_) |    | |_| |_| |
          |______\__,_|_.__/     |_(_)\___/  
 =================== Lab 4.0 =====================

 Copyright (c) 2025 Laboratório 4.0 - Projeto Educacional
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

*/
#pragma once

#include "htnb32lxxx_hal_usart.h"
#include "HT_SenseClima.h"
#include "HT_GPIO_Api.h"

#include "bsp.h"
#include "HT_BSP_Custom.h"
#include "osasys.h"
#include "ostask.h"
#include "queue.h"
#include "ps_event_callback.h"
#include "cmisim.h"
#include "cmimm.h"
#include "cmips.h"
#include "sockets.h"
#include "psifevent.h"
#include "ps_lib_api.h"
#include "lwip/netdb.h"
#include "debug_log.h"
#include "MQTTClient.h"
#include "plat_config.h"
#include "debug_trace.h"
#include "hal_uart.h"
#include "HT_MQTT_Api.h"
#include "flash_qcx212.h"
#include "flash_qcx212_rt.h"
#include "slpman_qcx212.h"
#include "FreeRTOS.h"
#include "netmgr.h"
#include <stdio.h>
#include "cmsis_os2.h"
#include "HT_MQTT_Api.h"
#include "mw_config.h"
#include "pmu_qcx212.h"
#include "ps_lib_api.h"
#include "flash_qcx212.h"

#include "HT_SenseClima.h"
#include "HT_GPIO_Api.h"
#include "HT_DHT22.h"

#define QMSG_ID_BASE               (0x160)
#define QMSG_ID_NW_IPV4_READY      (QMSG_ID_BASE)       /**< Message ID for IPv4 network ready event. */
#define QMSG_ID_NW_IPV6_READY      (QMSG_ID_BASE + 1)   /**< Message ID for IPv6 network ready event. */
#define QMSG_ID_NW_IPV4_6_READY    (QMSG_ID_BASE + 2)   /**< Message ID for IPv4/IPv6 network ready event. */
#define QMSG_ID_NW_DISCONNECT      (QMSG_ID_BASE + 3)   /**< Message ID for network disconnect event. */
#define QMSG_ID_SOCK_SENDPKG       (QMSG_ID_BASE + 4)   /**< Message ID for socket send package event. */
#define QMSG_ID_SOCK_RECVPKG       (QMSG_ID_BASE + 5)   /**< Message ID for socket receive package event. */

#define INIT_TASK_STACK_SIZE        (1024*6)    /**< Stack size for the main application initialization task. */
#define RINGBUF_READY_FLAG          (0x06)      /**< Flag for ring buffer ready. */
#define APP_EVENT_QUEUE_SIZE        (10)        /**< Size of the application event queue. */
#define MAX_PACKET_SIZE             (256)       /**< Maximum packet size. */