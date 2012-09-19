#include "common.h"

// wrap from the top to the bottom and vice versa

static int height;

void map_init(Display *dpy) {
  height = DisplayHeight (dpy, DefaultScreen(dpy));
}

void map(int *x, int *y) {
  WRAP(y, 0, height-2);		// wrap top to bottom
  WRAP(y, height-1, 1);		// wrap bottom to top
}
