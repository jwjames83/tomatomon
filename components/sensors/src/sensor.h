#include "freertos/FreeRTOS.h"

class Sensor
{
public:
    Sensor();
    ~Sensor();

    enum Baud {
        BPS2400,
        BPS4800,
        BPS9600
    };

    /**
     * @brief Performs a query of all values on the sensor
     * 
     */
    void query_sensor();

    /**
     * @brief Set the baud rate of the sensor
     * 
     * @param baud A Baud enum value
     */
    void set_baud_rate (Baud baud);

    /**
     * @brief Get the current baud rate
     * 
     * @return Baud A baud enum value
     */
    Baud get_baud_rate ();



    /* -------------------- Register definitions --------------------*/
    const uint REG_MOISTURE_CONTENT_RO = 0x0000;
    const uint REG_TEMPERATURE_RO = 0x0001;
    const uint REG_CONDUCTIVITY_RO = 0x0002;
    const uint REG_PH_VALUE_RO = 0x0003;
    const uint REG_NITROGEN_CONTENT_RO = 0x0004;
    const uint REG_PHOSPHORUS_CONTENT_RO = 0x0005;
    const uint REG_POTASSIUM_CONTENT_RO = 0x0006;
    const uint REG_SALINITY_RO = 0x0007;
    const uint REG_TDS_RO = 0x0008;

    const uint REG_CONDUCTIVITY_TMP_COEFFICIENT_RW = 0x0023;
    const uint REG_SALINITY_COEFFICIENT_RW = 0x0024;
    const uint REG_TEMPERATURE_CALIBRATION_VALUE_RW = 0x0050;
    const uint REG_WATER_CONTENT_CALIBRATION_VALUE_RW = 0x0051;
    const uint REG_CONDUCTIVITY_CALIBRATION_VALUE_RW = 0x0052;
    const uint REG_PH_CALIBRATION_VALUE_RW = 0x053;

    const uint REG_NITROGEN_CONTENT_COEFFICIENT_HIGH_RW = 0x4E8;
    const uint REG_NITROGEN_CONTENT_COEFFICIENT_LOW_RW = 0x4E9;
    const uint REG_NITROGEN_CONTENT_CALIBRATION_VALUE_RW = 0x4E9;
    
    const uint REG_PHOSPHORUS_CONTENT_COEFFICIENT_HIGH_RW = 0x4F2;
    const uint REG_PHOSPHORUS_CONTENT_COEFFICIENT_LOW_RW = 0x4F3;
    const uint REG_PHOSPHORUS_CONTENT_CALIBRATION_VALUE_RW = 0x4F4;
    
    const uint REG_POTASSIUM_CONTENT_COEFFICIENT_HIGH_RW = 0x4FC;
    const uint REG_POTASSIUM_CONTENT_COEFFICIENT_LOW_RW = 0x4FD;
    const uint REG_POTASSIUM_CONTENT_CALIBRATION_VALUE_RW = 0x4FE;

    const uint REG_DEVICE_ADDRESS_RW = 0x07D0;
    const uint REG_DEVICE_BAUD_RATE_RW = 0x07D1;

    const uint DEVICE_BAUD_2400 = 0x0000;
    const uint DEVICE_BAUD_4800 = 0x0001;
    const uint DEVICE_BAUD_9600 = 0x0002; // Default

private:
    struct request_message
    {
        uint address_code = 0x01;
        uint function_code = 0x03;
        uint16_t starting_address;
        uint16_t data_length;
        uint crc_low;
        uint crc_high;
    };

    void create_message (request_message* message);
};