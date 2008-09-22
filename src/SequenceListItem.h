#ifndef SEQUENCE_LIST_ITEM_H
#define SEQUENCE_LIST_ITEM_H

#include <qlistview.h>
#include <qwidget.h>
#include "Sequence.h"

class SequenceListItem : public QCheckListItem {

public:

    SequenceListItem( QListView* parent, const QString& text, Sequence sequence );
    ~SequenceListItem();

    Sequence& getSequence();

    virtual void setOn( bool isOn );

private:

    Sequence sequence;

};

#endif

