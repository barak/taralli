#include "common.h"

/* This file provides a map for a machine with
   the following screen layout:

   ┌──────────────┐
   │              │
   │    1920      │┌──────────┐
   │     x        ││   1366   │
   │    1200      ││    x     │
   │              ││   768    │
   └──────────────┘└──────────┘
*/

static int width, height;

void map_init(Display *dpy) {
  int screen = DefaultScreen(dpy);
  width  = DisplayWidth (dpy, screen);
  height = DisplayHeight(dpy, screen);
  if (width != 1920 + 1366) {
    fprintf(stderr, "width: %d\n", width);
  }
  if (height != 1200) {
    fprintf(stderr, "height: %d\n", height);
  }
}

void map(int *x, int *y) {
    int w = width;
    WRAP(x, 0, w-2);
    WRAP(x, w-1, 1);

    int h = (*x < 1920) ? 1200 : 768;
    WRAP(y, 0, h-2);
    WRAP(y, h-1, 1);
}
