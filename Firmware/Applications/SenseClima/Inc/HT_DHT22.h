#ifndef __HT_DHT22_H__
#define __HT_DHT22_H__

#include <stdint.h>
#include "bsp.h" 

// DHT22 sensor GPIO configuration
#define DHT22_GPIO_INSTANCE 0
#define DHT22_GPIO_PIN      2
#define DHT22_PAD_ID        13

/**
 * @brief Enum for DHT22_Read function return codes.
 */
typedef enum {
    DHT22_OK                    = 0,   /**< Success */
    DHT22_ERROR_TIMEOUT_START   = -1,  /**< Timeout waiting for sensor response start */
    DHT22_ERROR_TIMEOUT_LOW     = -2,  /**< Timeout waiting for response low pulse end */
    DHT22_ERROR_TIMEOUT_HIGH    = -3,  /**< Timeout waiting for response high pulse end */
    DHT22_ERROR_TIMEOUT_DATA    = -4,  /**< Timeout during data bit reception */
    DHT22_ERROR_CHECKSUM        = -5   /**< Checksum mismatch */
} DHT22_Status;

/**
 * @brief Initializes the GPIO pin for the DHT22 sensor.
 */
void DHT22_Init(void);

/**
 * @brief Reads temperature and humidity from the DHT22 sensor.
 * @param temperature Pointer to store the read temperature.
 * @param humidity Pointer to store the read humidity.
 * @return DHT22_OK on success, or an error code on failure.
 */
int DHT22_Read(float *temperature, float *humidity);

#endif // __HT_DHT22_H__