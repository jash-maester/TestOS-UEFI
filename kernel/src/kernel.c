typedef unsigned long long size_t;
typedef struct {
	void* BaseAddress;
	size_t BufferSize;
	unsigned int Width;
	unsigned int Height;
	unsigned int PixelsPerScanLine;
}Framebuffer;

typedef struct
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charSize;
} PSF1_HEADER;

typedef struct
{
	PSF1_HEADER* psf1_Header;
	void* glyphBuffer;
} PSF1_FONT;

typedef struct {
    unsigned int X;
    unsigned int Y;
} Point;

void putChar(Framebuffer* framebuffer, PSF1_FONT* psf1_font, unsigned int color, char chr, unsigned int xOff, unsigned int yOff)
{
    unsigned int* pixPtr = (unsigned int*)framebuffer->BaseAddress;
    char* fontPtr = psf1_font->glyphBuffer + (chr * psf1_font->psf1_Header->charSize);

    for(unsigned long y= yOff; y<yOff+16;y++)
    {
        for(unsigned long x=xOff;x<xOff+8;x++)
        {
            if((*fontPtr & (0b10000000 >> (x-xOff))) > 0 )
            {
                *(unsigned int*)(pixPtr+x+(y*framebuffer->PixelsPerScanLine)) = color;
            }
        }
        fontPtr++;
    }
}

Point CursorPosition;

void PrintStr(Framebuffer* framebuffer, PSF1_FONT* psf1_font, unsigned int color, char* str)
{
    char* chr = str;
    while(*chr!=0)
    {
        putChar(framebuffer, psf1_font, color, *chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if(CursorPosition.X + 8 > framebuffer->Width)
        {
            CursorPosition.X=0;
            CursorPosition.Y+=18;
        }
        chr++;
    }
}

void _start(Framebuffer* framebuffer, PSF1_FONT* psf1_font) {

// To draw a Line
/*	unsigned int y = 50;
	unsigned int BBP = 4;
	
	for(unsigned int x=0; x<framebuffer->Width/2*BBP;x+=BBP)
	{
		*(unsigned int*)(x+(y*framebuffer->PixelsPerScanLine*BBP) + framebuffer->BaseAddress) = 0xff00ffff;
	}
*/

    //putChar(framebuffer, psf1_font, 0xff00ffff, 'G', 10, 10);
    CursorPosition.X=10;
    CursorPosition.Y=10;
    char str[] = "Hello World";
    for(int t=0; t<50; t+=1)
    {
        PrintStr(framebuffer, psf1_font, 0xff00ffff, str);
    }

    /*
    putChar(framebuffer, psf1_font, 0xff00ffff, 'H', CursorPosition.X, 10);
    putChar(framebuffer, psf1_font, 0xff00ffff, 'E', CursorPosition.X+8, 10);
    putChar(framebuffer, psf1_font, 0xff00ffff, 'L', CursorPosition.X+16, 10);
    putChar(framebuffer, psf1_font, 0xff00ffff, 'L', CursorPosition.X+24, 10);
    putChar(framebuffer, psf1_font, 0xff00ffff, 'O', CursorPosition.X+32, 10);
    */

    return ;
}