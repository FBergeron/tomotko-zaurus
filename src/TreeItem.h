#ifndef TREE_ITEM_H
#define TREE_ITEM_H

#include <qlistview.h>

class TreeItem : public QCheckListItem {

public:

    TreeItem( QListView* parent, const QString& text, QCheckListItem::Type type );
    TreeItem( QListViewItem* parent, const QString& text, QCheckListItem::Type type );
    ~TreeItem();

    virtual bool isFolder() const;
    virtual void update();

};

#endif

