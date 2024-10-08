#include "string_view_utf8.hpp"

string_view_utf8::Length string_view_utf8::computeNumUtf8Chars() const {
    if (utf8Length >= 0) {
        return utf8Length;
    }

    Length r = 0;
    StringReaderUtf8 reader(*this);
    while (reader.getUtf8Char()) {
        ++r;
    }

    utf8Length = r;
    return r;
}

unichar string_view_utf8::getFirstUtf8Char() const {
    StringReaderUtf8 reader(*this);
    return reader.getUtf8Char();
}

string_view_utf8 string_view_utf8::substr(size_t pos) const {
    StringReaderUtf8 reader(*this);
    while (pos--) {
        reader.getUtf8Char();
    }

    return reader.remaining_string();
}

size_t string_view_utf8::copyToRAM(char *dst, size_t max_size) const {
    if (max_size == 0) {
        return 0;
    }
    char *dst_start = dst;
    StringReaderUtf8 reader(*this);
    for (size_t i = 0; i < max_size; ++i) {
        *dst = reader.getbyte();
        if (*dst == 0) {
            return dst - dst_start;
        }
        ++dst;
    }

    --dst; // dst pointer is decremented to point at the last character of the buffer
    // terminate string - cut the string in between characters (no leftover prefixes)
    while (dst != dst_start && UTF8_IS_CONT(*dst)) {
        dst--;
    }

    *dst = 0;
    return dst - dst_start;
}

size_t string_view_utf8::copyBytesToRAM(char *dst, size_t buffer_size) const {
    if (buffer_size == 0) {
        return 0;
    }
    StringReaderUtf8 reader(*this);
    char *dst_start = dst;
    for (size_t i = 0; i < buffer_size; ++i) {
        *dst = reader.getbyte();
        if (*dst == 0) {
            return dst - dst_start;
        }
        ++dst;
    }

    // Beware - no multibyte character check!
    // dst pointer is decremented to point at the last character of the buffer
    *(--dst) = 0; // safety termination in case of reaching the end of the buffer
    return dst - dst_start;
}

unichar StringReaderUtf8::getUtf8Char() {
    uint8_t byte = getbyte();

    if (!UTF8_IS_NONASCII(byte)) {
        return byte;
    }

    unichar ord = byte;
    ord &= 0x7F;
    for (unichar mask = 0x40; ord & mask; mask >>= 1) {
        ord &= ~mask;
    }

    while (UTF8_IS_CONT(peek())) {
        byte = getbyte();
        ord = (ord << 6) | (byte & 0x3F);
    }
    return ord;
}

uint8_t StringReaderUtf8::getbyte() {
    switch (view_.type) {

    case EType::CPUFLASH:
    case EType::RAM:
        return *view_.cpuflash.utf8raw++; // beware - expecting, that the input string is null-terminated! No other checks are done

    case EType::FILE:
        return FILE_getbyte();

    default:
        return 0;
    }
}

uint8_t StringReaderUtf8::peek() const {
    switch (view_.type) {

    case EType::CPUFLASH:
    case EType::RAM:
        return *view_.cpuflash.utf8raw; // beware - expecting, that the input string is null-terminated! No other checks are done

    case EType::FILE:
        return FILE_peek();

    default:
        return 0;
    }
}

uint8_t StringReaderUtf8::FILE_getbyte() {
    if (!view_.file.f) {
        return '\0';
    }
    uint8_t c;
    // sync among multiple reads from the sameMO file
    if (ftell(view_.file.f) != static_cast<long>(view_.file.offset)) {
        if (fseek(view_.file.f, view_.file.offset, SEEK_SET) != 0) {
            return '\0';
        }
    }

    view_.file.offset++;
    if (fread(&c, 1, 1, view_.file.f) != 1) {
        return '\0';
    }
    return c;
}

uint8_t StringReaderUtf8::FILE_peek() const {
    if (!view_.file.f) {
        return '\0';
    }
    uint8_t c;
    // sync among multiple reads from the sameMO file
    if (ftell(view_.file.f) != static_cast<long>(view_.file.offset)) {
        if (fseek(view_.file.f, view_.file.offset, SEEK_SET) != 0) {
            return '\0';
        }
    }

    if (fread(&c, 1, 1, view_.file.f) != 1) {
        return '\0';
    }
    return c;
}
