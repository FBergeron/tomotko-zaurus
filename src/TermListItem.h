#ifndef TERM_LIST_ITEM_H
#define TERM_LIST_ITEM_H

#include <iostream.h>
#include <qlistview.h>
#include <qmap.h>
#include <qobject.h>
#include <qpe/resource.h>
#include <qwidget.h>
#include "Term.h"

class TermListItem : public QCheckListItem {

public:

    TermListItem( QListView* parent, Term* term, const QString& firstLanguage, const QString& testLanguage, bool isAltShown = true );
    ~TermListItem();

    Term* getTerm();
    void updateUi();
    virtual void setOn( bool isOn );
    void setFont( int column, const QFont& font ); 
    QFont font( int column ) const;
    virtual void paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align );

private:

    Term*               term;
    QString             firstLanguage;
    QString             testLanguage;
    bool                altShown;

    QMap<int,QFont>     fonts;

};

#endif
