#ifndef PCF8583_h
#define PCF8583_h

#include <Arduino.h>
#include <Wire.h>

#define LOCATION_CONTROL 0x00
#define LOCATION_COUNTER 0x01
#define MODE_EVENT_COUNTER 0x20
#define MODE_TEST 0x30

class PCF8583
{
public:
    PCF8583(){};
    bool begin(uint8_t addr, TwoWire *thewire = &Wire);
    void setMode(uint8_t mode);
    uint8_t getMode();

    bool setCount(unsigned long count);
    unsigned long getCount();

private:
    uint8_t _address;
    TwoWire *_wire;
    uint8_t read(uint8_t reg);
    bool write(uint8_t reg, uint8_t value);
    uint8_t bcd2byte(uint8_t value);
    uint8_t byte2bcd(uint8_t value);
    void start();
    void stop();
};
#endif