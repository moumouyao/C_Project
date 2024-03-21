#ifndef _Base64_H
#define _Base64_H

#include "stm32f4xx.h"
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define BURSIZE 2048

size_t encode_length(const size_t size);
  size_t encode(const char *buf, const size_t size, char *base64);
//size_t encode(const char *buf, const long size, char *base64Char);
char *decode(const char *base64Char, const long base64CharSize, char *originChar, long originCharSize);
int hex2dec(char c);
char dec2hex(short int c);
void urlencode(char url[]);
uint16_t urldecode(char url[], uint16_t size, char* text);

bool is_base64(const char *data);
bool is_url_encoded(const char *data);

 
#endif
