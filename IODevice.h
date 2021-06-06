//
// Created by jwscoggins on 5/3/21.
//

#ifndef I960SXCHIPSET_IODEVICE_H
#define I960SXCHIPSET_IODEVICE_H
#include <stdint.h>
#include <string>
#include "ChipsetInteract.h"

class BuiltinIOBaseDevice {
public:
    BuiltinIOBaseDevice(uint32_t offset);
    uint32_t getBaseAddress() const { return baseAddress_; }
    uint32_t getOffset() const { return offset_; }
protected:
    uint32_t offset_;
    uint32_t baseAddress_;
};
/**
 * @brief Manages the builtin led provided by the chipset
 */
class BuiltinLED : public BuiltinIOBaseDevice {
public:
    BuiltinLED(uint32_t offset = 0);
    bool getValue();
    void setValue(bool value);
    void toggle();
private:
    volatile uint8_t& _memory;
};

/**
 * @brief The console on hitagi is very simple, it really only acts as a input/output channel
 */
class BuiltinConsole : public BuiltinIOBaseDevice {
public:
    BuiltinConsole(uint32_t offset = 0x100);
    void flush();
    bool available() const;
    bool availableForWrite() const;
    uint16_t read();
    void write(uint16_t value);
    void write(char c);
    void write(const char* ptr, bool newLine = false);
    inline void writeLine(const char* ptr) { write(ptr, true); }
    inline void writeLine(const std::string& str) { write(str, true); }
    void write(const std::string& str, bool newLine = false);
private:
    struct RawConsoleStructure {
        volatile uint16_t flushPort;
        volatile uint16_t isAvailable;
        volatile uint16_t isAvailableForWriting;
        volatile uint16_t ioPort;
    } __attribute__ ((packed));
private:
    volatile RawConsoleStructure& _memory;
};

class BuiltinTFTDisplay : public BuiltinIOBaseDevice {
public:
    BuiltinTFTDisplay(uint32_t offset = 0x200);
    uint16_t color565(uint8_t r, uint8_t g, uint8_t b);
    inline uint16_t color565(uint32_t packedColor) {
        return color565(packedColor & 0xFF,
                        (packedColor >> 8) & 0xFF,
                        (packedColor >> 16) & 0xFF);
    }
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillScreen(uint16_t color);
    inline void clearScreen() { fillScreen(color565(0,0,0)); }
    inline void drawPixel(int16_t x, int16_t y, uint8_t r, uint8_t g, uint8_t b)  {
        drawPixel(x, y, color565(r, g, b));
    }
    inline void drawPixel(int16_t x, int16_t y, uint32_t rgb) {
        drawPixel(x, y, color565(rgb));
    }
private:
    struct RawTFTCommand {
#define X(type, name) volatile type name ## Port
#define Z(name) X(uint16_t, name)
#define Y(name) X(int16_t, name)
        Y(flush);
        Y(io);
        Y(available);
        Y(availableForWriting);
        Y(command);
        Y(x);
        Y(y);
        Y(w);
        Y(h);
        Y(radius);
        Y(color);
        Y(x0);
        Y(y0);
        Y(x1);
        Y(y1);
        Y(x2);
        Y(y2);
        Y(red);
        Y(green);
        Y(blue);
        Y(doorbell);
        Z(backlight);
        Z(backlightFrequency);
        X(uint32_t, buttons);
        Z(buttonsQuery);
#undef Y
#undef Z
#undef X
    } __attribute__((packed));
private:
    volatile RawTFTCommand& _memory;
};

#endif //I960SXCHIPSET_IODEVICE_H
