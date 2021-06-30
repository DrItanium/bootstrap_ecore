//
// Created by jwscoggins on 5/3/21.
//

#include "IODevice.h"
#include "ChipsetInteract.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>


BuiltinIOBaseDevice::BuiltinIOBaseDevice(uint32_t offset) : offset_(offset), baseAddress_(getIOBase0Address(offset)) { }
ChipsetBasicFunctions::ChipsetBasicFunctions(uint32_t offset) : BuiltinIOBaseDevice(offset), _memory(memory<ChipsetRegistersRaw>(baseAddress_)), ledValue_(false) {
    _memory.led = 0;
}
bool
ChipsetBasicFunctions::getLEDValue() {
    return ledValue_;
}
void
ChipsetBasicFunctions::setLEDValue(bool value) {
    if (value != ledValue_) {
        ledValue_ = value;
        _memory.led = (ledValue_ ? 0xFF : 0x00);
    }
}
void
ChipsetBasicFunctions::toggleLED() {
    setLEDValue(!ledValue_);
}
uint8_t
ChipsetBasicFunctions::readPortZGPIO() {
    return _memory.portzGPIO;
}
void
ChipsetBasicFunctions::setPortZGPIO(uint8_t value) {
    _memory.portzGPIO = value;

}
uint8_t
ChipsetBasicFunctions::readPortZGPIOPullup() {
    return _memory.portzGPIOPullup;
}
void
ChipsetBasicFunctions::setPortZGPIOPullup(uint8_t value) {
    _memory.portzGPIOPullup = value;

}
uint8_t
ChipsetBasicFunctions::readPortZGPIOPolarity() {
    return _memory.portzGPIOPolarity;
}
void
ChipsetBasicFunctions::setPortZGPIOPolarity(uint8_t value) {
    _memory.portzGPIOPolarity = value;

}
uint8_t
ChipsetBasicFunctions::readPortZGPIODirection() {
    return _memory.portzGPIODirection;
}
void
ChipsetBasicFunctions::setPortZGPIODirection(uint8_t value) {
    _memory.portzGPIODirection = value;

}
void
ChipsetBasicFunctions::digitalWrite(PortZPins pin, bool value) {

}
void
ChipsetBasicFunctions::pinMode(PortZPins pin, PinModes mode) {

}
bool
ChipsetBasicFunctions::digitalRead(PortZPins pin) {
    return false;
}

bool
ChipsetBasicFunctions::available() const {
    return static_cast<bool>(_memory.consoleAvailablePort);
}
bool
ChipsetBasicFunctions::availableForWrite() const {
    return static_cast<bool>(_memory.consoleAvailableForWritePort);
}

uint16_t
ChipsetBasicFunctions::read() const {
    return _memory.consoleIOPort;
}

void
ChipsetBasicFunctions::write(uint16_t c) {
    _memory.consoleIOPort = c;
}

void
ChipsetBasicFunctions::write(char c) {
    write(static_cast<uint16_t>(c));
}
void
ChipsetBasicFunctions::flush() {
    // doesn't matter what you write as long as you write it
    _memory.consoleFlushPort = 1;
}
void
ChipsetBasicFunctions::write(const char* ptr) {
    write(const_cast<char*>(ptr), strlen(ptr));
}

enum TFTOpcodes {
    None = 0,
    SetRotation,
    InvertDisplay,
    FillRect,
    FillScreen,
    DrawLine,
    DrawRect,
    DrawCircle,
    FillCircle,
    DrawTriangle,
    FillTriangle,
    SetTextSizeSquare,
    SetTextSizeRectangle,
    SetCursor,
    SetTextColor0,
    SetTextColor1,
    SetTextWrap,
    GetWidth,
    GetHeight,
    GetRotation,
    GetCursorX,
    GetCursorY,
    DrawPixel,
    Color565,
    DrawRoundRect,
    FillRoundRect,
};
BuiltinTFTDisplay::BuiltinTFTDisplay(uint32_t offset) : BuiltinIOBaseDevice(offset), _memory(memory<RawTFTCommand>(baseAddress_)) { }

