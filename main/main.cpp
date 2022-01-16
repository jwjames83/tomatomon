#include <stdio.h>
#include <stdlib.h>
#include <sys/reent.h>
#include "esp_log.h"
#include "esp_vfs.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "tusb_console.h"
#include "sdkconfig.h"
#include "driver/gpio.h"

#include "../components/sensors/src/sensor.h"

extern "C" {
    void app_main();
}

static const char *TAG = "TOMATOMON";
static const gpio_num_t LED_GPIO = GPIO_NUM_13;

/**
 * @brief Configure the GPIO for the LED
 *
 */
static void configure_led(void)
{
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
}

/**
 * @brief Set LED state
 *
 * @param state The state for the LED
 */
static void blink_led(uint8_t state)
{
    gpio_set_level(LED_GPIO, state);
}

/**
 * @brief Generic 5 second status blink
 *
 */
static void blink_led_task(void *parameters)
{
    for (;;)
    {
        blink_led(true);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        blink_led(false);
        vTaskDelay(4500 / portTICK_PERIOD_MS);
    }
}


/**
 * @brief Task to poll the sensor on a regular interval
 * 
 * @param parms None shall be passed
 */
void poll_sensor_task(void *parms)
{
    Sensor* sensor = new Sensor ();
    for (;;)
    {
        sensor->query_sensor();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    // Configure tinyusb first
    tinyusb_config_t tusb_cfg = {
        .descriptor = NULL,
        .string_descriptor = NULL,
        .external_phy = false
    }; // the configuration uses default values

    ESP_ERROR_CHECK(tinyusb_driver_install(&tusb_cfg));

    tinyusb_config_cdcacm_t amc_cfg = {
        .usb_dev = TINYUSB_USBDEV_0,
        .cdc_port = TINYUSB_CDC_ACM_0,
        .rx_unread_buf_sz = 64,
        .callback_rx = NULL,
        .callback_rx_wanted_char = NULL,
        .callback_line_state_changed = NULL,
        .callback_line_coding_changed = NULL};

    ESP_ERROR_CHECK(tusb_cdc_acm_init(&amc_cfg));
   
    esp_tusb_init_console(TINYUSB_CDC_ACM_0); // log to usb

    // Holding off to give us time to connect
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    ESP_LOGI(TAG, "Tomato Mon - Program started");

    // Create our tasks
    xTaskCreate(blink_led_task, "Blink task", 8,
                (void *)NULL, tskIDLE_PRIORITY + 1, NULL);

    ESP_LOGI(TAG, "Created blink task");

    xTaskCreate(poll_sensor_task, "Poll task", 256, (void *)NULL, tskIDLE_PRIORITY + 2, NULL);

    vTaskStartScheduler();
    // Let's hope to never get here...
    ESP_LOGI(TAG, "Program errored! Going on forever...");
    for (;;)
        ;
}
