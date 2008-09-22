#ifndef SCROLLABLE_MULTILINEEDIT_H
#define SCROLLABLE_MULTILINEEDIT_H

#include <iostream.h>
#include <qmultilineedit.h>

class ScrollableMultiLineEdit : public QMultiLineEdit {
   
   Q_OBJECT

public:

    ScrollableMultiLineEdit( QWidget *parent, const char *name=0 );
   ~ScrollableMultiLineEdit();

public slots:


    void scrollUp();
    void scrollDown();
    void scrollPageUp();
    void scrollPageDown();

};

#endif
