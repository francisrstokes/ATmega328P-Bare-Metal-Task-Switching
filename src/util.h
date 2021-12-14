#ifndef UTIL_H
#define UTIL_H

#define U16_TO_U8_L(u16) (uint8_t)(u16 & 0xff)
#define U16_TO_U8_H(u16) (uint8_t)((u16 >> 8) & 0xff)

#endif