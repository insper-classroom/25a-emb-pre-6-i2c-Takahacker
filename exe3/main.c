#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <queue.h>

#include "pico/stdlib.h"
#include <stdio.h>

#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "mpu6050.h"

const int I2C_SDA_GPIO = 20;
const int I2C_SCL_GPIO = 21;
const int BMP280_I2C_ADDRESS = 0x76; // Default I2C address for BMP280

void i2c_task(void *p)
{
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(I2C_SDA_GPIO, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_GPIO, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_GPIO);
    gpio_pull_up(I2C_SCL_GPIO);

    // Read ID chip BMP280
    uint8_t buffer[1];
    i2c_write_blocking(i2c_default, BMP280_I2C_ADDRESS, (uint8_t[]){0xD0}, 1, true); // Register address for chip ID
    i2c_read_blocking(i2c_default, BMP280_I2C_ADDRESS, buffer, 1, false);
    printf("BMP280 ID: 0x%X \n", buffer[0]);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

int main()
{
    stdio_init_all();

    xTaskCreate(i2c_task, "i2c task", 4095, NULL, 1, NULL);
    vTaskStartScheduler();

    while (true)
    {
    }
}
