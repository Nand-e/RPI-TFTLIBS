
#include <stdlib.h>

#include "../GFX/GFX.h"
#include "glcdfont.c"


GFX::GFX (int16_t w, int16_t h):
  WIDTH(w), HEIGHT(h)
{
  _width    = WIDTH;
  _height   = HEIGHT;
  rotation  = 0;
  cursor_y  = cursor_x    = 0;
  textsize  = 1;
  textcolor = textbgcolor = 0xFFFF;
  wrap      = true;
}

// Draw a circle outline
void GFX::drawCircle(int16_t x0, int16_t y0, int16_t r,
    uint16_t color) {
  int16_t f = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x = 0;
  int16_t y = r;

  drawPixel(x0  , y0+r, color);
  drawPixel(x0  , y0-r, color);
  drawPixel(x0+r, y0  , color);
  drawPixel(x0-r, y0  , color);

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x;
  
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 + x, y0 - y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + y, y0 + x, color);
    drawPixel(x0 - y, y0 + x, color);
    drawPixel(x0 + y, y0 - x, color);
    drawPixel(x0 - y, y0 - x, color);
  }
}

void GFX::drawCircleHelper( int16_t x0, int16_t y0,
               int16_t r, uint8_t cornername, uint16_t color) {
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;
    if (cornername & 0x4) {
      drawPixel(x0 + x, y0 + y, color);
      drawPixel(x0 + y, y0 + x, color);
    } 
    if (cornername & 0x2) {
      drawPixel(x0 + x, y0 - y, color);
      drawPixel(x0 + y, y0 - x, color);
    }
    if (cornername & 0x8) {
      drawPixel(x0 - y, y0 + x, color);
      drawPixel(x0 - x, y0 + y, color);
    }
    if (cornername & 0x1) {
      drawPixel(x0 - y, y0 - x, color);
      drawPixel(x0 - x, y0 - y, color);
    }
  }
}

void GFX::fillCircle(int16_t x0, int16_t y0, int16_t r,
			      uint16_t color) {
  drawFastVLine(x0, y0-r, 2*r+1, color);
  fillCircleHelper(x0, y0, r, 3, 0, color);
}

// Used to do circles and roundrects
void GFX::fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
    uint8_t cornername, int16_t delta, uint16_t color) {

  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f     += ddF_y;
    }
    x++;
    ddF_x += 2;
    f     += ddF_x;

    if (cornername & 0x1) {
      drawFastVLine(x0+x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0+y, y0-x, 2*x+1+delta, color);
    }
    if (cornername & 0x2) {
      drawFastVLine(x0-x, y0-y, 2*y+1+delta, color);
      drawFastVLine(x0-y, y0-x, 2*x+1+delta, color);
    }
  }
}