void
BuiltinTFTDisplay::fillScreen(uint16_t color) {
    _memory.commandPort = FillScreen;
    _memory.colorPort = color;
    _memory.doorbellPort = 1;
}
void
BuiltinTFTDisplay::drawPixel(int16_t x, int16_t y, uint16_t color) {
    _memory.commandPort = DrawPixel;
    _memory.colorPort = color;
    _memory.xPort = x;
    _memory.yPort = y;
    _memory.doorbellPort = 1;
}

uint16_t
BuiltinTFTDisplay::color565(uint8_t r, uint8_t g, uint8_t b) {
    _memory.commandPort = Color565;
    _memory.redPort = r;
    _memory.greenPort = g;
    _memory.bluePort = b;
    return _memory.doorbellPort;
}

void
BuiltinTFTDisplay::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    _memory.commandPort = FillRect;
    _memory.xPort = x;
    _memory.yPort = y;
    _memory.colorPort = color;
    _memory.wPort = w;
    _memory.hPort = h;
    _memory.doorbellPort = 1;
}


void
BuiltinTFTDisplay::setCursor(int x, int y) {
    _memory.commandPort = SetCursor;
    _memory.xPort = x;
    _memory.yPort = y;
    _memory.doorbellPort = 1;
}

void
BuiltinTFTDisplay::flush() {
    /// @todo implement
}

void
BuiltinTFTDisplay::print(char c) {
    /// @todo implement
}
void
ChipsetBasicFunctions::writeLine() {
    write('\n');
}
void
ChipsetBasicFunctions::writeLine(const char* ptr) {
    write(ptr);
    writeLine();
}


void
ChipsetBasicFunctions::disableCacheLineActivityLogging() {
    _memory.showCacheLineUpdatesPort = false;
}

void
ChipsetBasicFunctions::enableCacheLineActivityLogging() {
    _memory.showCacheLineUpdatesPort = true;
}

void
ChipsetBasicFunctions::enableMemoryReadWriteLogging() {
    _memory.showReadsAndWritesPort = true;
}

void
ChipsetBasicFunctions::disableMemoryReadWriteLogging() {
    _memory.showReadsAndWritesPort = false;
}


ChipsetBasicFunctions&
getBasicChipsetInterface() {
    static ChipsetBasicFunctions theLed;
    return theLed;
}

BuiltinTFTDisplay& getDisplay() {
    static BuiltinTFTDisplay theDisplay;
    return theDisplay;
}

ssize_t
ChipsetBasicFunctions::write(char *buffer, size_t nbyte) {
    // unlike reading, we must be sequential in writing
    ssize_t numWritten = 0;
    for (size_t i = 0; i < nbyte; ++i) {
        _memory.consoleIOPort = buffer[i];
        ++numWritten;
    }
    flush();
    return numWritten;
}

ssize_t
ChipsetBasicFunctions::read(char *buffer, size_t nbyte) const {
    ssize_t numRead = 0;
    for (size_t i = 0; i < nbyte; ++i) {
        buffer[i] = _memory.consoleIOPort;
        ++numRead;
    }
    return numRead;
}
namespace SDCard {
    enum Operations {
        NoneOperation = 0,
        OpenFile,
        CloseFile,
        FileExists,
        MakeDirectory,
        RemoveDirectory,
        GetNumberOfOpenFiles,
        GetMaximumNumberOfOpenFiles,
        GetFixedPathMaximum,
        // File specific operations
        IsValidFileId = 0x8000,
        FileRead,
        FileWrite,
        FileFlush,
        FileSeek,
        FileIsOpen,
        GetFileName,
        GetFileBytesAvailable,
        GetFilePosition,
        GetFilePermissions,
        GetFileSize,
        GetFileCoordinates,
    };

