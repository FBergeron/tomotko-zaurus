#include "HistoryField.h"

HistoryField::HistoryField( QWidget* parent, const char* name = 0 ) : QComboBox( true, parent, name ), digraphEnabled( false ) {
    setDuplicatesEnabled( false );
    setInsertionPolicy( QComboBox::AtBottom );
    setAutoCompletion( true );
    installEventFilter( this );
}

bool HistoryField::eventFilter( QObject* obj, QEvent* event ) {
    if( event->type() == 6 ) {
        // The following code should be located in keyPressEvent() (like for DigraphLineEdit).
        // However, there seems to have a bug in the implementation of QComboBox where keyPressEvent()
        // is always called twice when we hit a key.  Placing the code here fixes that.
        QKeyEvent* keyEvent = (QKeyEvent*)event;
        if( digraphEnabled ) {
            // We consider only printable keys.  Control keys are processed normally.
            // I'm not sure if this test covers all the cases though.
            if( keyEvent->count() > 0 ) {
                if( keyEvent->key() == Key_Backspace ) {
                    if( lineEdit()->cursorPosition() > 0 ) {
                        if( buffer.isNull() ) {
                            QString charToDelete( lineEdit()->text().mid( lineEdit()->cursorPosition() - 1, 1 ) );
                            buffer = charToDelete;
                        }
                        else 
                            buffer = QString::null;
                    }
                }
                else {
                    if( !buffer.isNull() ) {
                        buffer += keyEvent->text();
                        const QString newChar( Util::getDigraph( buffer ) );
                        if( newChar == QString::null )
                            buffer = QString::null;
                        else {
                            QKeyEvent* digraphEvent = new QKeyEvent( QEvent::KeyPress, 0, 0, 0, newChar, keyEvent->isAutoRepeat(), 0 );
                            qApp->sendEvent( lineEdit(), digraphEvent );
                            buffer = QString::null;
                            return( true );
                        }
                    }
                }
            }
        }
    }
    return( QComboBox::eventFilter( obj, event ) );
}

void HistoryField::setDigraphEnabled( bool isEnabled ) {
    this->digraphEnabled = isEnabled;
}

bool HistoryField::isDigraphEnabled() const {
    return( digraphEnabled );
}

