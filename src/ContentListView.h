#ifndef CONTENT_LISTVIEW_H
#define CONTENT_LISTVIEW_H

#include <iostream.h>
#include <qheader.h>
#include "SmartListView.h"

class ContentListView : public SmartListView {

public:

    ContentListView( QWidget* parent = 0, const char* name = 0 );
    virtual ~ContentListView();

    QSize sizeHint() const;

};

#endif