// Bresenham's algorithm - thx wikpedia
void GFX::drawLine(int16_t x0, int16_t y0,
			    int16_t x1, int16_t y1,
			    uint16_t color) {
  int16_t steep = abs(y1 - y0) > abs(x1 - x0);
  if (steep) {
    swapp(x0, y0);
    swapp(x1, y1);
  }

  if (x0 > x1) {
    swapp(x0, x1);
    swapp(y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0<=x1; x0++) {
    if (steep) {
      drawPixel(y0, x0, color);
    } else {
      drawPixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}

// Draw a rectangle
void GFX::drawRect(int16_t x, int16_t y,
			    int16_t w, int16_t h,
			    uint16_t color) {
  drawFastHLine(x, y, w, color);
  drawFastHLine(x, y+h-1, w, color);
  drawFastVLine(x, y, h, color);
  drawFastVLine(x+w-1, y, h, color);
}

void GFX::drawFastVLine(int16_t x, int16_t y,
				 int16_t h, uint16_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x, y+h-1, color);
}

void GFX::drawFastHLine(int16_t x, int16_t y,
				 int16_t w, uint16_t color) {
  // Update in subclasses if desired!
  drawLine(x, y, x+w-1, y, color);
}

void GFX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
			    uint16_t color) {
  // Update in subclasses if desired!
  for (int16_t i=x; i<x+w; i++) {
    drawFastVLine(i, y, h, color);
  }
}

void GFX::fillScreen(uint16_t color) {
  fillRect(0, 0, _width, _height, color);
}

// Draw a rounded rectangle
void GFX::drawRoundRect(int16_t x, int16_t y, int16_t w,
  int16_t h, int16_t r, uint16_t color) {
  // smarter version
  drawFastHLine(x+r  , y    , w-2*r, color); // Top
  drawFastHLine(x+r  , y+h-1, w-2*r, color); // Bottom
  drawFastVLine(x    , y+r  , h-2*r, color); // Left
  drawFastVLine(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
  drawCircleHelper(x+r    , y+r    , r, 1, color);
  drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
  drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
  drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
}

// Fill a rounded rectangle
void GFX::fillRoundRect(int16_t x, int16_t y, int16_t w,
				 int16_t h, int16_t r, uint16_t color) {
  // smarter version
  fillRect(x+r, y, w-2*r, h, color);

  // draw four corners
  fillCircleHelper(x+w-r-1, y+r, r, 1, h-2*r-1, color);
  fillCircleHelper(x+r    , y+r, r, 2, h-2*r-1, color);
}

// Draw a triangle
void GFX::drawTriangle(int16_t x0, int16_t y0,
				int16_t x1, int16_t y1,
				int16_t x2, int16_t y2, uint16_t color) {
  drawLine(x0, y0, x1, y1, color);
  drawLine(x1, y1, x2, y2, color);
  drawLine(x2, y2, x0, y0, color);
}


// Fill a triangle - original Adafruit function
void GFX::fillTriangle ( int16_t x0, int16_t y0,
				  int16_t x1, int16_t y1,
				  int16_t x2, int16_t y2, uint16_t color) {

  int16_t a, b, y, last;

  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swapp(y0, y1); swapp(x0, x1);
  }
  if (y1 > y2) {
    swapp(y2, y1); swapp(x2, x1);
  }
  if (y0 > y1) {
    swapp(y0, y1); swapp(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  int16_t
    dx01 = x1 - x0,
    dy01 = y1 - y0,
    dx02 = x2 - x0,
    dy02 = y2 - y0,
    dx12 = x2 - x1,
    dy12 = y2 - y1,
    sa   = 0,
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if(a > b) swapp(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if(a > b) swapp(a,b);
    drawFastHLine(a, y, b-a+1, color);
  }
}

/*
void Adafruit_GFX_AS::drawBitmap(int16_t x, int16_t y,
			      const uint8_t *bitmap, int16_t w, int16_t h,
			      uint16_t color) {

  int16_t i, j, byteWidth = (w + 7) / 8;

  for(j=0; j<h; j++) {
    for(i=0; i<w; i++ ) {
      if(pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
	drawPixel(x+i, y+j, color);
      }
    }
  }
}

#if ARDUINO >= 100
size_t Adafruit_GFX_AS::write(uint8_t c) {
#else
void Adafruit_GFX_AS::write(uint8_t c) {
#endif
  if (c == '\n') {
    cursor_y += textsize*8;
    cursor_x  = 0;
  } else if (c == '\r') {
    // skip em
  } else {
    drawChar(cursor_x, cursor_y, c, textcolor, textbgcolor, textsize);
    cursor_x += textsize*6;
    if (wrap && (cursor_x > (_width - textsize*6))) {
      cursor_y += textsize*8;
      cursor_x = 0;
    }
  }
#if ARDUINO >= 100
  return 1;
#endif
}
*/


// Draw a character - only used for the original Adafruit font
void GFX::drawChar(int16_t x, int16_t y, unsigned char c,
			    uint16_t color, uint16_t bg, uint8_t size) {
#ifdef LOAD_GLCD
  if((x >= _width)            || // Clip right
     (y >= _height)           || // Clip bottom
     ((x + 6 * size - 1) < 0) || // Clip left
     ((y + 8 * size - 1) < 0))   // Clip top
    return;

  for (int8_t i=0; i<6; i++ ) {
    uint8_t line;
    if (i == 5) 
      line = 0x0;
    else 
      line = font[c*5+i];
    for (int8_t j = 0; j<8; j++) {
      if (line & 0x1) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, color);
        else {  // big size
          fillRect(x+(i*size), y+(j*size), size, size, color);
        } 
      } else if (bg != color) {
        if (size == 1) // default size
          drawPixel(x+i, y+j, bg);
        else {  // big size
          fillRect(x+i*size, y+j*size, size, size, bg);
        }
      }
      line >>= 1;
    }
  }
#endif
}


void GFX::setCursor(int16_t x, int16_t y) {
  cursor_x = x;
  cursor_y = y;
}

void GFX::setTextSize(uint8_t s) {
  textsize = (s > 0) ? s : 1;
}

void GFX::setTextColor(uint16_t c) {
  // For 'transparent' background, we'll set the bg 
  // to the same as fg instead of using a flag
  textcolor = textbgcolor = c;
}

void GFX::setTextColor(uint16_t c, uint16_t b) {
  textcolor   = c;
  textbgcolor = b; 
}

void GFX::setTextWrap(boolean w) {
  wrap = w;
}

uint8_t GFX::getRotation(void) {
  return rotation;
}

void GFX::setRotation(uint8_t x) {
  rotation = (x & 3);
  switch(rotation) {
   case 0:
   case 2:
    _width  = WIDTH;
    _height = HEIGHT;
    break;
   case 1:
   case 3:
    _width  = HEIGHT;
    _height = WIDTH;
    break;
  }
}

// Return the size of the display (per current rotation)
int16_t GFX::width(void) {
  return _width;
}
 
int16_t GFX::height(void) {
  return _height;
}

void GFX::invertDisplay(boolean i) {
  // Do nothing, must be subclassed if supported
}

/***************************************************************************************
** Function name:           drawChar
** Description:             draw a unicode onto the screen
***************************************************************************************/
int GFX::drawChar(unsigned int uniCode, int x, int y, int size)
{
  unsigned int width = 0;
//  unsigned int height = 0;
 // unsigned int flash_address = 0; // 16 bit address OK for Arduino if font files <60K

  int8_t gap = 0;
  uniCode -= 32; // Not using info from font files at the moment

  switch(size) {
     case 1:
     case 2:
      drawChar(x, y,uniCode+32,textcolor, textbgcolor, size);
      return 6*size;

/*
  #ifdef LOAD_FONT2
    case 2:
      flash_address = & chrtbl_f16[uniCode] ;
      width = pgm_read_byte(widtbl_f16+uniCode);
      height = chr_hgt_f16;
      gap = 1;
      break;
  #endif
*/
//    case 3:
//      flash_address = pgm_read_word(&chrtbl_f24[uniCode]);
//      width = pgm_read_byte(widtbl_f24+uniCode);
//      height = chr_hgt_f24;
//      gap = 0;
//      break;
/*
  #ifdef LOAD_FONT4
    case 4:
      flash_address = pgm_read_word(&chrtbl_f32[uniCode]);
      width = pgm_read_byte(widtbl_f32+uniCode);
      height = chr_hgt_f32;
      gap = -3;
      break;
  #endif
*/
//    case 5:
//      flash_address = pgm_read_word(&chrtbl_f48[uniCode]);
//      width = pgm_read_byte(widtbl_f48+uniCode);
//      height = chr_hgt_f48;
//      gap = -3;
//      break;
/*
  #ifdef LOAD_FONT6
    case 6:
      flash_address = pgm_read_word(&chrtbl_f64[uniCode]);
      width = pgm_read_byte(widtbl_f64+uniCode);
      height = chr_hgt_f64;
      gap = -3;
      break;
  #endif

  #ifdef LOAD_FONT7
    case 7:
      flash_address = pgm_read_word(&chrtbl_f7s[uniCode]);
      width = pgm_read_byte(widtbl_f7s+uniCode);
      height = chr_hgt_f7s;
      gap = 2;
      break;
  #endif

  #ifdef LOAD_FONT8
    case 8:
      flash_address = pgm_read_word(&chrtbl_f72[uniCode]);
      width = pgm_read_byte(widtbl_f72+uniCode);
      height = chr_hgt_f72;
      gap = 2;
      break;
  #endif
*/
    default:
      return 0;
  }

//	if (x+(width+gap)*textsize >= _width) return (width+gap)*textsize ;

//	int w = (width+7)/8;
//	int pX      = 0;
//	int pY      = y;
//	unsigned char line = 0;

	/*
  for(unsigned int i=0; i<height; i++)
  {
    if (textcolor != textbgcolor) {
      if (textsize == 1) drawFastHLine(x, pY, width+gap, textbgcolor);
      else fillRect(x, pY, (width+gap)*textsize, textsize, textbgcolor);
    }
    for (int k = 0;k < w; k++)
    { 
      line = *( flash_address+w*i+k );
      if(line) {
        if (textsize==1){
          pX = x + k*8;
          if(line & 0x80) drawPixel(pX, pY, textcolor);
          if(line & 0x40) drawPixel(pX+1, pY, textcolor);
          if(line & 0x20) drawPixel(pX+2, pY, textcolor);
          if(line & 0x10) drawPixel(pX+3, pY, textcolor);
          if(line & 0x8) drawPixel(pX+4, pY, textcolor);
          if(line & 0x4) drawPixel(pX+5, pY, textcolor);
          if(line & 0x2) drawPixel(pX+6, pY, textcolor);
          if(line & 0x1) drawPixel(pX+7, pY, textcolor);
        }
        else {
          pX = x + k*8*textsize;
          if(line & 0x80) fillRect(pX, pY, textsize, textsize, textcolor);
          if(line & 0x40) fillRect(pX+textsize, pY, textsize, textsize, textcolor);
          if(line & 0x20) fillRect(pX+2*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x10) fillRect(pX+3*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x8) fillRect(pX+4*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x4) fillRect(pX+5*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x2) fillRect(pX+6*textsize, pY, textsize, textsize, textcolor);
          if(line & 0x1) fillRect(pX+7*textsize, pY, textsize, textsize, textcolor);
        }
      }
    }
    pY+=textsize;
	}
	*/
	return (width+gap)*textsize;        // x +
}

/***************************************************************************************
** Function name:           Fast character drawing support functions
***************************************************************************************/

void GFX::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
  // Do nothing, MUST be subclassed
}


