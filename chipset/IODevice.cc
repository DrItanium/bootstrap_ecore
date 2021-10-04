//
// Created by jwscoggins on 5/3/21.
//

#include "IODevice.h"
#include "ChipsetInteract.h"
#include <string.h>
#include <errno.h>
#include <fcntl.h>


ChipsetBasicFunctions::SDFile::SDFile(uint32_t baseAddress) : raw(memory<FileInterfaceRaw>(baseAddress)) {}
BuiltinIOBaseDevice::BuiltinIOBaseDevice(uint32_t offset) : offset_(offset), baseAddress_(getIOBase0Address(offset)) { }
ChipsetBasicFunctions::ChipsetBasicFunctions(uint32_t offset) : BuiltinIOBaseDevice(offset),
_memory(memory<ChipsetRegistersRaw>(baseAddress_)),
_sdbase(memory<SDCardBaseInterfaceRaw>(baseAddress_ + 0x100)),
openFiles(new SDFile*[_sdbase.maximumNumberOfOpenFilesPort]) {
    for (int i = 0, offset = 2; i < _sdbase.maximumNumberOfOpenFilesPort; ++i, ++offset) {
        openFiles[i] = new SDFile(baseAddress_ + (0x100 * offset));
    }
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

void
ChipsetBasicFunctions::writeLine() {
    write('\n');
}
void
ChipsetBasicFunctions::writeLine(const char* ptr) {
    write(ptr);
    writeLine();
}


ChipsetBasicFunctions&
getBasicChipsetInterface() {
    static ChipsetBasicFunctions theLed;
    return theLed;
}

ssize_t
ChipsetBasicFunctions::write(char *buffer, size_t nbyte) {
    // unlike reading, we must be sequential in writing
    ssize_t numWritten = 0;
    for (size_t i = 0; i < nbyte; ++i) {
        _memory.consoleIOPort = static_cast<uint16_t>(static_cast<unsigned char>(buffer[i]));
        ++numWritten;
    }
    flush();
    return numWritten;
}
uint16_t
ChipsetBasicFunctions::waitForLegalCharacter() {
    uint16_t rawConsoleValue = _memory.consoleIOPort;
    while (rawConsoleValue == 0xFFFF) {
        rawConsoleValue = _memory.consoleIOPort;
    }
    return rawConsoleValue;
}
ssize_t
ChipsetBasicFunctions::read(char *buffer, size_t nbyte) {
    ssize_t numRead = 0;
    for (size_t i = 0; i < nbyte; ++i) {
        buffer[i] = static_cast<char>(waitForLegalCharacter());
        ++numRead;
        if ((buffer[i] == '\n') || (buffer[i] == '\r')) {
            return numRead;
        }
    }
    return numRead;
}




void
ChipsetBasicFunctions::triggerInt0() {
    // force the chipset to trigger an interrupt
    _memory.triggerInt0Port = 1;
}
bool
ChipsetBasicFunctions::addressDebuggingEnabled() const {
    return _memory.addressDebuggingFlag != 0;
}
void
ChipsetBasicFunctions::enableAddressDebugging() {
    // upper 16-bits are ignored on the chipset side but who cares
    _memory.addressDebuggingFlag = 0xFFFFFFFF;
}
void
ChipsetBasicFunctions::disableAddressDebugging() {
    _memory.addressDebuggingFlag = 0;
}

ChipsetBasicFunctions::~ChipsetBasicFunctions() {
    for (int i = 0; i < _sdbase.maximumNumberOfOpenFilesPort; ++i) {
        if (openFiles[i]->isOpen()) {
            openFiles[i]->close();
        }
        delete openFiles[i];
    }
    delete [] openFiles;
}

int
ChipsetBasicFunctions::openFile(const char *path, int flags, int mode) {
    int len = strlen(path);
    if (len > 80) {
        errno = EBADF;
        return -1;
    } else {
        // transfer the path
        for (int i = 0; i < len; ++i) {
            _sdbase.path[i] = path[i];
        }
        _sdbase.permissionsPort = flags | mode; // unsure if this is safe or not...
        // make the chipset look for an open file handle that we can use
        return _sdbase.openPort;
    }
}
bool
ChipsetBasicFunctions::readFile(int fd, void *buf, size_t sz, int &nread) {
    SDFile* file = openFiles[fd];
    if (!file->isOpen()) {
        return false;
    } else {
        /// @todo implement
        char* buffer = reinterpret_cast<char*>(buf) ;
        for (size_t i = 0; i < sz; ++i) {
            int result = file->getChar();
            if (result == -1) {
                // EOF reached
                break;
            } else {
                ++nread;
                buffer[i] = static_cast<char>(result);
            }
        }
        return true;
    }
}

bool
ChipsetBasicFunctions::writeFile(int fd, const void *buf, size_t sz, int &nwrite) {
    SDFile* file = openFiles[fd];
    if (!file->isOpen()) {
        return false;
    } else {
        /// @todo implement
        const char* buffer = reinterpret_cast<const char*>(buf) ;
        for (size_t i = 0; i < sz; ++i) {
            file->putChar(buffer[i]);
            if (file->getWriteError()) {
                break;
            } else {
                ++nwrite;
            }
        }
        return true;
    }
}
