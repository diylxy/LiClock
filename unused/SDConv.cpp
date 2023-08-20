#include <A_Config.h>
#include <SDConv.h>
#include <ff.h>
SDConv sdconv;

static const WCHAR uc850[] = {/*  CP850(Latin 1) to Unicode conversion table */
                              0x00C7, 0x00FC, 0x00E9, 0x00E2, 0x00E4, 0x00E0, 0x00E5, 0x00E7, 0x00EA, 0x00EB, 0x00E8, 0x00EF, 0x00EE, 0x00EC, 0x00C4, 0x00C5,
                              0x00C9, 0x00E6, 0x00C6, 0x00F4, 0x00F6, 0x00F2, 0x00FB, 0x00F9, 0x00FF, 0x00D6, 0x00DC, 0x00F8, 0x00A3, 0x00D8, 0x00D7, 0x0192,
                              0x00E1, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00D1, 0x00AA, 0x00BA, 0x00BF, 0x00AE, 0x00AC, 0x00BD, 0x00BC, 0x00A1, 0x00AB, 0x00BB,
                              0x2591, 0x2592, 0x2593, 0x2502, 0x2524, 0x00C1, 0x00C2, 0x00C0, 0x00A9, 0x2563, 0x2551, 0x2557, 0x255D, 0x00A2, 0x00A5, 0x2510,
                              0x2514, 0x2534, 0x252C, 0x251C, 0x2500, 0x253C, 0x00E3, 0x00C3, 0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x00A4,
                              0x00F0, 0x00D0, 0x00CA, 0x00CB, 0x00C8, 0x0131, 0x00CD, 0x00CE, 0x00CF, 0x2518, 0x250C, 0x2588, 0x2584, 0x00A6, 0x00CC, 0x2580,
                              0x00D3, 0x00DF, 0x00D4, 0x00D2, 0x00F5, 0x00D5, 0x00B5, 0x00FE, 0x00DE, 0x00DA, 0x00DB, 0x00D9, 0x00FD, 0x00DD, 0x00AF, 0x00B4,
                              0x00AD, 0x00B1, 0x2017, 0x00BE, 0x00B6, 0x00A7, 0x00F7, 0x00B8, 0x00B0, 0x00A8, 0x00B7, 0x00B9, 0x00B3, 0x00B2, 0x25A0, 0x00A0};
extern "C"
{
    WCHAR ff_uni2oem_1(           /* Returns OEM code character, zero on error */
                       DWORD uni, /* UTF-16 encoded character to be converted */
                       WORD cp    /* Code page for the conversion */
    );
    WCHAR ff_oem2uni_1(           /* Returns Unicode character in UTF-16, zero on error */
                       WCHAR oem, /* OEM code to be converted */
                       WORD cp    /* Code page for the conversion */
    );
}
#define MYPUTCHAR(x)       \
    {                      \
        _buffer[_end] = x; \
        ++_end;            \
    }
