#include "sensor.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_crc.h"


/**
 * @brief A soil sensor UART receive task
 * 
 * @param params A Sensor object
 */
static void receive_task (void* params)
{
    Sensor* thiz = (Sensor*)params;
}

/**
 * @brief Construct a new Sensor
 * 
 */
Sensor::Sensor(int uart_num = UART_NUM_1, Baudrate baudrate = Baudrate::BPS_DEFAULT)
{
    int _baud_rate = 9600;
    m_uart_num = uart_num;

    // Set the baud rate
    if (baudrate != Baudrate::BPS_DEFAULT)
    {
        if (baudrate == Baudrate::BPS_2400)
        {
            _baud_rate = 2400;
        }

        else if (baudrate == Baudrate::BPS_4800)
        {
            _baud_rate = 4800;
        }
    }

    // Configure uart
    uart_config_t uart_config =
        {
            .baud_rate = _baud_rate,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .rx_flow_ctrl_thresh = 122,
        };

    // Config and check
    ESP_ERROR_CHECK(uart_param_config(m_uart_num, &uart_config));

    // Create our recieve task
    xTaskCreate (receive_task, "SoilSenRx", configMINIMAL_STACK_SIZE, (Sensor*) this, tskIDLE_PRIORITY + 5, NULL);
}

Sensor::~Sensor()
{
    
}

/**
 * @brief Query RS-485 sensor values for moisture, temp, conductivity, pH, NPK, salinity, TDS
 * 
 */
void Sensor::query_sensor()
{
    request_message_t message = {
        .address_code = 0x01,
        .function_code = 0x03,
        .starting_address = register_measurement::MOISTURE_CONTENT,
        .data_length = 9,
    };

    write_message_bytes (&message);
}

/**
 * @brief Send a request_message out in bytes on the wire
 * 
 * @param message 
 */
void Sensor::write_message_bytes(request_message_t *message)
{
    uint8_t *msg_bytes = (uint8_t *)pvPortMalloc(8);
    msg_bytes[0] = message->address_code;
    msg_bytes[1] = message->function_code;
    msg_bytes[2] = message->starting_address >> 8;
    msg_bytes[3] = message->starting_address & 0xff;
    msg_bytes[4] = message->data_length >> 8;
    msg_bytes[5] = message->data_length & 0xff;

    uint16_t x = esp_crc16_be(0, msg_bytes, 6);
    msg_bytes[6] = x >> 8;
    msg_bytes[7] = x & 0xff;

    ESP_LOGI (TAG, "Writing to UART1: %02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X", msg_bytes[0],msg_bytes[1],msg_bytes[2],msg_bytes[3],msg_bytes[4],msg_bytes[5],msg_bytes[6],msg_bytes[7]);
    uart_write_bytes(UART_NUM_1, msg_bytes, 8);
    free(msg_bytes);
}