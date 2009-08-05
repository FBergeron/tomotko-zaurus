#include "VocabTreeView.h"

VocabTreeView::VocabTreeView( Controller& controller, QWidget *parent = 0, const char* name = 0 )
    : SmartListView( parent, name ), dirty( false ), controller( controller ) {
}

VocabTreeView::~VocabTreeView() {
}

VocabTreeItem* VocabTreeView::currentItem() const {
    return( (VocabTreeItem*)QListView::currentItem() );
}

FolderTreeItem* VocabTreeView::addFolder( Folder* folder = NULL, QMap<QString,Vocabulary>* vocabularies = NULL ) {
    FolderTreeItem* newFolderItem = NULL;
    if( folder )
        setUpdatesEnabled( false );
    TreeItem* selectedItem = (TreeItem*)currentItem();
    if( selectedItem && selectedItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
        newFolderItem = addFolder( folderItem, folder, vocabularies );
    }
    if( folder )
        setUpdatesEnabled( true );
    return( newFolderItem );
}

FolderTreeItem* VocabTreeView::addFolder( FolderTreeItem* parentFolderItem, Folder* folder = NULL, QMap<QString,Vocabulary>* vocabularies = NULL ) {
    Folder* parentFolder = parentFolderItem->getFolder();
    Folder* newFolder = controller.addFolder( parentFolder, folder );    
    if( !newFolder )
        return( NULL );
    FolderTreeItem* newFolderItem = new FolderTreeItem( parentFolderItem, newFolder, controller.getPreferences() );
    newFolderItem->setOpen( false );
    newFolderItem->setOn( !folder );
    parentFolderItem->setOpen( !folder );
    if( folder ) {
        for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) {
            if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
                Folder* childFolder = (Folder*)folderChild;
                addFolder( newFolderItem, childFolder, vocabularies );
            }
            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
                Vocabulary* childVocab = (Vocabulary*)folderChild;
                // childVocab contains only a reference to a vocabulary.  The actual data is found in the map.
                if( vocabularies && vocabularies->contains( childVocab->getUid().toString() ) ) {
                    Vocabulary& actualVocab = (*vocabularies)[ childVocab->getUid().toString() ];
                    addVocab( newFolderItem, &actualVocab );
                }
            }
        }
    }
    return( newFolderItem );
}

//FolderTreeItem* VocabTreeView::addFolder( Folder* folder = NULL, QMap<int,Vocabulary>* vocabularies = NULL ) {
//    FolderTreeItem* newFolderItem = NULL;
//    if( folder )
//        setUpdatesEnabled( false );
//    TreeItem* selectedItem = (TreeItem*)currentItem();
//    if( selectedItem && selectedItem->isFolder() ) {
//        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
//        newFolderItem = addFolder( folderItem, folder, vocabularies );
//    }
//    if( folder )
//        setUpdatesEnabled( true );
//    return( newFolderItem );
//}
//
//FolderTreeItem* VocabTreeView::addFolder( FolderTreeItem* parentFolderItem, Folder* folder = NULL, QMap<int,Vocabulary>* vocabularies = NULL ) {
//    Folder* parentFolder = parentFolderItem->getFolder();
//    Folder* newFolder = controller.addFolder( parentFolder, folder );    
//    if( !newFolder )
//        return( NULL );
//    FolderTreeItem* newFolderItem = new FolderTreeItem( parentFolderItem, newFolder, controller.getPreferences() );
//    newFolderItem->setOpen( false );
//    newFolderItem->setOn( !folder );
//    parentFolderItem->setOpen( !folder );
//    if( folder ) {
//        for( Base* folderChild = folder->first(); folderChild; folderChild = folder->next() ) {
//            if( strcmp( folderChild->className(), "Folder" ) == 0 ) {
//                Folder* childFolder = (Folder*)folderChild;
//                addFolder( newFolderItem, childFolder, vocabularies );
//            }
//            else if( strcmp( folderChild->className(), "Vocabulary" ) == 0 ) {
//                Vocabulary* childVocab = (Vocabulary*)folderChild;
//                // childVocab contains only a reference to a vocabulary.  The actual data is found in the map.
//                if( vocabularies && vocabularies->contains( childVocab->getId() ) ) {
//                    Vocabulary& actualVocab = (*vocabularies)[ childVocab->getId() ];
//                    addVocab( newFolderItem, &actualVocab );
//                }
//            }
//        }
//    }
//    return( newFolderItem );
//}

