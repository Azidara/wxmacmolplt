#ifndef __AtomPatterns__
#define __AtomPatterns__

#ifdef __WXOSX_CARBON__
#include <AGL/glu.h>
#elif __WXOSX_COCOA__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

const int kNumPatterns = 29;

const GLubyte atomMaskPatterns[kNumPatterns][128] =
  {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  //pattern 0

  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
    0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}, //pattern 1

  {0xc0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00,
    0x00, 0xc0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00,
    0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x80, 0x00,
    0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x18, 0x00,
    0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x01, 0x80,
    0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x18,
   0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x01}, //pattern 2

  {0x80, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c,
   0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0,
   0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00,
   0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x00,
   0x00, 0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00,
   0x00, 0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00,
   0x01, 0x80, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
   0x18, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00},  //pattern 3

  {0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0x30, 0x00, 0x18, 0x00, 0x18, 0x00,
    0x0c, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x00, 0x01, 0x80, 0x01, 0x80,
    0x00, 0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0x30, 0x00, 0x18, 0x00, 0x18,
    0x00, 0x0c, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x80, 0x01, 0x80, 0x01,
    0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0x30, 0x00, 0x18, 0x00, 0x18, 0x00,
    0x0c, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x00, 0x01, 0x80, 0x01, 0x80,
    0x00, 0xc0, 0x00, 0xc0, 0x00, 0x60, 0x00, 0x60, 0x00, 0x30, 0x00, 0x30, 0x00, 0x18, 0x00, 0x18,
    0x00, 0x0c, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x06, 0x00, 0x03, 0x00, 0x03, 0x80, 0x01, 0x80, 0x01}, //pattern 4

  {0x80, 0x01, 0x80, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c,
   0x00, 0x18, 0x00, 0x18, 0x00, 0x30, 0x00, 0x30, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0,
   0x01, 0x80, 0x01, 0x80, 0x03, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c, 0x00,
   0x18, 0x00, 0x18, 0x00, 0x30, 0x00, 0x30, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0, 0x00,
   0x80, 0x01, 0x80, 0x01, 0x00, 0x03, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c,
   0x00, 0x18, 0x00, 0x18, 0x00, 0x30, 0x00, 0x30, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0,
   0x01, 0x80, 0x01, 0x80, 0x03, 0x00, 0x03, 0x00, 0x06, 0x00, 0x06, 0x00, 0x0c, 0x00, 0x0c, 0x00,
   0x18, 0x00, 0x18, 0x00, 0x30, 0x00, 0x30, 0x00, 0x60, 0x00, 0x60, 0x00, 0xc0, 0x00, 0xc0, 0x00}, //pattern 5

  {0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01},  //pattern 6

  {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   },  //pattern 7

  {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01,
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
   0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
   0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
   0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80,
   },  //pattern 8 "brick"
{0xf0, 0x0f, 0xf0, 0x0f, 0xe0, 0x07, 0xe0, 0x07, 0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x80, 0x01,
   0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 
   0xc0, 0x03, 0xc0, 0x03, 0xe0, 0x07, 0xe0, 0x07, 0xf0, 0x0f, 0xf0, 0x0f, 0xf8, 0x1f, 0xf8, 0x1f,
   0xf0, 0x0f, 0xf0, 0x0f, 0xe0, 0x07, 0xe0, 0x07, 0xc0, 0x03, 0xc0, 0x03, 0x80, 0x01, 0x80, 0x01,
   0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 
   0xc0, 0x03, 0xc0, 0x03, 0xe0, 0x07, 0xe0, 0x07, 0xf0, 0x0f, 0xf0, 0x0f, 0xf8, 0x1f, 0xf8, 0x1f
   },  //pattern 9

  {0xe0, 0x00, 0x38, 0x00, 0xe7, 0x00, 0x39, 0xc0, 0xe7, 0x00, 0x39, 0xc0, 0x07, 0x00, 0x01, 0xc0,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x0e, 0x1c, 0x1c, 0x00, 0x0e,
   0x1c, 0x00, 0xe0, 0x00, 0x1c, 0xe0, 0xe7, 0x00, 0x00, 0xe0, 0xe7, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x00, 0x00, 0x1c,
   0xe0, 0x00, 0x38, 0x00, 0xe7, 0x00, 0x39, 0xc0, 0xe7, 0x00, 0x39, 0xc0, 0x07, 0x00, 0x01, 0xc0,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x0e, 0x00, 0x1c, 0x00, 0x0e, 0x1c, 0x1c, 0x00, 0x0e,
   0x1c, 0x00, 0xe0, 0x00, 0x1c, 0xe0, 0xe7, 0x00, 0x00, 0xe0, 0xe7, 0x00, 0x00, 0xe0, 0x07, 0x00,
   0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x03, 0x80, 0x1c, 0x00, 0x00, 0x00, 0x1c
   },  //pattern 10

  {0xc0, 0x18, 0xc0, 0x18, 0x60, 0x30, 0x60, 0x30, 0x30, 0x60, 0x30, 0x60, 0x18, 0xc0, 0x18, 0xc0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80,
    0x18, 0xc0, 0x18, 0xc0, 0x30, 0x60, 0x30, 0x60, 0x60, 0x30, 0x60, 0x30, 0xc0, 0x18, 0xc0, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00,
    0xc0, 0x18, 0xc0, 0x18, 0x60, 0x30, 0x60, 0x30, 0x30, 0x60, 0x30, 0x60, 0x18, 0xc0, 0x18, 0xc0,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x18, 0xc0, 0x18, 0xc0, 0x30, 0x60, 0x30, 0x60, 0x60, 0x30, 0x60, 0x30, 0xc0, 0x18, 0xc0, 0x18,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00}, //pattern 11

  {0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00,
   0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38,
    0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07,
    0xe0, 0x18, 0xc0, 0x38, 0xe0, 0x18, 0xc0, 0x38, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7,
   0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00,
   0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0x18, 0xc0, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38,
    0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07, 0x00, 0x07,
    0xe0, 0x18, 0xc0, 0x38, 0xe0, 0x18, 0xc0, 0x38, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7, 0x38, 0xe7
    }, //pattern 12

  {0x80, 0x00, 0x80, 0x00, 0x40, 0x01, 0x40, 0x01, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x00, 0x80, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x00, 0x80, 0x00, 0x40, 0x01, 0x40, 0x01, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x80, 0x00, 0x80, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   },  //pattern 13

  {0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,
   0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88
   },  //pattern 14

  {0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   },  //pattern 15

  {0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   },  //pattern 16

  {0x7f, 0xfc, 0xff, 0xfe, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0xcf, 0xfc, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x7f, 0xfc, 0xff, 0xfe, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02,
   0x60, 0x04, 0xc0, 0x02, 0x60, 0x04, 0xc0, 0x02, 0x7f, 0xfc, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00
   },  //pattern 17

  {0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x88, 0x88, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00
   },  //pattern 18

  {0x80, 0x80, 0x80, 0x80, 0x42, 0x42, 0x42, 0x42, 0xa5, 0xa5, 0xa5, 0xa5, 0x42, 0x42, 0x42, 0x42,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x42, 0x42, 0x42, 0x42, 0xa5, 0xa5, 0xa5, 0xa5, 0x42, 0x42, 0x42, 0x42,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x42, 0x42, 0x42, 0x42, 0xa5, 0xa5, 0xa5, 0xa5, 0x42, 0x42, 0x42, 0x42,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x42, 0x42, 0x42, 0x42, 0xa5, 0xa5, 0xa5, 0xa5, 0x42, 0x42, 0x42, 0x42,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
   },  //pattern 19

  {0x80, 0x80, 0x80, 0x80, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20, 0x04, 0x10, 0x04, 0x10,
   0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x20, 0x02, 0x40, 0x01, 0x40, 0x01,
   0x80, 0x80, 0x80, 0x80, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20, 0x04, 0x10, 0x04, 0x10,
   0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x20, 0x02, 0x40, 0x01, 0x40, 0x01,
   0x80, 0x80, 0x80, 0x80, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20, 0x04, 0x10, 0x04, 0x10,
   0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x20, 0x02, 0x40, 0x01, 0x40, 0x01,
   0x80, 0x80, 0x80, 0x80, 0x01, 0x40, 0x01, 0x40, 0x02, 0x20, 0x02, 0x20, 0x04, 0x10, 0x04, 0x10,
   0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x20, 0x02, 0x40, 0x01, 0x40, 0x01 
   },  //pattern 20

  {0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x02, 0x20, 0x02, 0x20, 0x00, 0x00, 0x00, 0x00,
   0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00 
   },  //pattern 21

  {0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
   0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
   },  //pattern 22

  {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
   0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
   0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
   0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
   0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
   0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
   0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc,
   0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc
   },    //pattern 23

  {0x80, 0x80, 0x80, 0x80, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x20, 0x02,
   0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x80, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x20, 0x02,
   0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x80, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x20, 0x02,
   0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x80, 0x00,
   0x80, 0x80, 0x80, 0x80, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04, 0x20, 0x02, 0x20, 0x02,
   0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x40, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x80, 0x00
   },    //pattern 24

  {0x80, 0x80, 0x80, 0x80, 0x04, 0x10, 0x04, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04,
   0x20, 0x02, 0x20, 0x02, 0x40, 0x01, 0x40, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x80, 0x00,
   0x80, 0x00, 0x80, 0x00, 0x80, 0x01, 0x80, 0x01, 0x40, 0x01, 0x40, 0x01, 0x20, 0x02, 0x20, 0x02,
   0x10, 0x04, 0x10, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x10, 0x04, 0x10, 0x80, 0x80, 0x80, 0x80,
   0x80, 0x80, 0x80, 0x80, 0x04, 0x10, 0x04, 0x10, 0x08, 0x08, 0x08, 0x08, 0x10, 0x04, 0x10, 0x04,
   0x20, 0x02, 0x20, 0x02, 0x40, 0x01, 0x40, 0x01, 0x80, 0x01, 0x80, 0x01, 0x80, 0x00, 0x80, 0x00,
   0x80, 0x00, 0x80, 0x00, 0x80, 0x01, 0x80, 0x01, 0x40, 0x01, 0x40, 0x01, 0x20, 0x02, 0x20, 0x02,
   0x10, 0x04, 0x10, 0x04, 0x08, 0x08, 0x08, 0x08, 0x04, 0x10, 0x04, 0x10, 0x80, 0x80, 0x80, 0x80
   },    //pattern 25

  {0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
   0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
   0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
   0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
   0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
   0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
   0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
   0xff, 0xff, 0xff, 0xff, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11
   }, //pattern 26

  {0xfc, 0x0f, 0xfc, 0x0f, 0xe6, 0x07, 0xe6, 0x07, 0xc3, 0x03, 0xc3, 0x83, 0x83, 0x01, 0x83, 0x01,
   0x06, 0x03, 0x06, 0x03, 0x0c, 0x06, 0x0c, 0x06, 0x18, 0x0c, 0x18, 0x0c, 0x30, 0x18, 0x30, 0x18,
   0x60, 0x30, 0x60, 0x30, 0xc0, 0x60, 0xc0, 0x60, 0x80, 0xc1, 0x80, 0xc1, 0x81, 0x83, 0x81, 0x83, 
   0xc0, 0xc7, 0xc0, 0xc7, 0xe0, 0x6f, 0xe0, 0x6f, 0xf0, 0x3f, 0xf0, 0x3f, 0xf8, 0x1f, 0xf8, 0x1f,
   0xfc, 0x0f, 0xfc, 0x0f, 0xe6, 0x07, 0xe6, 0x07, 0xc3, 0x03, 0xc3, 0x03, 0x83, 0x01, 0x83, 0x01,
   0x06, 0x03, 0x06, 0x03, 0x0c, 0x06, 0x0c, 0x06, 0x18, 0x0c, 0x18, 0x0c, 0x30, 0x18, 0x30, 0x18,
   0x60, 0x30, 0x60, 0x30, 0xc0, 0x60, 0xc0, 0x60, 0x80, 0xc1, 0x80, 0xc1, 0x81, 0x83, 0x81, 0x83, 
   0xc0, 0xc7, 0xc0, 0xc7, 0xe0, 0x6f, 0xe0, 0x6f, 0xf0, 0x3f, 0xf0, 0x3f, 0xf8, 0x1f, 0xf8, 0x1f
   },  //pattern 27

  {0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
   0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80
  }  //pattern 28

  };

#endif
