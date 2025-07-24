/*

  _    _ _______   __  __ _____ _____ _____   ____  _   _
 | |  | |__   __| |  \/  |_   _/ ____|  __ \ / __ \| \ | |
 | |__| |  | |    | \  / | | || |    | |__) | |  | |  \| |
 |  __  |  | |    | |\/| | | || |    |  _  /| |  | | . ` |
 | |  | |  | |    | |  | |_| || |____| | \ \| |__| | |\  |
 |_|  |_|  |_|    |_|  |_|_____\_____|_|  \_\\____/|_| \_|
 =================== Advanced R&D ========================

 Copyright (c) 2023 HT Micron Semicondutores S.A.
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

/**
 * @file HT_SenseClima.h
 * @brief Main application logic for the HTNB32L-XXX SenseClima project.
 *
 * This file defines application-specific data types and function prototypes.
 */

#ifndef __HT_SENSECLIMA_H__
#define __HT_SENSECLIMA_H__

#include "stdint.h"
#include "main.h"
#include "HT_MQTT_Api.h"
#include "MQTTFreeRTOS.h"
#include "bsp.h"
#include "HT_GPIO_Api.h"
#include "cmsis_os2.h"
#include "MQTTClient.h"

/* Defines  ------------------------------------------------------------------*/
#define TASK_STACK_SIZE             (1024*4) /**< Stack size for application tasks, originally named after LED tasks. */
#define HT_MQTT_KEEP_ALIVE_INTERVAL 240      /**< MQTT keep-alive interval in seconds. */
#define HT_MQTT_VERSION             4        /**< MQTT protocol version. */

#if MQTT_TLS_ENABLE == 1
#define HT_MQTT_PORT   8883                               /**< MQTT TCP TLS port. */
#else
#define HT_MQTT_PORT   1883                               /**< MQTT TCP port. */
#endif

#define HT_MQTT_SEND_TIMEOUT    60000                     /**< MQTT TX timeout in milliseconds. */
#define HT_MQTT_RECEIVE_TIMEOUT 60000                     /**< MQTT RX timeout in milliseconds. */
#define HT_MQTT_BUFFER_SIZE     1024                      /**< Maximum MQTT buffer size. */
#define HT_SUBSCRIBE_BUFF_SIZE  6                         /**< Maximum buffer size for MQTT subscribed messages. */

/* Typedefs  ------------------------------------------------------------------*/

/**
 * @brief HTNB32L-XXX connection status.
 */
typedef enum {
    HT_CONNECTED = 0,       /**< Device is connected. */
    HT_NOT_CONNECTED        /**< Device is not connected. */
} HT_ConnectionStatus;

/**
 * @brief States definition for the Finite State Machine (FSM).
 */
typedef enum {
    HT_MQTT_SUBSCRIBE_STATE = 0, /**< State for subscribing to MQTT topics. */
    HT_MQTT_PUBLISH_STATE,       /**< State for publishing MQTT messages. */
    HT_CHECK_SOCKET_STATE,       /**< State for checking socket status. */
    HT_SUBSCRIBE_HANDLE_STATE    /**< State for handling subscribed messages. */
} HT_FSM_States;

/* Functions ------------------------------------------------------------------*/

/**
 * @brief Manages intervals based on received MQTT payload and topic.
 * @param payload Pointer to the received payload data.
 * @param payload_len Length of the payload.
 * @param topic Pointer to the received topic string.
 * @param topic_len Length of the topic string.
 */
void interval_manager(uint8_t *payload, uint8_t payload_len,
    uint8_t *topic, uint8_t topic_len);

/**
 * @brief Implements the Finite State Machine for the SenseClima application.
 *
 * Connects to the MQTT Broker, subscribes to topics, and handles data publishing
 * (e.g., sensor data) and subscribed messages.
 */
void HT_Fsm(void);

#endif /* __HT_SENSECLIMA_H__ */

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
