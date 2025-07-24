/**
 *
 * Copyright (c) 2023 HT Micron Semicondutores S.A.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "HT_SenseClima.h"
#include <stdio.h>    // Required for printf
#include <string.h>   // Required for memset, memcpy, strlen
#include "HT_DHT22.h" // Required for DHT22_Init, DHT22_Read
#include "slpman_qcx212.h" // Required for sleep management functions

/* Function prototypes  ------------------------------------------------------------------*/

/**
 * @brief Connects the device to the MQTT Broker and returns the connection status.
 * @return Connection status (HT_CONNECTED or HT_NOT_CONNECTED).
 */
static HT_ConnectionStatus HT_FSM_MQTTConnect(void);

/**
 * @brief Thread function for reading DHT22 sensor data and publishing it.
 * @param arg Thread parameter (unused).
 */
static void HT_DhtThread(void *arg);

/**
 * @brief Creates a thread for DHT22 sensor reading and publishing.
 * @param arg Parameters for the created thread (unused).
 */
static void HT_Dht_Thread(void *arg);

/* ---------------------------------------------------------------------------------------*/

static MQTTClient mqttClient;
static Network mqttNetwork;

static uint8_t mqttSendbuf[HT_MQTT_BUFFER_SIZE] = {0};
static uint8_t mqttReadbuf[HT_MQTT_BUFFER_SIZE] = {0};

static const char clientID[] = {"SIP_HTNB32L-XXX"};
static const char username[] = {""};
static const char password[] = {""};

static const char addr[] = {"131.255.82.115"};

static const char topic_temperature[] = {"hana/prototipagem/senseclima/01/temperature"};
static const char topic_humidity[] = {"hana/prototipagem/senseclima/01/humidity"};
static const char topic_interval[] = {"hana/prototipagem/senseclima/01/interval"};

#define TIMER_ID 0

uint8_t voteHandle = 0xFF;
extern uint8_t mqttEpSlpHandler;

static StaticTask_t dht_thread, sleep_thread;
static uint8_t dhtTaskStack[TASK_STACK_SIZE], sleepTaskStack[1024 * 2];

/**
 * @brief Converts time components (days, hours, minutes, seconds) into total milliseconds.
 *
 * This function also includes a check for maximum supported time by the device,
 * defaulting to 30 seconds if the input exceeds approximately 580 hours.
 *
 * @param days Number of days.
 * @param hours Number of hours.
 * @param min Number of minutes.
 * @param sec Number of seconds.
 * @return Total time in milliseconds.
 */
uint64_t time_ms(int days, int hours, int min, int sec)
{
    uint64_t total_ms = 0;

    total_ms += (uint64_t)days * 86400000ULL;
    total_ms += (uint64_t)hours * 3600000ULL;
    total_ms += (uint64_t)min * 60000ULL;
    total_ms += (uint64_t)sec * 1000ULL;

    if (total_ms > 2088000000ULL)
    { // Approximately 580 hours in milliseconds
        printf("\nTime exceeds the maximum supported by the device!\n");
        printf(" Default value of 30s will be used.\n");
        return 30 * 1000ULL;
    }

    return total_ms;
}

/**
 * @brief Callback function executed before entering hibernate state.
 * @param pdata User data pointer (unused).
 * @param state Low power state.
 */
void beforeHibernateCb(void *pdata, slpManLpState state)
{
    printf("[Callback] Before Hibernate\n");
}

/**
 * @brief Callback function executed after waking up from hibernate state.
 * @param pdata User data pointer (unused).
 * @param state Low power state.
 */
void afterHibernateCb(void *pdata, slpManLpState state)
{
    printf("[Callback] Woke up from Hibernate\n");
}

/**
 * @brief Enters a specified sleep mode with power saving configurations.
 *
 * This function sets up modem function, SIM sleep, PMU sleep mode, registers
 * hibernate callbacks, enables sleep, and starts an RTC timer for wakeup.
 * The system is expected to enter sleep automatically.
 *
 * @param mode The desired sleep state (e.g., SLP_HIB_STATE).
 */
void sleepWithMode(slpManSlpState_t mode)
{
    printf("\n=== Entering Sleep Mode %d===\n", mode);

    appSetCFUN(0); // Set modem to minimum functionality.
    appSetEcSIMSleepSync(1); // Enable SIM sleep.

    slpManSetPmuSleepMode(true, mode, false); // Set PMU sleep mode.

    // 1. Mode setup
    slpManApplyPlatVoteHandle("SLEEP_TEST", &voteHandle);

    // Register user-defined backup and restore callbacks for hibernate.
    slpManRegisterUsrdefinedBackupCb(beforeHibernateCb, NULL, SLPMAN_HIBERNATE_STATE);
    slpManRegisterUsrdefinedRestoreCb(afterHibernateCb, NULL, SLPMAN_HIBERNATE_STATE);

    // Enable sleep mode using the platform vote handle.
    slpManPlatVoteEnableSleep(voteHandle, mode);

    // slpManPlatVoteDisableSleep(mqttEpSlpHandler, SLP_STATE_MAX); // Commented out in original.

    // Activate RTC timer as wakeup source.
    uint64_t interval_ms = time_ms(0, 0, 0, 30);
    slpManDeepSlpTimerStart(TIMER_ID, interval_ms);

    // Passive wait - the system should enter sleep automatically.
    while (1)
    {
        printf("Hibernating ....");
        osDelay(2000); // After the timer expires, the system wakes up and messages are displayed.
    }
}

