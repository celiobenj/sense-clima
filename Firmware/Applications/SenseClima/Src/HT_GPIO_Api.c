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

#include "HT_GPIO_Api.h"

/**
 * @brief Writes a value to a specified GPIO pin.
 * @param pin GPIO port number.
 * @param instance GPIO instance.
 * @param value Value to write (0 for low, non-zero for high).
 */
void HT_GPIO_WritePin(uint16_t pin, uint32_t instance, uint16_t value)
{
    GPIO_PinWrite(instance, 1 << pin, (value ? 1 << pin : 0));
}

/**
 * @brief Reads the value of a specified GPIO pin.
 * @param pin GPIO pin number.
 * @param instance GPIO instance.
 * @return The value read from the GPIO pin (0 or 1).
 */
uint32_t HT_GPIO_ReadPin(uint16_t pin, uint32_t instance)
{
    return GPIO_PinRead(instance, pin);
}

/************************ HT Micron Semicondutores S.A *****END OF FILE****/