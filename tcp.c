#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

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

#define true 1;
#define false 0;

#define KB_Q 24
#define KB_C 54
#define KB_ESC 66

bool8 m_strcmp(char *str1, char *str2);

bool8
m_strcmp(char *str1, char *str2)
{
  uint8 result = true; 
  uint32 index = 0;
  while ((result = str1[index] == str2[index]) && 
         (str1[index] != '\0') && (str2[index] != '\0')) 
    ++index;
  if (((str1[index] != '\0') && (str1[index] != '\0')) ||
      ((str1[index] != '\0') && (str1[index] != '\0'))) 
    result = false;
  return(result);
}

int 
main(int argc, char *argv[])
{
  bool8 argmultiple = false;
  bool8 argcolor = true;
  bool8 running = true;
  uint8 rgb[3] = {0};
  Display *display = XOpenDisplay(0);
  Window rootwindow = DefaultRootWindow(display);
  XEvent event;
  XImage *image;
  XGrabPointer(display, rootwindow, False, 
               ButtonMotionMask | ButtonPressMask | ButtonReleaseMask, 
               GrabModeAsync, GrabModeAsync, rootwindow, 
               XCreateFontCursor(display, 34), CurrentTime);
  XGrabKeyboard(display, rootwindow, False,  
               GrabModeAsync, GrabModeAsync, CurrentTime);
  for (uint i = 0; i < argc; ++i) {
    if(m_strcmp(argv[i], "-m"))
      argmultiple = true;
    if(m_strcmp(argv[i], "-nc"))
      argcolor = false;
  }
  while (running) {
    XNextEvent(display, &event);
    switch (event.type)  {
    case (KeyPress):
      if (event.xkey.keycode == KB_Q || 
          event.xkey.keycode == KB_C ||
          event.xkey.keycode == KB_ESC) { 
        XUngrabPointer(display, CurrentTime);
        XUngrabKeyboard(display, CurrentTime);
        running = false;
      }
      break;
    case (ButtonRelease):
      image = XGetImage(display, rootwindow,
                        event.xbutton.x, event.xbutton.y,
                        1, 1, AllPlanes, ZPixmap);
      rgb = {(uint8) image->data[2], (uint8) image->data[1], b = (uint8) image->data[0]};
      if (argcolor) {
        printf("\033[48;2;%d;%d;%dm             \033[0m   ", rgb[0], rgb[1], rgb[2]);
      }
      printf("#%02X%02X%02X   %03d,%03d,%03d\n", rgb[0], rgb[1], rgb[2], rgb[0], rgb[1], rgb[2]);
      XDestroyImage(image);
      if (!argmultiple) {
        XUngrabPointer(display, CurrentTime);
        XUngrabKeyboard(display, CurrentTime);
        running = false;
      }
      break;
    }
  }
  XFlush(display);
  XSync(display, False);
  XCloseDisplay(display);
  return(0);
}
