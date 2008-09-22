#include "ContentListView.h"

ContentListView::ContentListView( QWidget *parent = 0, const char* name = 0 )
    : SmartListView( parent, name ) {
    setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Minimum ) );
    setHScrollBarMode( QScrollView::AlwaysOff );
    setVScrollBarMode( QScrollView::AlwaysOff );
}

ContentListView::~ContentListView() {
}

QSize ContentListView::sizeHint() const {
    int height = header()->height() + 8;
    if( childCount() > 0 )
        height += firstChild()->height() * childCount();
    return( QSize( parentWidget()->width(), height ) );
}
