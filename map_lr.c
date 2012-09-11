#include "common.h"

// wrap from the left to the right and vice versa

static int width;

void map_init(Display *dpy) {
  width = DisplayWidth (dpy, DefaultScreen(dpy));
}

void map(int *x, int *y) {
  WRAP(x, 0, width-2);		// wrap left to right
  WRAP(x, width-1, 1);		// wrap right to left
}
