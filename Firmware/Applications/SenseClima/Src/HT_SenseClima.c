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

/* Function prototypes  ------------------------------------------------------------------*/

/*!******************************************************************
 * \fn static void HT_YieldThread(void *arg)
 * \brief Thread created as MQTT background.
 *
 * \param[in]  void *arg                    Thread parameter.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_YieldThread(void *arg);

/*!******************************************************************
 * \fn static void HT_Yield_Thread(void *arg)
 * \brief Creates a thread that will be the MQTT background.
 *
 * \param[in]  void *arg                    Parameters that will be used in the created thread.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_Yield_Thread(void *arg);

/*!******************************************************************
 * \fn static void HT_FSM_MQTTWritePayload(uint8_t *ptr, uint8_t size)
 * \brief Copy the *ptr content to the mqtt_payload.
 *
 * \param[in]  uint8_t *ptr                 Pointer with the content that will be copied.
 * \param[in]  uint8_t size                 Buffer size.
 * \param[out] none
 *
 * \retval none.
 *******************************************************************/
static void HT_FSM_MQTTWritePayload(uint8_t *ptr, uint8_t size);

/*!******************************************************************
 * \fn static HT_ConnectionStatus HT_FSM_MQTTConnect(void)
 * \brief Connects the device to the MQTT Broker and returns the connection
 * status.
 *
 * \param[in]  none
 * \param[out] none
 *
 * \retval Connection status.
 *******************************************************************/
static HT_ConnectionStatus HT_FSM_MQTTConnect(void);

/* ---------------------------------------------------------------------------------------*/

static MQTTClient mqttClient;
static Network mqttNetwork;

// Buffer that will be published.
static uint8_t mqtt_payload[128] = {"Undefined Button"};
static uint8_t mqttSendbuf[HT_MQTT_BUFFER_SIZE] = {0};
static uint8_t mqttReadbuf[HT_MQTT_BUFFER_SIZE] = {0};

static const char clientID[] = {"SIP_HTNB32L-XXX"};
static const char username[] = {""};
static const char password[] = {""};

// MQTT broker host address
static const char addr[] = {"131.255.82.115"};
static char topic[25] = {0};

// FSM state.
volatile HT_FSM_States state = HT_WAIT_FOR_BUTTON_STATE;

// Buffer where the digital twin messages will be stored.
static uint8_t subscribe_buffer[HT_SUBSCRIBE_BUFF_SIZE] = {0};

void HT_FSM_SetSubscribeBuff(uint8_t *buff, uint8_t payload_len)
{
    memcpy(subscribe_buffer, buff, payload_len);
}

static HT_ConnectionStatus HT_FSM_MQTTConnect(void) {

    // Connect to MQTT Broker using client, network and parameters needded. 
    if(HT_MQTT_Connect(&mqttClient, &mqttNetwork, (char *)addr, HT_MQTT_PORT, HT_MQTT_SEND_TIMEOUT, HT_MQTT_RECEIVE_TIMEOUT,
                (char *)clientID, (char *)username, (char *)password, HT_MQTT_VERSION, HT_MQTT_KEEP_ALIVE_INTERVAL, mqttSendbuf, HT_MQTT_BUFFER_SIZE, mqttReadbuf, HT_MQTT_BUFFER_SIZE)) {
        return HT_NOT_CONNECTED;   
    }

    printf("MQTT Connection Success!\n");

    return HT_CONNECTED;
}

void HT_FSM_MQTTPublishDHT22state(void)
{
    float temperature, humidity;
    int dht_status = DHT22_Read(&temperature, &humidity);

    if (dht_status == 0)
    {
        char temp_payload[16];
        char hum_payload[16];

        // WORKAROUND: Convert float para string manualmente para evitar problemas
        // com a falta de suporte a '%f' em snprintf em ambientes embarcados.
        int temp_int_x10 = (int)(temperature * 10);
        int hum_int_x10 = (int)(humidity * 10);

        // Formata a string como "parte_inteira.parte_decimal"
        snprintf(temp_payload, sizeof(temp_payload), "%d.%d", temp_int_x10 / 10, temp_int_x10 % 10);
        snprintf(hum_payload, sizeof(hum_payload), "%d.%d", hum_int_x10 / 10, hum_int_x10 % 10);

        HT_MQTT_Publish(&mqttClient, "hana/prototipagem/senseclima/sensor01/temperature", (uint8_t *)temp_payload, strlen(temp_payload), QOS0, 0, 0, 0);
        HT_MQTT_Publish(&mqttClient, "hana/prototipagem/senseclima/sensor01/humidity", (uint8_t *)hum_payload, strlen(hum_payload), QOS0, 0, 0, 0);
        printf("DHT22 published: temp=%s, hum=%s\n", temp_payload, hum_payload); // Agora os valores devem aparecer aqui
    }
    else
    {
        printf("Erro ao ler DHT22 (codigo: %d)\n", dht_status);
    }
    state = HT_WAIT_FOR_BUTTON_STATE;
}

void HT_Fsm(void)
{

    // Initialize MQTT Client and Connect to MQTT Broker defined in global variables
    if (HT_FSM_MQTTConnect() == HT_NOT_CONNECTED)
    {
        printf("\n MQTT Connection Error!\n");
        while (1)
            ;
    }

    // Init irqn after connection
    HT_GPIO_ButtonInit();

    // HT_MQTT_Subscribe(&mqttClient, topic_blueled_sw, QOS0);
    // HT_MQTT_Subscribe(&mqttClient, topic_whiteled_sw, QOS0);

    // HT_Yield_Thread(NULL);

    // Led to sinalize connection stablished
    // HT_LED_GreenLedTask(NULL);

    // Ldr Task
    // HT_LDR_Task(NULL);

    // Btn Task()
    // HT_Btn_Thread_Start(NULL);

    printf("Executing fsm...\n");
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/
