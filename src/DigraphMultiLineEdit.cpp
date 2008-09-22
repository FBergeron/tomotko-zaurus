#include "DigraphMultiLineEdit.h"

DigraphMultiLineEdit::DigraphMultiLineEdit( QWidget* parent, const char* name = 0 )
    : QMultiLineEdit( parent, name ), digraphEnabled( false ) {
}

DigraphMultiLineEdit::~DigraphMultiLineEdit() {
}

void DigraphMultiLineEdit::keyPressEvent( QKeyEvent* event ) {
    //cout << "txt='" << event->text() << "' asc=" << event->ascii() << "; k=" << event->key() << " count=" << event->count() << endl;
    if( digraphEnabled ) {
        // We consider only printable keys.  Control keys are processed normally.
        // I'm not sure if this test covers all the cases though.
        if( event->count() > 0 ) {
            if( event->key() == Key_Backspace ) {
                int line;
                int col;
                getCursorPosition( &line, &col );
                if( line + col > 0 ) {
                    if( buffer.isNull() ) {
                        QString charToDelete( textLine( line ).mid( col - 1, 1 ) );
                        buffer = charToDelete;
                    }
                    else 
                        buffer = QString::null;
                }
            }
            else {
                if( !buffer.isNull() ) {
                    buffer += event->text();
                    const QString newChar( Util::getDigraph( buffer ) );
                    if( newChar == QString::null )
                        buffer = QString::null;
                    else {
                        QKeyEvent* digraphEvent = new QKeyEvent( QEvent::KeyPress, 0, 0, 0, newChar, event->isAutoRepeat(), 0 );
                        QMultiLineEdit::keyPressEvent( digraphEvent );
                        return;
                    }
                }
            }
        }
    }
    QMultiLineEdit::keyPressEvent( event );
}

void DigraphMultiLineEdit::setDigraphEnabled( bool isEnabled ) {
    this->digraphEnabled = isEnabled;
}

bool DigraphMultiLineEdit::isDigraphEnabled() const {
    return( digraphEnabled );
}
