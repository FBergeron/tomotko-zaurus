#ifndef ZPIXMAP_H
#define ZPIXMAP_H
#include <qpixmap.h>

#define ZPIXMAP( x ) ( small_display_p() ? small_pixmap( ( x ) ) : QPixmap( ( x ) ) )
#define ZSMALLDISPLAY small_display_p()

extern int small_display_cache;
extern int small_display_p();
extern QPixmap small_pixmap( const char* xpm[] );

#endif
