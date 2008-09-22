#include "HistoryField.h"

HistoryField::HistoryField( QWidget* parent, const char* name = 0 ) : QComboBox( true, parent, name ) {
    setDuplicatesEnabled( false );
    setInsertionPolicy( QComboBox::AtBottom );
    setAutoCompletion( true );
}

void HistoryField::activate() {
    int c = count();
    insertItem( currentText(), c );
    setCurrentItem( c );  
}

void HistoryField::keyPressEvent( QKeyEvent* e ) {
    if( e->key() == Qt::Key_Return )
        emit returnPressed();
    QComboBox::keyPressEvent(e);
}