VocabTreeItem* VocabTreeView::addVocab( Vocabulary* vocab = NULL ) {
    VocabTreeItem* newVocabItem = NULL;
    if( vocab )
        setUpdatesEnabled( false );
    TreeItem* selectedItem = (TreeItem*)currentItem();
    if( selectedItem && selectedItem->isFolder() ) {
        FolderTreeItem* folderItem = (FolderTreeItem*)selectedItem;
        newVocabItem = addVocab( folderItem, vocab );
    }
    if( vocab )
        setUpdatesEnabled( true );
    return( newVocabItem );
}

VocabTreeItem* VocabTreeView::addVocab( FolderTreeItem* parentFolderItem, Vocabulary* vocab = NULL ) {
    Folder* parentFolder = parentFolderItem->getFolder();
    Vocabulary* newVocab = controller.addVocabulary( parentFolder, vocab );
    if( !newVocab )
        return( NULL );

    VocabTreeItem* newVocabItem = new VocabTreeItem( parentFolderItem, newVocab );
    newVocabItem->setOpen( false );
    newVocabItem->setOn( !vocab );
    parentFolderItem->setOpen( true );
    return( newVocabItem );
}

void VocabTreeView::removeItem() {
    if( currentItem() ) {
        QListViewItem* itemToSelect = currentItem()->itemAbove();
        delete( currentItem() );
        setSelected( itemToSelect, true );
    }
}

VocabTreeItem* VocabTreeView::getVocabTreeItem( const QUuid& uid ) {
    return( getVocabTreeItemRec( (TreeItem*)firstChild(), uid ) );
}

//VocabTreeItem* VocabTreeView::getVocabTreeItem( int id ) {
//    return( getVocabTreeItemRec( (TreeItem*)firstChild(), id ) );
//}

//Vocabulary* VocabTreeView::getVocabulary( int id ) {
//    VocabTreeItem* vocabTreeItem = getVocabTreeItemRec( (TreeItem*)firstChild(), id );
//    return( vocabTreeItem ? vocabTreeItem->getVocabulary() : NULL );
//}

VocabTreeItem* VocabTreeView::getVocabTreeItemRec( TreeItem* item, const QUuid& uid ) {
    if( item ) {
        if( item->isFolder() ) {
            TreeItem* child = (TreeItem*)item->firstChild();
            while( child ) {
                VocabTreeItem* vocabTreeItem = getVocabTreeItemRec( child, uid );
                if( vocabTreeItem )
                    return( vocabTreeItem );
                child = (TreeItem*)child->nextSibling();
            }
        }
        else {
            VocabTreeItem* vocabTreeItem = (VocabTreeItem*)item;
            Vocabulary* vocab = vocabTreeItem->getVocabulary();
            return( vocab->getUid() == uid ? vocabTreeItem : NULL );
        }
    }
    return( NULL );
}

//VocabTreeItem* VocabTreeView::getVocabTreeItemRec( TreeItem* item, int id ) {
//    if( item ) {
//        if( item->isFolder() ) {
//            TreeItem* child = (TreeItem*)item->firstChild();
//            while( child ) {
//                VocabTreeItem* vocabTreeItem = getVocabTreeItemRec( child, id );
//                if( vocabTreeItem )
//                    return( vocabTreeItem );
//                child = (TreeItem*)child->nextSibling();
//            }
//        }
//        else {
//            VocabTreeItem* vocabTreeItem = (VocabTreeItem*)item;
//            Vocabulary* vocab = vocabTreeItem->getVocabulary();
//            return( vocab->getId() == id ? vocabTreeItem : NULL );
//        }
//    }
//    return( NULL );
//}

IdList VocabTreeView::getAllChildrenVocab( TreeItem* item ) const {
    IdList vocabList;
    getAllChildrenVocabRec( item, vocabList );
    return( vocabList );
}

void VocabTreeView::getAllChildrenVocabRec( TreeItem* item, IdList& vocabList ) const {
    if( item->isFolder() ) {
        TreeItem* child = (TreeItem*)item->firstChild();
        while( child ) {
            getAllChildrenVocabRec( child, vocabList );
            child = (TreeItem*)child->nextSibling();
        }
    }
    else {
        VocabTreeItem* vocabItem = (VocabTreeItem*)item;
        Vocabulary* vocab = vocabItem->getVocabulary();
        vocabList.append( vocab->getId() );
    }
}

Folder* VocabTreeView::getRootFolder() const {
    FolderTreeItem* rootFolderItem = (FolderTreeItem*)firstChild();
    return( rootFolderItem->getFolder() );
}
