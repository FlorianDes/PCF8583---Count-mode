#include <PCF8583.h>
#include <Wire.h>

bool PCF8583::begin(uint8_t addr, TwoWire *thewire)
{
    bool status = false;
    _address = addr >> 1;
    _wire = thewire;
    _wire->begin();
    _wire->beginTransmission(_address);
    status = _wire->endTransmission();
    Serial.println(status);
    return status == 0;
}

bool PCF8583::setCount(unsigned long count)
{
    bool status = false;
    stop();
    Wire.beginTransmission(_address);
    Wire.write(LOCATION_COUNTER);
    Wire.write(byte2bcd(count % 100));
    Wire.write(byte2bcd((count / 100) % 100));
    Wire.write(byte2bcd((count / 10000) % 100));
    status = Wire.endTransmission();
    start();
    return status == 0;
}

void PCF8583::setMode(uint8_t mode)
{
    uint8_t control = read(LOCATION_CONTROL);

    control = (control & ~MODE_TEST) | (mode & MODE_TEST);
    write(LOCATION_CONTROL, control);
}

uint8_t PCF8583::read(uint8_t reg)
{
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.endTransmission();
    Wire.requestFrom(_address, (uint8_t)1);
    return Wire.read();
}

bool PCF8583::write(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(_address);
    Wire.write(reg);
    Wire.write(value);
    return Wire.endTransmission() == 0;
}

unsigned long PCF8583::getCount()
{
    Wire.beginTransmission(_address);
    Wire.write(LOCATION_COUNTER);
    Wire.endTransmission();
    Wire.requestFrom(_address, (uint8_t)3);

    unsigned long count = 0;
    count = bcd2byte(Wire.read());
    count = count + bcd2byte(Wire.read()) * 100L;
    count = count + bcd2byte(Wire.read()) * 10000L;
    lastRain = millis();
    return count;
}

uint8_t PCF8583::bcd2byte(uint8_t value)
{
    return ((value >> 4) * 10) + (value & 0x0f);
}

uint8_t PCF8583::byte2bcd(uint8_t value)
{
    return ((value / 10) << 4) + (value % 10);
}

void PCF8583::stop()
{
    uint8_t control = read(LOCATION_CONTROL);
    control |= 0x80;
    write(LOCATION_CONTROL, control);
}

void PCF8583::start()
{
    uint8_t control = read(LOCATION_CONTROL);
    control &= 0x7F;
    write(LOCATION_CONTROL, control);
}