#include "base64.h"


static const char base64_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";

static const uint8_t base64_lookup[256] = {
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x62, 0x64, 0x64, 0x64, 0x63,
0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x60, 0x61, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F, 0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64,
0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64, 0x64
};

size_t encode_length(const size_t size) {
    return (((size + 2) / 3) * 4) + 1;
}

size_t encode(const char *buf, const size_t size, char *base64) {
    if (size == 0) {
        base64[0] = '\0';
        return 0;
    }

    size_t out_len = encode_length(size);
    if (out_len > SIZE_MAX - 1) {
        // 输出缓冲区太大
        return 0;
    }

    uint32_t value = 0;
    size_t i = 0, j = 0;
    while (i < size) {
        value = (value << 8) | (uint8_t)buf[i++];
        for (int k = 0; k < 3 && i < size; k++) {
            value = (value << 8) | (uint8_t)buf[i++];
        }

        base64[j++] = base64_chars[(value >> 18) & 0x3F];
        base64[j++] = base64_chars[(value >> 12) & 0x3F];
        base64[j++] = base64_chars[(value >> 6) & 0x3F];
        base64[j++] = base64_chars[value & 0x3F];
    }

    size_t rem = size % 3;
    if (rem == 1) {
        base64[out_len - 2] = '=';
        base64[out_len - 1] = '=';
    } else if (rem == 2) {
        base64[out_len - 1] = '=';
    }

    base64[out_len - 1] = '\0';
    return out_len - 1;
}



//base64全部字符
static const char *ALPHA_BASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


////base64编码
//size_t encode(const char *buf, const long size, char *base64Char) {
//    int a = 0;
//    int i = 0;
//    while (i < size) {
//        char b0 = buf[i++];
//        char b1 = (i < size) ? buf[i++] : 0;
//        char b2 = (i < size) ? buf[i++] : 0;
//         
//        int int63 = 0x3F; //  00111111
//        int int255 = 0xFF; // 11111111
//        base64Char[a++] = ALPHA_BASE[(b0 >> 2) & int63];
//        base64Char[a++] = ALPHA_BASE[((b0 << 4) | ((b1 & int255) >> 4)) & int63];
//        base64Char[a++] = ALPHA_BASE[((b1 << 2) | ((b2 & int255) >> 6)) & int63];
//        base64Char[a++] = ALPHA_BASE[b2 & int63];
//    }
//    switch (size % 3) {
//        case 1:
//            base64Char[--a] = '=';
//        case 2:
//            base64Char[--a] = '=';
//    }
//    return a;
//}
 
//base64解码
char *decode(const char *base64Char, const long base64CharSize, char *originChar, long originCharSize) {
    int toInt[128] = {-1};
    for (int i = 0; i < 64; i++) {
        toInt[ALPHA_BASE[i]] = i;
    }
    int int255 = 0xFF;
    int index = 0;
    for (int i = 0; i < base64CharSize; i += 4) {
        int c0 = toInt[base64Char[i]];
        int c1 = toInt[base64Char[i + 1]];
        originChar[index++] = (((c0 << 2) | (c1 >> 4)) & int255);
        if (index >= originCharSize) {
            return originChar;
        }
        int c2 = toInt[base64Char[i + 2]];
        originChar[index++] = (((c1 << 4) | (c2 >> 2)) & int255);
        if (index >= originCharSize) {
            return originChar;
        }
        int c3 = toInt[base64Char[i + 3]];
        originChar[index++] = (((c2 << 6) | c3) & int255);
    }
    return originChar;
}
 


 //十六进制转十进制
int hex2dec(char c)
{
    if ('0' <= c && c <= '9')
    {
        return c - '0';
    }
    else if ('a' <= c && c <= 'f')
    {
        return c - 'a' + 10;
    }
    else if ('A' <= c && c <= 'F')
    {
        return c - 'A' + 10;
    }
    else
    {
        return -1;
    }
}
 
//十进制转十六进制
char dec2hex(short int c)
{
    if (0 <= c && c <= 9)
    {
        return c + '0';
    }
    else if (10 <= c && c <= 15)
    {
        return c + 'A' - 10;
    }
    else
    {
        return (char)-1;
    }
}
 
 
//编码一个url 
void urlencode(char url[])
{
    int i = 0;
    int len = strlen(url);
    int res_len = 0;
    char res[BURSIZE];
    for (i = 0; i < len; ++i)
    {
        char c = url[i];
        if (    ('0' <= c && c <= '9') ||
                ('a' <= c && c <= 'z') ||
                ('A' <= c && c <= 'Z') ||
                c == '/' || c == '.')
        {
            res[res_len++] = c;
        }
        else
        {
            int j = (short int)c;
            if (j < 0)
                j += 256;
            int i1, i0;
            i1 = j / 16;
            i0 = j - i1 * 16;
            res[res_len++] = '%';
            res[res_len++] = dec2hex(i1);
            res[res_len++] = dec2hex(i0);
        }
    }
    res[res_len] = '\0';
    strcpy(url, res);
}
 
// 解码url
uint16_t urldecode(char url[], uint16_t size, char* text)
{
    int i = 0;
    int len = strlen(url);
    int res_len = 0;
//    char res[BURSIZE];
    for (i = 0; i < len; ++i)
    {
        char c = url[i];
        if (c != '%')
        {
            text[res_len++] = c;
        }
        else
        {
            char c1 = url[++i];
            char c0 = url[++i];
            int num = 0;
            num = hex2dec(c1) * 16 + hex2dec(c0);
            text[res_len++] = num;
        }
    }
    text[res_len] = '\0';
		size = res_len;
		return size;
}  



//判断是否为base64编码
bool is_base64(const char *data) {
    int i;
    for (i = 0; data[i]; ++i) {
        if (!((data[i] >= 'A' && data[i] <= 'Z') ||
              (data[i] >= 'a' && data[i] <= 'z') ||
              (data[i] >= '0' && data[i] <= '9') ||
              data[i] == '+' || data[i] == '/' || data[i] == '=')) {
            return false;
        }
    }
    return true;
}

//判断是否为URL编码
bool is_url_encoded(const char *data) {
    int i;
    for (i = 0; data[i]; ++i) {
        if (data[i] == '%' && isxdigit(data[i+1]) && isxdigit(data[i+2])) {
            return true;
        }
    }
    return false;
}



