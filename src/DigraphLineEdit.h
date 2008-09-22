#ifndef DIGRAPH_LINE_EDIT_H
#define DIGRAPH_LINE_EDIT_H

#include <iostream.h>
#include <qevent.h>
#include <qlineedit.h>
#include "Util.h"

class DigraphLineEdit : public QLineEdit {

    Q_OBJECT

public:

    DigraphLineEdit( QWidget* parent, const char* name = 0 );
    DigraphLineEdit( const QString& content, QWidget* parent, const char* name = 0 );
    ~DigraphLineEdit();

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
