#include "VocabTreeItem.h"
#include "icons/closedVocab.xpm"
#include "icons/openedVocab.xpm"

VocabTreeItem::VocabTreeItem( QListViewItem* parent, Vocabulary* vocab, PropertiesPanel* propsPanel )
    : TreeItem( parent, "VocabTreeItem", CheckBox ), vocab( vocab ), propsPanel( propsPanel ) {
    setText( 0, vocab->getTitle() );
    setOn( vocab->isMarkedForStudy() );
}

VocabTreeItem::~VocabTreeItem() {
}

void VocabTreeItem::setPropertiesPanel( PropertiesPanel* propsPanel ) {
    this->propsPanel = propsPanel;
}

void VocabTreeItem::setOpen( bool isOpened ) {
    // Ignore close event when selected.
    if( !isSelected() || isOpened ) {
        QCheckListItem::setOpen( isOpened );
        setPixmap( 0, ( isOpened ? ZPIXMAP( openedVocab_xpm ) : ZPIXMAP( closedVocab_xpm ) ) );
    }
}

void VocabTreeItem::setOn( bool isOn ) {
    // We clear the selection in order to effectively reset it after
    // changing the state of the item.  This is needed because the
    // item is updated when a selection event occurs.
    bool isUpdateHackEnabled = listView()->isUpdatesEnabled();
    if( isUpdateHackEnabled )
        listView()->clearSelection();
    QCheckListItem::setOn( isOn );
    vocab->setMarkedForStudy( isOn );
    // Force updating the event.
    if( isUpdateHackEnabled )
        listView()->setSelected( this, true );
}

void VocabTreeItem::setSelected( bool isSelected ) {
    QCheckListItem::setSelected( isSelected );
    if( !isSelected )
        setOpen( FALSE );
}

Vocabulary* VocabTreeItem::getVocabulary() {
    return( vocab );
}

void VocabTreeItem::update() {
    if( propsPanel )
        propsPanel->updateCounters();
}
