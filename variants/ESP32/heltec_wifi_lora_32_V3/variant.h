// Heltec wifi lora 32 V3

// General
#define LED_PIN 35
#define VEXT_PIN 36 // low = ON, needed for display

// SX1262
#define RADIO0_TYPE lora
#define RADIO0_BAND 868

#define SX126X_CS 8
#define SX126X_DIO1 14
#define SX126X_RESET 12
#define SX126X_BUSY 13

// For LORA, spi 0
#define PIN_SPI_MISO 11
#define PIN_SPI_MOSI 10
#define PIN_SPI_SCK 9

// SSD1306
#define OLED0_SDA 17
#define OLED0_SCL 18
#define OLED0_RST 21
#define I2C_ADDR 0x3C

void variantSetDefaultInterfaces();