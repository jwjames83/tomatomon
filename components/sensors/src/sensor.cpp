#include "sensor.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "esp_crc.h"

Sensor::Sensor()
{
    // Configure port

    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    // Configure UART parameters
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_1, &uart_config));
}

void Sensor::query_sensor()
{
    request_message message;

    message.address_code = 0x01;
    message.function_code = 0x03;
    message.starting_address = REG_MOISTURE_CONTENT_RO;
    message.data_length = 9;
}

void Sensor::set_baud_rate(Baud baud)
{

}

void Sensor::create_message(request_message* message)
{
    uint8_t*  msg_bytes = new uint8_t[8]{
        message->address_code,
        message->function_code,
        message->starting_address >> 8,
        message->starting_address & 0xff,
        message->data_length >> 8,
        message->data_length & 0xff,
        0, 0
    };
    
    uint16_t x = esp_crc16_be (0, msg_bytes, 6);
    msg_bytes[6] = x >> 8;
    msg_bytes[7] = x & 0xff;
    
    uart_write_bytes (UART_NUM_1, msg_bytes, 8);
    delete msg_bytes;
}