/**
 * @brief Thread function for reading DHT22 sensor data and publishing it.
 * @param arg Thread parameter (unused).
 */
static void HT_DhtThread(void *arg)
{
    float temp = 0.0f, humi = 0.0f;
    char tempString[10], humString[10];

    while (1)
    {
        int dht_status = DHT22_Read(&temp, &humi); // Read sensor data.

        if (dht_status == DHT22_OK) // Check if DHT22_Read was successful.
        {
            // Convert float temperature and humidity to string format.
            int temp_int = (int)(temp * 10);
            int hum_int = (int)(humi * 10);
            sprintf(tempString, "%d.%d", temp_int / 10, temp_int % 10);
            sprintf(humString, "%d.%d", hum_int / 10, hum_int % 10);

            printf("\nTemperature: %s°C | Humidity: %s %%\n", tempString, humString);

            while (1) // Loop until connected and published.
            {
                while (!mqttClient.isconnected)
                {
                    if (HT_FSM_MQTTConnect() == HT_NOT_CONNECTED)
                    {
                        printf("\nMQTT Connection Error! Retrying in 5 seconds...\n");
                        osDelay(5000);
                    }
                }

                HT_MQTT_Publish(&mqttClient, (char *)topic_temperature, (uint8_t *)tempString, strlen(tempString), QOS0, 0, 0, 0);
                osDelay(2000);
                HT_MQTT_Publish(&mqttClient, (char *)topic_humidity, (uint8_t *)humString, strlen(humString), QOS0, 0, 0, 0);
                osDelay(2000);
                printf("\nValues Published...\n");

                break;
            }

            printf("\nInitiating deep sleep process.\n");
            sleepWithMode(SLP_HIB_STATE); // Enter deep sleep.
        }

        osDelay(1000); // Delay before next sensor read attempt.
    }
}

/**
 * @brief Creates a thread for DHT22 sensor reading and publishing.
 * @param arg Parameters for the created thread (unused).
 */
static void HT_Dht_Thread(void *arg)
{
    osThreadAttr_t task_attr;

    memset(&task_attr, 0, sizeof(task_attr));
    memset(dhtTaskStack, 0xA5, TASK_STACK_SIZE);
    task_attr.name = "dht_thread";
    task_attr.stack_mem = dhtTaskStack;
    task_attr.stack_size = TASK_STACK_SIZE;
    task_attr.priority = osPriorityNormal;
    task_attr.cb_mem = &dht_thread;
    task_attr.cb_size = sizeof(StaticTask_t);

    osThreadNew(HT_DhtThread, NULL, &task_attr);
}

/**
 * @brief Connects the device to the MQTT Broker.
 * @return Connection status (HT_CONNECTED or HT_NOT_CONNECTED).
 */
static HT_ConnectionStatus HT_FSM_MQTTConnect(void)
{
    // Attempt to connect to the MQTT Broker using specified client, network, and parameters.
    if (HT_MQTT_Connect(&mqttClient, &mqttNetwork, (char *)addr, HT_MQTT_PORT, HT_MQTT_SEND_TIMEOUT, HT_MQTT_RECEIVE_TIMEOUT,
                        (char *)clientID, (char *)username, (char *)password, HT_MQTT_VERSION, HT_MQTT_KEEP_ALIVE_INTERVAL, mqttSendbuf, HT_MQTT_BUFFER_SIZE, mqttReadbuf, HT_MQTT_BUFFER_SIZE))
    {
        printf("MQTT Connection Failed!\n");
        return HT_NOT_CONNECTED;
    }

    printf("MQTT Connection Success!\n");

    return HT_CONNECTED;
}

/**
 * @brief Manages intervals based on received MQTT payload and topic.
 *
 * This function currently prints the received message and topic.
 *
 * @param payload Pointer to the received payload data.
 * @param payload_len Length of the payload.
 * @param topic Pointer to the received topic string.
 * @param topic_len Length of the topic string.
 */
void interval_manager(uint8_t *payload, uint8_t payload_len,
                      uint8_t *topic, uint8_t topic_len)
{
    // The original function only prints the message and topic.
    printf("\nMessage: [%s] | Topic: [%s]\n", payload, topic);
}

/**
 * @brief Implements the Finite State Machine for the SenseClima application.
 *
 * Connects to the MQTT Broker, subscribes to topics, and handles data publishing
 * (e.g., sensor data) and subscribed messages.
 */
void HT_Fsm(void)
{
    printf("\nAttempting to connect to MQTT Client...");

    // Loop until MQTT client is connected.
    while (!mqttClient.isconnected)
    {
        if (HT_FSM_MQTTConnect() == HT_NOT_CONNECTED)
        {
            printf("\nMQTT Connection Error! Retrying in 5 seconds...\n");
            osDelay(5000);
        }
    }

    // Subscribe to the interval topic.
    HT_MQTT_Subscribe(&mqttClient, topic_interval, QOS0);

    printf("\nInitializing DHT sensor...\n");
    DHT22_Init(); // Initialize the DHT22 sensor.

    HT_Dht_Thread(NULL); // Create and start the DHT sensor reading thread.

    printf("Executing SenseClima application.\n");
    while (1)
    {
        osDelay(100); // Main loop delay.
    }
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