/***************************************************************************************
** Function name:           drawNumber unsigned with size
** Description:             draw a long integer
***************************************************************************************/
int GFX::drawNumber(long long_num,int poX, int poY, int size)
{
   // char tmp[12];
  //  if (long_num < 0) sprintf(tmp, "%li", long_num);
  //  else sprintf(tmp, "%lu", long_num);
    return 0; // drawString(tmp, poX, poY, size);
}

/***************************************************************************************
** Function name:           drawString
** Description :            draw string
***************************************************************************************/

int GFX::drawString(char *string, int poX, int poY, int size)
{
    int sumX = 0;

    while(*string)
    {
        int xPlus = drawChar(*string, poX, poY, size);
        sumX += xPlus;
        *string++;
        poX += xPlus;                            //M ove cursor right     
    }
    return sumX;
}

/***************************************************************************************
** Function name:           drawCentreString
** Descriptions:            draw string centred on dX
***************************************************************************************/
/*
int Adafruit_GFX_AS::drawCentreString(char *string, int dX, int poY, int size)
{
    int sumX = 0;
    int len = 0;
    char *pointer = string;
    char ascii;

    while(*pointer)
    {
        ascii = *pointer;

#ifdef LOAD_GLCD
        if (size==1)len += 6;
#endif
#ifdef LOAD_FONT2
        if (size==2)len += 1+pgm_read_byte(widtbl_f16+ascii-32);
#endif
        //if (size==3)len += 1+pgm_read_byte(widtbl_f48+ascii-32)/2;
#ifdef LOAD_FONT4
        if (size==4)len += pgm_read_byte(widtbl_f32+ascii-32)-3;
#endif
        //if (size==5) len += pgm_read_byte(widtbl_f48+ascii-32)-3;
#ifdef LOAD_FONT6
        if (size==6) len += pgm_read_byte(widtbl_f64+ascii-32)-3;
#endif
#ifdef LOAD_FONT7
        if (size==7) len += pgm_read_byte(widtbl_f7s+ascii-32)+2;
#endif
#ifdef LOAD_FONT8
        if (size==8) len += pgm_read_byte(widtbl_f72+ascii-32)+2;
#endif

        *pointer++;
    }
    len = len*textsize;
    int poX = dX - len/2;

    if (poX < 0) poX = 0;

    while(*string)
    {
      int xPlus =0; // drawChar(*string, poX, poY, size);
      sumX += xPlus;
      *string++;
      poX += xPlus;                  // Move cursor right
    }
    
    return sumX;
}
*/
/***************************************************************************************
** Function name:           drawRightString
** Descriptions:            draw string right justified to dX
***************************************************************************************/
/*
int Adafruit_GFX_AS::drawRightString(char *string, int dX, int poY, int size)
{
    int sumX = 0;
    int len = 0;
    char *pointer = string;
    char ascii;

    while(*pointer)
    {
        ascii = *pointer;

#ifdef LOAD_GLCD
        if (size==1)len += 6;
#endif
#ifdef LOAD_FONT2
        if (size==2)len += 1+pgm_read_byte(widtbl_f16+ascii-32);
#endif
        //if (size==3)len += 1+pgm_read_byte(widtbl_f48+ascii-32)/2;
#ifdef LOAD_FONT4
        if (size==4)len += pgm_read_byte(widtbl_f32+ascii-32)-3;
#endif
        //if (size==5) len += pgm_read_byte(widtbl_f48+ascii-32)-3;
#ifdef LOAD_FONT6
        if (size==6) len += pgm_read_byte(widtbl_f64+ascii-32)-3;
#endif
#ifdef LOAD_FONT7
        if (size==7) len += pgm_read_byte(widtbl_f7s+ascii-32)+2;
#endif
#ifdef LOAD_FONT8
        if (size==8) len += pgm_read_byte(widtbl_f72+ascii-32)+2;
#endif

        *pointer++;
    }
    
    len = len*textsize;
    int poX = dX - len;

    if (poX < 0) poX = 0;

    while(*string)
    {
      int xPlus = 0; // drawChar(*string, poX, poY, size);
      sumX += xPlus;
      *string++;
      poX += xPlus;          // Move cursor right
    }
    
    return sumX;
}
*/
/***************************************************************************************
** Function name:           drawFloat
** Descriptions:            drawFloat
***************************************************************************************/
/*int Adafruit_GFX_AS::drawFloat(float floatNumber, int decimal, int poX, int poY, int size)
{
    unsigned long temp=0;
    float decy=0.0;
    float rounding = 0.5;
    
    float eep = 0.000001;
    
    int sumX    = 0;
    int xPlus   = 0;
    
    if(floatNumber-0.0 < eep)       // floatNumber < 0
    {
        xPlus = drawChar('-',poX, poY, size);
        floatNumber = -floatNumber;

        poX  += xPlus; 
        sumX += xPlus;
    }
    
    for (unsigned char i=0; i<decimal; ++i)
    {
        rounding /= 10.0;
    }
    
    floatNumber += rounding;

    temp = (long)floatNumber;
    
    
    xPlus = drawNumber(temp,poX, poY, size);

    poX  += xPlus; 
    sumX += xPlus;

    if(decimal>0)
    {
        xPlus = drawChar('.',poX, poY, size);
        poX += xPlus;                            // Move cursor right
        sumX += xPlus;
    }
    else
    {
        return sumX;
    }
    
    decy = floatNumber - temp;
    for(unsigned char i=0; i<decimal; i++)                                      
    {
        decy *= 10;                              //   for the next decimal
        temp = decy;                              //  get the decimal
        xPlus = drawNumber(temp,poX, poY, size);
        
        poX += xPlus;                             // Move cursor right
        sumX += xPlus;
        decy -= temp;
    }
    return sumX;
}
*/
