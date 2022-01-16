#include "freertos/FreeRTOS.h"
#include "driver/uart.h"

typedef struct
{
    uint address_code;
    uint function_code;
    uint16_t starting_address;
    uint16_t data_length;
    uint crc_low;
    uint crc_high;
} request_message_t;

class Sensor
{
public:
    /**
     * @brief Enum for baud rates
     * 
     */
    typedef enum
    {
        BPS_2400 = 0x0000,
        BPS_4800 = 0x0001,
        BPS_9600 = 0x0002, // Default
        BPS_DEFAULT = BPS_9600,
    } Baudrate;

    /* ------------------------ funcs ------------------------ */
    /**
     * @brief Performs a query of all values on the sensor
     * 
     */
    void query_sensor();

    /**
     * @brief Get the current baud rate
     * 
     * @return Baud A baud enum value
     */
    Baudrate get_baud_rate();

    Sensor();
    Sensor(int uart_num, Baudrate baudrate);
    ~Sensor();

private:
    const char *TAG = "Soil Sensor";

    /* ------------------------ types ------------------------ */
    /**
     * @brief Register addresses for reading moisture, temperature, conductivity, pH, NPK, salinity, and TDS (R)
     * 
     */
    typedef enum
    {
        MOISTURE_CONTENT = 0x0000,
        TEMPERATURE = 0x0001,
        CONDUCTIVITY = 0x0002,
        PH_VALUE = 0x0003,
        NITROGEN_CONTENT = 0x0004,
        PHOSPHORUS_CONTENT = 0x0005,
        POTASSIUM_CONTENT = 0x0006,
        SALINITY = 0x0007,
        TDS = 0x0008,
    } register_measurement;

    /**
     * @brief Register addresses for conductivity, salinity, temperature, moisture, conductivity, and pH coefficient and calibration values
     * 
     */
    typedef enum
    {
        CONDUCTIVITY_TMP_COEFFICIENT = 0x0023,
        SALINITY_COEFFICIENT = 0x0024,
        TEMPERATURE_CALIBRATION_VALUE = 0x0050,
        WATER_CONTENT_CALIBRATION_VALUE = 0x0051,
        CONDUCTIVITY_CALIBRATION_VALUE = 0x0052,
        PH_CALIBRATION_VALUE = 0x053,
    } registers_calibration;

    /**
     * @brief Register addresses for nitrogen coefficient and calibration values
     * 
     */
    typedef enum
    {
        NITROGEN_CONTENT_COEFFICIENT_HIGH = 0x4E8,
        NITROGEN_CONTENT_COEFFICIENT_LOW = 0x4E9,
        NITROGEN_CONTENT_CALIBRATION_VALUE = 0x4E9,
    } registers_nitrogen;

    /**
     * @brief Register addresses for potassium coefficient and calibration values
     * 
     */
    typedef enum
    {
        POTASSIUM_CONTENT_COEFFICIENT_HIGH = 0x4FC,
        POTASSIUM_CONTENT_COEFFICIENT_LOW = 0x4FD,
        POTASSIUM_CONTENT_CALIBRATION_VALUE = 0x4FE,
    } registers_potassium;

    /**
     * @brief Register addresses for phosphorus coefficient and calibration values
     * 
     */
    typedef enum
    {
        PHOSPHORUS_CONTENT_COEFFICIENT_HIGH = 0x4F2,
        PHOSPHORUS_CONTENT_COEFFICIENT_LOW = 0x4F3,
        PHOSPHORUS_CONTENT_CALIBRATION_VALUE = 0x4F4,
    } register_phosphorus;

    /**
     * @brief Register addresses for device settings
     * 
     */
    typedef enum
    {
        DEVICE_ADDRESS = 0x07D0,
        DEVICE_BAUD_RATE = 0x07D1,
    } registers_settings;

    int m_uart_num = UART_NUM_1;

    void write_message_bytes(request_message_t *message);
    void send_bytes(uint8_t *bytes);
    static void receive_task(void *params);
};