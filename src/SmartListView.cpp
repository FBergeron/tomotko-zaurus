#include "SmartListView.h"

SmartListView::SmartListView( QWidget *parent = 0, const char* name = 0 )
    : QListView( parent, name ), stretchColumn( -1 ), allColumnsWide( false ) {
}

SmartListView::~SmartListView() {
}

void SmartListView::setStretchColumn( int col ) {
    stretchColumn = col;
}

int SmartListView::getStretchColumn() const {
    return( stretchColumn );
}

void SmartListView::setAllColumnsWide( bool isOn ) {
    allColumnsWide = isOn;
}

bool SmartListView::areAllColumnsWide() const {
    return( allColumnsWide );
}

void SmartListView::updateColumnsWidth() {
    //cerr << "update w=" << width() << " vp.w=" << viewport()->width();
    //cerr << " vw=" << visibleWidth();
    //cerr << " cw=" << contentsWidth();
    //cerr << endl;
    //resizeContents( width() + 20, height() );
    viewport()->adjustSize();
    if( stretchColumn != -1 ) {
        int otherColumnsWidth = 0;
        for( int i = 0; i < columns(); i++ ) {
            if( i != stretchColumn )
                otherColumnsWidth += columnWidth( i );
        }
        setColumnWidth( stretchColumn, viewport()->width() - otherColumnsWidth );
    }
    if( allColumnsWide ) {
        int columnWidth = viewport()->width() / columns();
        for( int i = 0; i < columns(); i++ )
            setColumnWidth( i, columnWidth );
    }
}

void SmartListView::resizeEvent( QResizeEvent* event ) {
    QListView::resizeEvent( event );
    updateColumnsWidth();
}
