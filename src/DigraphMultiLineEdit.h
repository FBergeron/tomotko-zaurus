#ifndef DIGRAPH_MULTI_LINE_EDIT_H
#define DIGRAPH_MULTI_LINE_EDIT_H

#include <iostream.h>
#include <qevent.h>
#include <qmultilineedit.h>
#include "Util.h"

class DigraphMultiLineEdit : public QMultiLineEdit {

    Q_OBJECT

public:

    DigraphMultiLineEdit( QWidget* parent, const char* name = 0 );
    ~DigraphMultiLineEdit();

    bool isDigraphEnabled() const;

public slots:

    void setDigraphEnabled( bool isEnabled );

protected:

    virtual void keyPressEvent( QKeyEvent* event );

private:

    bool digraphEnabled;
    QString buffer;

};

#endif
