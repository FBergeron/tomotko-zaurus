#include "FolderTreeItem.h"
#include "icons/closedFolder.xpm"
#include "icons/openedFolder.xpm"

FolderTreeItem::FolderTreeItem( VocabTreeView* parent, Folder* folder, Preferences& prefs, PropertiesPanel* propsPanel )
    : TreeItem( (QListView*) parent, "FolderTreeItem", CheckBox ), folder( folder ), prefs( prefs ), propsPanel( propsPanel ) {
    setText( 0, folder->getTitle() );
    setOn( folder->isMarkedForStudy() );
    //setOpen( prefs.isFolderOpen( folder->getId() ) );
}

FolderTreeItem::FolderTreeItem( QListViewItem* parent, Folder* folder, Preferences& prefs, PropertiesPanel* propsPanel )
    : TreeItem( parent, "FolderTreeItem", CheckBox ), folder( folder ), prefs( prefs ), propsPanel( propsPanel ) {
    setText( 0, folder->getTitle() );
    setOn( folder->isMarkedForStudy() );
}

FolderTreeItem::~FolderTreeItem() {
}

bool FolderTreeItem::isFolder() const {
    return( true );
}

void FolderTreeItem::setPropertiesPanel( PropertiesPanel* propsPanel ) {
    this->propsPanel = propsPanel;
}

void FolderTreeItem::setOpen( bool isOpened ) {
    // Ignore close event for root.
    if( parent() || isOpened ) {
        QCheckListItem::setOpen( isOpened );
        setPixmap( 0, ( isOpened ? ZPIXMAP( openedFolder_xpm ) : ZPIXMAP( closedFolder_xpm ) ) );
        prefs.setFolderOpen( folder->getUid(), isOpened );
    }
}

void FolderTreeItem::setOn( bool isOn ) {
    // We clear the selection in order to effectively reset it after
    // changing the state of the item.  This is needed because the
    // item is updated when a selection event occurs.
    bool isUpdateHackEnabled = listView()->isUpdatesEnabled();
    if( isUpdateHackEnabled )
        listView()->clearSelection();
    QCheckListItem::setOn( isOn );
    folder->setMarkedForStudy( isOn );
    // Force updating the event.
    if( isUpdateHackEnabled )
        listView()->setSelected( this, true );
}

Folder* FolderTreeItem::getFolder() {
    return( folder );
}

void FolderTreeItem::update() {
    if( propsPanel )
        propsPanel->updateCounters();
}
