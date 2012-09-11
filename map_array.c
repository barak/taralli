#include "common.h"

#ifdef NDEBUG
#define debug 0
#else
volatile int debug = 1;
#endif

// General routine for horizonal array of monitors.
// Wraps from left of leftmost to right of rightmost, and vice-versa.
// Wraps from top of each monitor to bottom, and vice-versa.

// For each monitor, list:
//  {width, height, vertical_offset}
// where vertical_offset is AT_TOP + the gap between the top of that
// screen and the top of the highest screen, or AT_BOTTOM + the gap
// between the bottom of that screen and the bottom of the lowest screen.

#define AT_TOP 0
#define AT_BOTTOM 1000000

// Examples.
//   ┌────────┐┌─────────────────┐┌────────┐
//   │        ││                 ││        │
//   │        ││                 ││        │
//   │  1200  ││   2560 × 1600   ││  1200  │
//   │   ×    ││                 ││   ×    │
//   │  1920  ││                 ││  1920  │
//   │        │└─────────────────┘│        │
//   │        │                   │        │
//   └────────┘                   └────────┘
// static int screens[][3] =
//   {{1200, 1920, AT_TOP}, {2560, 1600, AT_TOP}, {1200, 1920, AT_TOP}};
//
//   ┌────────────────┐
//   │                │
//   │      1920      │┌──────────┐
//   │       x        ││   1366   │
//   │      1200      ││    x     │
//   │                ││   768    │
//   └────────────────┘└──────────┘
// static int screens[][3] =
//   {{1920, 1200, AT_TOP}, {1366, 768, AT_BOTTOM}};

static int screens[][3] =
  {{1920, 1200, AT_TOP}, {1366, 768, AT_BOTTOM}};

static int n_screens = sizeof(screens) / sizeof(screens[0]);

static int width, height;

int max(int a, int b) {
  return (a < b) ? b : a;
}

void map_init(Display *dpy) {
  int screen = DefaultScreen(dpy);
  width  = DisplayWidth (dpy, screen);
  height = DisplayHeight(dpy, screen);
  if (debug)
    fprintf(stderr, "n_screens: %d\n", n_screens);
  int w = 0;
  int h = 0;
  for (int i=0; i<n_screens; i++) {
    w += screens[i][0];
    h = max(h, screens[i][1]);
  }
  if (width != w)
    fprintf(stderr, "width: %d vs %d\n", width, w);
  if (height != h)
    fprintf(stderr, "height: %d vs %d\n", height, h);
}

void map(int *x, int *y) {
  int xx = *x;
  int yy = *y;
  if (debug) {
    static int x0=-1, y0=-1;
    if (xx != x0 || yy != y0) {
      x0 = xx;
      y0 = yy;
      fprintf(stderr, "(%d, %d)\n", xx, yy);
    }
  }

  // Figure out which screen we're on
  int s = -1;
  int s_left;			// left offset of current screen
  int s_width;			// width of current screen
  int s_right = 0;		// right offset of current screen
  while (s_right <= xx) {
    s += 1;
    if (s >= n_screens) {
      fprintf(stderr, "warning: past rightmost screen.\n");
      return;			// blow this popsickle stand!
    }
    s_left = s_right;
    s_width = screens[s][0];
    s_right = s_left + s_width;
  }

  if (debug) {
    static int s0 = -1;
    if (s0 != s) {
      s0 = s;
      fprintf(stderr, "screen %d\n", s);
    }
  }

  // Figure out where the top and bottom of this screen are
  int s_height = screens[s][1];
  int s_offset = screens[s][2];
  int s_top;
  int s_bottom;
  if (s_offset < AT_BOTTOM) {
    s_top = s_offset - AT_TOP;
    s_bottom = s_height;
  } else {
    s_bottom = height - (s_offset - AT_BOTTOM);
    s_top = s_bottom - s_height;
  }

  // Wrap off the top onto the bottom
  if (yy == s_top) {
    *y = s_bottom - 2;
    return;
  }
  // Wrap off the bottom onto the top
  if (yy == s_bottom - 1) {
    *y = 1;
    return;
  }
  // Wrap off the left onto the right
  if (s == 0 && xx == 0) {
    *x = width-2;
    return;
  }
  // Wrap off the right onto the left
  if (s == n_screens-1 && xx == s_right-1) {
    *x = 1;
    return;
  }
}
