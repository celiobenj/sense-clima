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

/*!
 * \file HT_GPIO_Api.h
 * \brief GPIO API used for HTNB32L-XXX MQTT Example. 
  * \author HT Micron Advanced R&D,
 *         Hêndrick Bataglin Gonçalves, Christian Roberto Lehmen,  Matheus da Silva Zorzeto, Felipe Kalinski Ferreira,
 *         Leandro Borges, Mauricio Carlotto Ribeiro, Henrique Kuhn, Cleber Haack, Eduardo Mendel
 *         Gleiser Alvarez Arrojo
 * 
 * \link https://github.com/htmicron
 * \version 0.1
 * \date February 23, 2023
 */

#ifndef __HT_GPIO_API_H__
#define __HT_GPIO_API_H__

#include "stdint.h"
#include "main.h"
#include "pad_qcx212.h"
#include "gpio_qcx212.h"
#include "cmsis_os2.h"

/**
 * @brief Writes a value to a specified GPIO pin to turn on/off LEDs.
 * @param pin GPIO port number.
 * @param instance GPIO instance.
 * @param value Value to write (LED_ON/LED_OFF).
 */
void HT_GPIO_WritePin(uint16_t pin, uint32_t instance, uint16_t value);

/**
 * @brief Reads the value of a specified GPIO pin.
 * @param pin GPIO pin number.
 * @param instance GPIO instance.
 * @return The value read from the GPIO pin.
 */
uint32_t HT_GPIO_ReadPin(uint16_t pin, uint32_t instance);

// gpio_set_direction is removed as it was specific to button configuration.
// void gpio_set_direction(gpio_pin_direction_t direction);

#endif /* __HT_GPIO_API_H__ */

/************************ HT Micron Semicondutores S.A *****END OF FILE****/