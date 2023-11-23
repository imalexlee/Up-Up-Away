#include "mode4.h"
#include "font.h"

// Sets a pixel in mode 4
void setPixel4(int x, int y, unsigned char colorIndex)
{
    u16 pixelData = videoBuffer[OFFSET(x, y, SCREENWIDTH) / 2];
    pixelData = (x & 1) ? ((pixelData & 0x00FF) | (colorIndex << 8)) : ((pixelData & 0xFF00) | colorIndex);
    videoBuffer[OFFSET(x, y, SCREENWIDTH) / 2] = pixelData;
}

// Draws a rectangle in mode 4
void drawRect4(int x, int y, int width, int height, volatile unsigned char colorIndex)
{
    volatile u16 color = colorIndex | colorIndex << 8;
    for (int i = 0; i < height; i++)
    {
        if (!(x % 2) && !(width % 2))
        {
            if (width > 0)
            {
                DMANow(3, &color, &videoBuffer[OFFSET(x, y + i, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | width / 2);
            }
        }
        else if ((x % 2) && (width % 2))
        {
            setPixel4(x, y + i, colorIndex);
            if (width > 1)
            {
                DMANow(3, &color, &videoBuffer[OFFSET(x + 1, y + i, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (width - 1) / 2);
            }
        }
        else if (!(x % 2) && (width % 2))
        {
            if (width > 1)
            {
                DMANow(3, &color, &videoBuffer[OFFSET(x, y + i, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (width - 1) / 2);
            }
            setPixel4(x + width - 1, y + i, colorIndex);
        }
        else
        {
            setPixel4(x, y + i, colorIndex);
            if (width > 2)
            {
                DMANow(3, &color, &videoBuffer[OFFSET(x + 1, y + i, SCREENWIDTH) / 2], DMA_SOURCE_FIXED | (width - 2) / 2);
            }
            setPixel4(x + width - 1, y + i, colorIndex);
        }
    }
}

// Fills the screen in mode 4 using specified palette index
void fillScreen4(volatile unsigned char colorIndex)
{
    volatile unsigned int color = colorIndex << 24 | colorIndex << 16 |
                                  colorIndex << 8 | colorIndex;
    DMANow(3, &color, videoBuffer, DMA_SOURCE_FIXED | DMA_32 | (38400 / 4));
}

// Draws an image in mode 4
void drawImage4(int x, int y, int width, int height, const unsigned short *image)
{
    for (int i = 0; i < height; i++)
    {
        DMANow(3, &image[OFFSET(0, i, width) / 2], &videoBuffer[OFFSET(x, y + i, SCREENWIDTH) / 2], width / 2);
    }
}

// Draws a full-screen image in mode 4
void drawFullscreenImage4(const unsigned short *image)
{
    DMANow(3, image, videoBuffer, (SCREENWIDTH * SCREENHEIGHT) / 2);
}

// Flips page being displayed and page being drawn to
void flipPage()
{
    if (REG_DISPCTL & DISP_BACKBUFFER)
    {
        videoBuffer = BACKBUFFER;
    }
    else
    {
        videoBuffer = FRONTBUFFER;
    }
    REG_DISPCTL ^= DISP_BACKBUFFER;
}

// Draws the specified character at the specified location in Mode 4
void drawChar4(int x, int y, char ch, unsigned short colorIndex)
{
    for (int r = 0; r < 8; r++)
    {
        for (int c = 0; c < 6; c++)
        {
            if (fontdata_6x8[48 * ch + OFFSET(c, r, 6)])
            {
                setPixel4(x + c, y + r, colorIndex);
            }
        }
    }
}

// Draws the specified string at the specified location in Mode 4
void drawString4(int x, int y, char *str, unsigned short colorIndex)
{
    // until the null character appears
    while (*str != '\0')
    {
        drawChar4(x, y, *str, colorIndex);
        x += 6;
        // point to the next character
        str++;
    }
}