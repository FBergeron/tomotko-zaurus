#include "SequenceListItem.h"

SequenceListItem::SequenceListItem( QListView* parent, const QString& text, Sequence sequence )
    : QCheckListItem( parent, text, QCheckListItem::CheckBox ), sequence( sequence ) {
}

SequenceListItem::~SequenceListItem() {
}

void SequenceListItem::setOn( bool isOn ) {
    QCheckListItem::setOn( isOn );
    sequence.setEnabled( isOn );
}

Sequence& SequenceListItem::getSequence() {
    return( sequence );
}
