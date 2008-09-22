#ifndef RESULT_LIST_ITEM_H
#define RESULT_LIST_ITEM_H

#include <iostream.h>
#include <qfontmetrics.h>
#include <qlistview.h>
#include <qmap.h>
#include <qobject.h>
#include <qpe/resource.h>
#include <qwidget.h>
#include "Term.h"

class ResultListItem : public QListViewItem {

public:

    ResultListItem( QListView* parent, Term* term, const QString& firstLanguage, const QString& testLanguage, const QString& vocabTitle, const QString& location, bool isAltShown = true );
    ~ResultListItem();

    Term* getTerm();
    void setup();
    void updateUi();
    void setFont( int column, const QFont& font ); 
    QFont font( int column ) const;
    virtual void paintCell( QPainter * p, const QColorGroup & cg, int column, int width, int align );

private:

    Term*               term;
    QString             firstLanguage;
    QString             testLanguage;
    QString             vocabTitle;
    QString             location;
    bool                altShown;

    QMap<int,QFont>     fonts;

};

#endif


