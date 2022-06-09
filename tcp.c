typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long uint64;
typedef char int8;
typedef short int16;
typedef int int32;
typedef long int64;
typedef float real32;
typedef double real64;
typedef char bool8;

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#define true 1;
#define false 0;

int 
main(void)
{
  bool8 running = true; 
  uint32 status = 0;
  int32 x = -1, y = -1;
  Display *display = XOpenDisplay(0);
  Window root = DefaultRootWindow(display);
  XImage *image;
  XEvent event;
  XGrabPointer(display, root, False, ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, 
               GrabModeAsync, GrabModeAsync, root, 
               XCreateFontCursor(display, 86), CurrentTime);
  /* select pixel */
  while (running) {
    while (running && XPending(display)) {
      XNextEvent(display, &event);
      switch (event.type) {
      case (ButtonRelease):
        x = event.xbutton.x; 
        y = event.xbutton.y;  
        running = false;
        break; 
      }
    }
  }
  /* get image and hand it over to save function */
  if ((x > -1) && (y > -1)) {
    image = XGetImage(display, root, x, y, 1, 1, AllPlanes, ZPixmap);
    uint8 r = (uint8) image->data[2];
    uint8 g = (uint8) image->data[1];
    uint8 b = (uint8) image->data[0];
    printf("hex: #%02X%02X%02X\nrgb: %d,%d,%d\n",  r, g, b, r, g, b);
    XDestroyImage(image);
  } else {
    status = 1;
  }
  XCloseDisplay(display);
  return(status);
}
