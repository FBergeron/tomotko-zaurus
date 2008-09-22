#ifndef VOCAB_TREE_ITEM_H
#define VOCAB_TREE_ITEM_H

#include <iostream.h>
#include <qlistview.h>
#include <qpe/resource.h>
#include <qwidget.h>
#include "Base.h"
#include "PixMap.h"
#include "PropertiesPanel.h"
#include "TreeItem.h"
#include "Vocabulary.h"

class VocabTreeView;

class VocabTreeItem : public TreeItem, public Base {

public:

    VocabTreeItem( QListViewItem* parent, Vocabulary* vocab, PropertiesPanel* propsPanel = NULL );
    ~VocabTreeItem();

    const char* className() const { return "VocabTreeItem"; }

    void setPropertiesPanel( PropertiesPanel* propsPanel );
    void setOpen( bool isOpened );
    virtual void setOn( bool isOn );
    virtual void setSelected( bool isSelected );

    Vocabulary* getVocabulary();

    virtual void update();

private:

    Vocabulary*         vocab;
    PropertiesPanel*    propsPanel;

};

#endif