WCHAR utf8to16(const char *str)
{
    int x, z, y, a1, a2, a, b, c, d, flag; // z=Unicode,x=variable for input,a-b-c-d=temporary variables for byte management
    x = *str++;
    if (x <= 0x7F)
    { // 1-byte-encoded: 0xxxxxxx
        z = x;
    }
    else if (x >= 0xC0 && x <= 0xDF)
    { // 2-byte-encoded: 110xxxxx 10xxxxxx	,	where x are unicode bits
        a = x & 31;
        a <<= 6;

        x = *str++;
        b = x & 63;

        z = a | b;
    }
    else if (x >= 0xE0 && x <= 0xEF)
    { // 3-byte-encoded: 1110xxxx 10xxxxxx 10xxxxxx
        a = x & 15;
        a <<= 12;

        x = *str++;
        b = x & 63;
        b <<= 6;

        x = *str++;
        c = x & 63;

        z = a | b | c;
    }
    else if (x >= 0xF0 && x <= 0xF7)
    { // 4-byte-encoded: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        a = x & 7;
        a <<= 18;

        x = *str++;
        b = x & 0x3F;
        b <<= 12;

        x = *str++;
        c = x & 63;
        c <<= 6;

        x = *str++;
        d = x & 63;

        z = a | b | c | d;
    }
    return z;
}
const char *SDConv::OEM2UTF8(const char *str)
{
    _end = 0;
    int ptr = 0;
    int x, y, j, z, flag;
    unsigned int h;
    // z=Unicode,h=pair to 2-byte encoding or 1st pair to 4byte-encoding,j=2nd pair to a 4byte-encoding,
    while (*str)
    {
        WCHAR oem;
        // 首先：utf8-utf16le
        {
            int x, z, y, a1, a2, a, b, c, d, flag; // z=Unicode,x=variable for input,a-b-c-d=temporary variables for byte management
            x = *str++;
            if (x <= 0x7F)
            { // 1-byte-encoded: 0xxxxxxx
                z = x;
            }
            else if (x >= 0xC0 && x <= 0xDF)
            { // 2-byte-encoded: 110xxxxx 10xxxxxx	,	where x are unicode bits
                a = x & 31;
                a <<= 6;

                x = *str++;
                b = x & 63;

                z = a | b;
            }
            else if (x >= 0xE0 && x <= 0xEF)
            { // 3-byte-encoded: 1110xxxx 10xxxxxx 10xxxxxx
                a = x & 15;
                a <<= 12;

                x = *str++;
                b = x & 63;
                b <<= 6;

                x = *str++;
                c = x & 63;

                z = a | b | c;
            }
            else if (x >= 0xF0 && x <= 0xF7)
            { // 4-byte-encoded: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
                a = x & 7;
                a <<= 18;

                x = *str++;
                b = x & 0x3F;
                b <<= 12;

                x = *str++;
                c = x & 63;
                c <<= 6;

                x = *str++;
                d = x & 63;

                z = a | b | c | d;
            }
            oem = z;
        }
        // 之后：utf16le-oem850
        WCHAR c = 0;
        const WCHAR *p = uc850;
        if (oem < 0x80)
        { /* ASCII? */
            c = (WCHAR)oem;
        }
        else
        { /* Non-ASCII */
            if (oem < 0x10000)
            { /* Is it in BMP and valid code page? */
                for (c = 0; c < 0x80 && oem != p[c]; c++)
                    ;
                c = (c + 0x80) & 0xFF;
            }
        }
        h = ff_oem2uni_1(c, 936);
        if ((h <= 0xD7FF) || (h >= 0xE000 && h <= 0xFFFF))
        { // 2-byte encoded,h exists in [0x0000,0xD7FF] or [0xE000,0xFFFF].
            z = h;
        }
        if (z < 0x80)
        { // Encoding using 1 byte , 0xxxxxxx
            MYPUTCHAR(z);
        }
        else if (z < 0x800)
        { // Encoding using 2 bytes , 110xxxxx 10xxxxxx
            MYPUTCHAR(192 | z >> 6);
            MYPUTCHAR(128 | z & 63);
        }
        else if (z <= 0xFFFF)
        { // Encoding using 3 bytes , 1110xxxx 10xxxxxx 10xxxxxx
            if (z < 0xD800 || z > 0xDFFF)
            {
                MYPUTCHAR(224 | z >> 12);
                MYPUTCHAR(128 | z >> 6 & 63);
                MYPUTCHAR(128 | z & 63);
            }
        }
        else if (z <= 0x10FFFF)
        { // Encoding using 4 bytes , 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
            MYPUTCHAR(240 | z >> 18);
            MYPUTCHAR(128 | z >> 12 & 63);
            MYPUTCHAR(128 | z >> 6 & 63);
            MYPUTCHAR(128 | z & 63);
        }
    }
    _buffer[_end] = 0;
    return _buffer;
}
