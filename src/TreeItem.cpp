#include "TreeItem.h"

TreeItem::TreeItem( QListView* parent, const QString& text, QCheckListItem::Type type )
    : QCheckListItem( parent, text, type ) {
}

TreeItem::TreeItem( QListViewItem* parent, const QString& text, QCheckListItem::Type type )
    : QCheckListItem( parent, text, type ) {
}

TreeItem::~TreeItem() {
}

bool TreeItem::isFolder() const {
    return( false );
}

void TreeItem::update() {
}