    enum ErrorCodes {
        None = 0,
        NoCommandProvided,
        UndefinedCommandProvided,
        BadFileId,
        FileIsNotValid,
        /**
         * @brief Attempts to open ram.bin, boot.rom, or boot.data will trigger this fault
         */
        CriticalFileSideChannelAttempt,
        UnimplementedCommand,
        AllFileSlotsInUse,
        AttemptToReadFromUnmappedMemory,
        AttemptToWriteToUnmappedMemory,
        UnableToSeekToRequestedDestination,
    };
} // end namespace SDCard

SDCardInterface::SDCardInterface() : BuiltinIOBaseDevice(0x300), _memory(memory<RawSDCardInterface>(getIOBase0Address(0x300))) {
    // clear out any operations
    _memory.command = SDCard::GetMaximumNumberOfOpenFiles;
    _memory.doorbell = 0;
    maxFileCount_ = _memory.result.words[0];
    _memory.command = SDCard::NoneOperation;
}


SDCardInterface& getSDCardInterface() {
    static SDCardInterface theSDCard;
    return theSDCard;
}

bool
SDCardInterface::fileExists(const char *path) {
    size_t length = strlen(path);
    if ((length <= 80) && (length > 0)) {
        for (int i = 0; i < length; ++i) {
            _memory.path[i] = path[i];
        }
        if (length < 80) {
            _memory.path[length] = 0;
        }
        _memory.command = SDCard::FileExists;
        _memory.doorbell = 1;
        return _memory.result.bytes[0] != 0;
    } else {
        return false;
    }
}

void
BuiltinTFTDisplay::print(const char *c) {
    size_t length = strlen(c);
    for (size_t i = 0; i < length; ++i) {
        print(c[i]);
    }
}

void
BuiltinTFTDisplay::println(const char *c) {
    print(c);
    print('\n');
}

void
BuiltinTFTDisplay::print(const std::string &line) {
    for (std::string::const_iterator it = line.begin(); it != line.end(); ++it) {
        print(*it);
    }
}
void
BuiltinTFTDisplay::println(const std::string &line) {
    print(line);
    print('\n');
}
void
BuiltinTFTDisplay::setRotation(int16_t value) {
    _memory.commandPort = SetRotation;
    _memory.xPort = value;
    _memory.doorbellPort = 0;
}
void
BuiltinTFTDisplay::invertDisplay(bool value) {
    _memory.commandPort = InvertDisplay;
    _memory.xPort = value ? 1 : 0;
    _memory.doorbellPort = 0;
}
void
BuiltinTFTDisplay::setTextSize(int16_t x) {
    _memory.commandPort = SetTextSizeSquare;
    _memory.xPort = x;
    _memory.doorbellPort = 0;
}
void
BuiltinTFTDisplay::setTextSize(int16_t x, int16_t y) {
    _memory.commandPort = SetTextSizeRectangle;
    _memory.xPort = x;
    _memory.yPort = y;
    _memory.doorbellPort = 0;
}
uint16_t
BuiltinTFTDisplay::getCursorX() const {
    _memory.commandPort = GetCursorX;
    return _memory.doorbellPort;
}
uint16_t
BuiltinTFTDisplay::getCursorY() const {
   _memory.commandPort = GetCursorY;
   return _memory.doorbellPort;
}
uint16_t
BuiltinTFTDisplay::getRotation() const {
    _memory.commandPort = GetRotation;
    return _memory.doorbellPort;
}
int
SDCardInterface::openFile(const std::string& path, int flags) {
    if (path.length() > 80) {
        errno = ENAMETOOLONG;
        return -1;
    }
    _memory.command = SDCard::OpenFile;
    volatile char* ptr = _memory.path;
    for (std::string::const_iterator it = path.begin(); it != path.end(); ++it, ++ptr) {
       *ptr = *it;
    }
    if (path.length() < 80) {
        _memory.path[path.length()] = 0;
    }

    _memory.permissionBits = flags;
    // cannot open write only at this point
    _memory.openReadWrite = ((flags & O_RDWR) || (flags & O_WRONLY));
    uint16_t outcome = _memory.doorbell;
    if (outcome == 0xFFFF) {
        uint16_t errorCode = _memory.errorCode;
        switch (errorCode) {
            case SDCard::AllFileSlotsInUse:
                errno = ENFILE;
                break;
            case SDCard::FileIsNotValid:
                errno = EBADF;
                break;
            default:
                errno = EBADF;
                break;
        }
        return -1;
    } else {
        return static_cast<int>(_memory.result.words[0]);
    }
}

