#ifndef SMART_LISTVIEW_H
#define SMART_LISTVIEW_H

#include <iostream.h>
#include <qevent.h>
#include <qlistview.h>

class SmartListView : public QListView {

public:

    SmartListView( QWidget* parent = 0, const char* name = 0 );
    virtual ~SmartListView();

    int getStretchColumn() const;
    void setStretchColumn( int col );
    bool areAllColumnsWide() const;
    void setAllColumnsWide( bool isOn );
    void updateColumnsWidth();

protected:

    void resizeEvent( QResizeEvent* event );

private:

    int stretchColumn;
    bool allColumnsWide;

};

#endif
