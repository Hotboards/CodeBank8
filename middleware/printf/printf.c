/*-------------------------------------------------------------------------------------------------/
/  Universal string handler for user console interface
/--------------------------------------------------------------------------------------------------/
/
/  Copyright (C) 2011, ChaN, all right reserved.
/
/ * This software is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial products UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/-------------------------------------------------------------------------------------------------*/
/*-- Includes --*/
#include "printf.h"
#include <stdarg.h>
#include "hardware_profile.h"


/*-- Private Definitions--*/
#define DW_CHAR                 sizeof(char)
#define DW_SHORT                sizeof(short)
#define DW_LONG                 sizeof(long)


/*-- Global variables --*/
static void (*xfunc_out)(const unsigned char);	/* Pointer to the output stream */
static char *outptr = (void*)0;


/*-- Private Macros --*/


/*-- Private functions prototypes --*/
static void xvprintf(const char *fmt, va_list arp);


/*-- External functions --*/
/**-----------------------------------------------------------------------------------------------*/
void xdev_out( void(*func)(const unsigned char) )
{
    xfunc_out = func;
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void xputc( char c )
{
    if (outptr)
    {
        *outptr++ = (unsigned char)c;
        return;
    }
    if (xfunc_out)
    {
        xfunc_out((unsigned char)c);
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void xputs( const char* str )
{
    while(*str != 0)
    {
        xputc(*str);
        str++;
    }
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void xfputs(void(*func)(const unsigned char), const char *str)
{
    void (*pf)(const unsigned char);

    pf = xfunc_out;		/* Save current output device */
    xfunc_out = func;	/* Switch output to specified device */
    while (*str != 0)		/* Put the string */
    {
        xputc(*str);
        str++;
    }
    xfunc_out = pf;		/* Restore output device */
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void xprintf(const char *fmt, ...)
{
    va_list arp;

    va_start(arp, fmt);
    xvprintf(fmt, arp);
    va_end(arp);
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void xsprintf(char* buff, const char *fmt, ... )
{
    va_list arp;

    outptr = buff;		/* Switch destination for memory */

    va_start(arp, fmt);
    xvprintf(fmt, arp);
    va_end(arp);

    *outptr = 0;		/* Terminate output string with a \0 */
    outptr = 0;			/* Switch destination for device */
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void xfprintf(void(*func)(const unsigned char), const char *fmt, ... )							/* Optional arguments */
{
    va_list arp;
    void (*pf)(const unsigned char);

    pf = xfunc_out;		/* Save current output device */
    xfunc_out = func;	/* Switch output to specified device */

    va_start(arp, fmt);
    xvprintf(fmt, arp);
    va_end(arp);

    xfunc_out = pf;		/* Restore output device */
}
/**-----------------------------------------------------------------------------------------------*/

/**-----------------------------------------------------------------------------------------------*/
void put_dump(const void* buff, unsigned long addr, int len, int width)
{
    int i;
    const unsigned char *bp;
    const unsigned short *sp;
    const unsigned long *lp;

    xprintf("%08lX ", addr);		/* address */

    switch (width)
    {
        case DW_CHAR:
            bp = buff;
            for (i = 0; i < len; i++)		/* Hexdecimal dump */
                xprintf(" %02X", bp[i]);
            xputc(' ');
            for (i = 0; i < len; i++)		/* ASCII dump */
                xputc((bp[i] >= ' ' && bp[i] <= '~') ? bp[i] : '.');
        break;
        case DW_SHORT:
            sp = buff;
            do								/* Hexdecimal dump */
                xprintf(" %04X", *sp++);
            while (--len);
        break;
        case DW_LONG:
            lp = buff;
            do								/* Hexdecimal dump */
                xprintf(" %08LX", *lp++);
            while (--len);
        break;
    }

    xputc('\n');
}
/**-----------------------------------------------------------------------------------------------*/


/*-- Private functions --*/
/**-------------------------------------------------------------------------------------------------    
  \brief        None
  \param        None
  \return       None
  \warning      None       
--------------------------------------------------------------------------------------------------*/
static void xvprintf(const char* fmt, va_list arp)
{
    unsigned int r, i, j, w, f;
    unsigned long v;
    char s[16], c, d, *p;

    for (;;)
    {
        c = *fmt++;					/* Get a char */
        if (!c) break;				/* End of format? */
        if (c != '%') {				/* Pass through it if not a % sequense */
            xputc(c);
            continue;
        }
        f = 0;
        c = *fmt++;					/* Get first char of the sequense */
        if (c == '0') {				/* Flag: '0' padded */
            f = 1;
            c = *fmt++;
        }
        else {
            if (c == '-') {			/* Flag: left justified */
                f = 2;
                c = *fmt++;
            }
        }
        for (w = 0; c >= '0' && c <= '9'; c = *fmt++)	/* Minimum width */
            w = w * 10 + c - '0';
        if (c == 'l' || c == 'L') {	/* Prefix: Size is long int */
            f |= 4;
            c = *fmt++;
        }
        if (!c) break;				/* End of format? */
        d = c;
        if (d >= 'a')
            d -= 0x20;
        switch (d) {				/* Type is... */
            case 'S' :					/* String */
                p = va_arg(arp, char*);
                for (j = 0; p[j]; j++);
                while (!(f & 2) && j++ < w)
                    xputc(' ');
                xputs((const char*)p);
                while (j++ < w)
                    xputc(' ');
                continue;
            case 'C' :					/* Character */
                xputc((char)va_arg(arp, int)); continue;
            case 'B' :					/* Binary */
                r = 2; break;
            case 'O' :					/* Octal */
                r = 8; break;
            case 'D' :					/* Signed decimal */
            case 'U' :					/* Unsigned decimal */
                r = 10; break;
            case 'X' :					/* Hexdecimal */
                r = 16; break;
            default:					/* Unknown type (passthrough) */
                xputc(c); continue;
        }
        /* Get an argument and put it in numeral */
        v = (f & 4) ? va_arg(arp, long) : ((d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int));
        if (d == 'D' && (v & 0x80000000))
        {
            v = 0 - v;
            f |= 8;
        }
        i = 0;
        do
        {
            d = (char)(v % r); v /= r;
            if (d > 9)
                d += (c == 'x') ? 0x27 : 0x07;
            s[i++] = d + '0';
        } while (v && i < sizeof(s));

        if (f & 8)
            s[i++] = '-';
        j = i;
        d = (f & 1) ? '0' : ' ';
        while (!(f & 2) && j++ < w)
            xputc(d);
        do 
            xputc(s[--i]);
        while(i);
        while (j++ < w)
            xputc(' ');
    }
}
