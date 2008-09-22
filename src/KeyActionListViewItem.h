/*
 * This class contains code borrowed from 
 * keyboard_shortcuts.h (rev. 1.2) and
 * keyboard_shortcuts.cpp (rev. 1.3) of ZBEDic.
 * Copyright (C) 2004 Rafal Mantiuk <rafm@users.sourceforge.org>
 */
#ifndef KEY_ACTION_LIST_VIEW_ITEM
#define KEY_ACTION_LIST_VIEW_ITEM

#include <qaction.h>
#include <qlistview.h>
#include "Util.h"

class KeyActionListViewItem : public QListViewItem {

public:
    KeyActionListViewItem( QListView *parent, QAction *action, Action actionIndex ) : QListViewItem( parent ), action( action ), actionIndex( actionIndex ) {
        keyCode = action->accel();
        updateText();
    }

    QAction *getAction() {
        return action;
    }

    Action getActionIndex() {
        return( actionIndex );
    }

    void setKey( int keyCode ) {
        this->keyCode = keyCode;
        updateText();
    }

    int getKey() {
        return keyCode;
    }

    void updateText() {
        setText( 1, keyCode ? Util::describeKey( keyCode ) : QString::null );
    }  

private:

    QAction*    action;
    int         keyCode;
    Action      actionIndex;

};

#endif
