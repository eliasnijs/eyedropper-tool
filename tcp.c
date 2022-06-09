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

#define global_variable static;

uint8 background_color[] = {0, 0, 0};
uint8 text_color[] = {0, 255, 0};
uint8 border_color[] = {15, 15, 15};

uint64
RGB_to_long(uint8 rgb[])
{
  return(rgb[2] + (rgb[1] << 8) + (rgb[0] << 16));
}

int 
main(void)
{
  Display *display = XOpenDisplay(0);
  int32 screen = DefaultScreen(display);
  Window rootwindow = DefaultRootWindow(display);
  uint64 background = RGB_to_long(background_color);
  uint64 foreground = WhitePixel(display, screen);
  Window window = XCreateSimpleWindow(display, rootwindow,
                                      300, 300, 300, 360, 5, 
                                      foreground, background);
  GC gc = XCreateGC(display, window, 0, 0);
  bool8 running = true;
  XEvent event;
  XImage *image;
  
  XSetBackground(display, gc, foreground);
  XSetForeground(display, gc, background);
  XClearWindow(display, window);
  XMapRaised(display, window);
  
  XGrabPointer(display, rootwindow, False, 
               ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, 
               GrabModeAsync, GrabModeAsync, rootwindow, 
               XCreateFontCursor(display, 22), CurrentTime);
  XGrabKeyboard(display, rootwindow, False,  
               GrabModeAsync, GrabModeAsync, 
               CurrentTime);

  GC rect_gc = XCreateGC(display, window, 0, 0);
  XSetForeground(display, rect_gc, 0);
  
  GC rect_border_gc = XCreateGC(display, window, 0, 0);
  XSetForeground(display, rect_border_gc, RGB_to_long(border_color));
  XSetLineAttributes(display, rect_border_gc, 2, LineSolid, CapNotLast, JoinBevel);
  
  GC text_gc = XCreateGC(display, window, 0, 0);
  XSetForeground(display, text_gc, RGB_to_long(text_color));

  char hex_string[12] = "#000000";
  char rgb_string[12] = "  0,  0,  0";
  while (running) {
    XClearWindow(display, window);
    
    XDrawString(display, window, text_gc ,25, 25, "Tiny Color Picker", 17);
    
    XDrawString(display, window, text_gc ,25, 125, "HEX: ", 5);
    XDrawString(display, window, text_gc ,75, 125, hex_string, 11);
    XDrawString(display, window, text_gc ,25, 150, "DEC: ", 5);
    XDrawString(display, window, text_gc ,75, 150, rgb_string, 11);

    XFillRectangle(display, window, rect_gc, 25, 175, 250, 162);
    XDrawRectangle(display, window, rect_border_gc, 25, 175, 250, 162);
    XNextEvent(display, &event);
    switch (event.type)  {
    case (KeyPress):
      printf("KEYPRESS");
      if ( event.xkey.keycode == 0x09 )
        running = false;
      XUngrabPointer(display, CurrentTime);
      XUngrabKeyboard(display, CurrentTime);
      break;
    case (ButtonRelease): {
      image = XGetImage(display, rootwindow, 
                        event.xbutton.x, event.xbutton.y,
                        1, 1, AllPlanes, ZPixmap);
      snprintf(hex_string, 12, "    #%02X%02X%02X", (uint8) image->data[2],  (uint8) image->data[1], (uint8) image->data[0]);
      snprintf(rgb_string, 12, "%3d,%3d,%3d", (uint8) image->data[2],  (uint8) image->data[1], (uint8) image->data[0]);
      XSetForeground(display, rect_gc,*((uint32 *) image->data));
      XDestroyImage(image);
      } break;
    }
  }

  XFlush(display);
  XSync(display, False);
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
  return(0);
}
