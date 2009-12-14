#ifndef VOCAB_TREEVIEW_H
#define VOCAB_TREEVIEW_H

#include <iostream.h>
#include <qapplication.h>
#include <qlistview.h>
#include <qpaintdevice.h>
#include <qobject.h>
#include <qstring.h>
#include <qpe/resource.h>
#include "Base.h"
#include "Controller.h"
#include "FolderTreeItem.h"
#include "Preferences.h"
#include "SmartListView.h"
#include "Util.h"
#include "VocabTreeItem.h"
#include "Vocabulary.h"

class VocabTreeView : public SmartListView {

public:

    VocabTreeView( Controller& controller, QWidget* parent = 0, const char* name = 0 );
    virtual ~VocabTreeView();

    VocabTreeItem* currentItem() const;

    FolderTreeItem* addFolder( Folder* folder = NULL, QMap<QString,Vocabulary>* vocabularies = NULL );
    FolderTreeItem* addFolder( FolderTreeItem* parentFolderItem, Folder* folder = NULL, QMap<QString,Vocabulary>* vocabularies = NULL );
    VocabTreeItem* addVocab( Vocabulary* vocab = NULL );
    VocabTreeItem* addVocab( FolderTreeItem* parentFolderItem, Vocabulary* vocab = NULL );
    void removeItem();

    VocabTreeItem* getVocabTreeItem( const QUuid& uid );

    IdList getAllChildrenVocab( TreeItem* item ) const;

    Folder* getRootFolder() const;

private:

    VocabTreeItem* getVocabTreeItemRec( TreeItem* item, const QUuid& uid );

    void getAllChildrenVocabRec( TreeItem* item, IdList& vocabList ) const;
    
    bool dirty;

    Controller& controller;

};

#endif
