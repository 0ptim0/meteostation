#include "stm32f401_conf.h"

void vMeteoMeasure(void *pvParameters) {
    SSD1306_Init();
    SSD1306_DisplayClear();
    BME280_Init();
    BME280_Settings(BME280_Oversampling_X16, BME280_Oversampling_X16, BME280_Oversampling_X16);
    for(;;) {
            BME280_Measure();
            SSD1306_SetCursorPage(0,0);
            SSD1306_PrintString("METEO");
            SSD1306_SetCursorPage(1,0);
            SSD1306_PrintString("Temp = ");
            SSD1306_PrintNum(BME280.Measurement.Temperature / 100.0);
            SSD1306_PrintString(" C");
            SSD1306_SetCursorPage(2,0);
            SSD1306_PrintString("Humi = ");
            SSD1306_PrintNum(BME280.Measurement.Humiduty / 1024.0);
            SSD1306_PrintString(" %");
            SSD1306_SetCursorPage(3,0);
            SSD1306_PrintString("Pres = ");
            SSD1306_PrintNum(BME280.Measurement.Pressure * 0.00750062 / 256);
            SSD1306_PrintString(" mm/Hg");
            vTaskDelay(1000);
    }
}

void vMeteoCLI(void *pvParameters) {
    for(;;) {
    	CLI_Transmit();
    }
}

void vMeteo(void) {
    gpio_config();
    USART_InitOnce();
    I2C_InitOnce();
    xTaskCreate(vMeteoMeasure, "MeteoMeasure", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreate(vMeteoCLI, "MeteoTransmit", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    vTaskStartScheduler();
}