int
SDCardInterface::writeFile(int fileId, const void *buf, size_t count) {
    _memory.command = SDCard::FileWrite;
    _memory.fileId = fileId;
    _memory.address = reinterpret_cast<uint32_t>(buf);
    _memory.count = count;
    uint16_t result = _memory.doorbell;
    if (result != 0) {
        switch (static_cast<SDCard::ErrorCodes>(result)) {
            case SDCard::AttemptToWriteToUnmappedMemory:
                errno = EIO;
                break;
            case SDCard::FileIsNotValid:
            case SDCard::BadFileId:
            default:
                errno = EBADF;
                break;
        }
        return -1;
    }
    return static_cast<int>(_memory.result.words[0]);
}
namespace {
    bool checkSDCardReadError(uint16_t value) {
        if (value != 0) {
            switch (static_cast<SDCard::ErrorCodes>(value)) {
                case SDCard::AttemptToReadFromUnmappedMemory:
                    errno = EIO;
                    break;
                case SDCard::FileIsNotValid:
                case SDCard::BadFileId:
                default:
                    errno = EBADF;
                    break;
            }
            return false;
        }
        return true;
    }
}
int
SDCardInterface::readFile(int fileId, void *buf, size_t count) {
    _memory.command = SDCard::FileRead;
    _memory.fileId = fileId;
    _memory.address = reinterpret_cast<uint32_t>(buf);
    uint32_t times = count / 16;
    uint32_t slop = count % 16;
    uint32_t numRead = 0;
    _memory.count = 16;
    for (uint32_t i = 0; i < times; ++i) {
        if (!checkSDCardReadError(_memory.doorbell)) {
            return -1;
        } else {
            numRead += _memory.result.words[0];
            _memory.address += 16;
        }
    }
    if (slop > 0) {
        _memory.count = slop;
        if (!checkSDCardReadError(_memory.doorbell)) {
            return -1;
        } else {
            numRead += _memory.result.words[0];
        }
    }
    return static_cast<int>(numRead);
}

int
SDCardInterface::closeFile(int fileId) {
    _memory.command = SDCard::CloseFile;
    _memory.fileId = fileId;
    uint16_t executionResult = _memory.doorbell;
    if (executionResult == 0xFFFF) {
        errno = EBADF;
        return -1;
    } else {
        return 0;
    }

}

off_t
SDCardInterface::seek(int fileId, off_t offset, int whence) {
    _memory.command = SDCard::FileSeek;
    _memory.fileId = fileId;
    _memory.seekPosition = offset;
    switch (whence) {
        case SEEK_END:
            _memory.whence = 2;
            break;
        case SEEK_CUR:
            _memory.whence = 1;
            break;
        case SEEK_SET:
        default:
            _memory.whence = 0;
            break;
    }
    uint16_t result = _memory.doorbell;
    if (result == 0xFFFF) {
        switch (static_cast<SDCard::ErrorCodes>(result)) {
            case SDCard::UnableToSeekToRequestedDestination:
                errno = EOVERFLOW;
                break;
            default:
                errno = EBADF;
                break;
        }
        return -1;
    } else {
        return _memory.result.swords[0];
    }
}