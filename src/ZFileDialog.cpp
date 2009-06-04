/****************************************************************************
* ZFileDialog.cpp
* 
* The code of this class is borrowed from zfiledialog.cpp (rev. 1.6) found in 
* ZBEDic project.
* Copyright (C) 2004 Rafal Mantiuk <rafm@users.sourceforge.org>
****************************************************************************/

#include <qlayout.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qvbox.h>
#include <qlistview.h>
#include <qfileinfo.h>
#include <qpixmap.h>
#include <qlineedit.h>

#include <qpe/mimetype.h>
#include <qpe/resource.h>

#include "ZFileDialog.h"

class ZFileItem : public QListViewItem {

public:

    ZFileItem( QListView *parent, const QFileInfo finfo ) :
        QListViewItem( parent ), finfo( finfo ) {
    }

    QString text( int column ) const {
        if( column == 1 ) {
            if( finfo.isDir() )
                return "<dir>";
            else {
                uint fsize = finfo.size();
                if( fsize < 1000 )
                    return QString::number(finfo.size());
                else if( fsize < 1000000 ) {
                    return QString::number(finfo.size()/1000) + " Kb";
                } else
                    return QString::number(finfo.size()/1000000) + " Mb";        
            }
        }
        if( column == 2 ) {
            QDate fdate = finfo.lastModified().date();
            QString dateStr;
            dateStr.sprintf( "%04d/%02d/%02d", fdate.year(), fdate.month(), fdate.day() );
            return dateStr;
        }
        return finfo.fileName();
    }

    virtual const QPixmap* pixmap( int column ) const {
        if( column != 0 ) 
            return NULL;

        if( file_pixmap.isNull() ) {
            if( finfo.isDir() )
                file_pixmap = Resource::loadPixmap( "folder" );
            else {
                const MimeType mime( finfo.absFilePath() );
                if( mime.id().length() == 0 ) 
                    return NULL;
                file_pixmap = mime.pixmap();
            }
        }
        return &file_pixmap;
    }

public:

    const QFileInfo finfo;

protected:
//   QPixmap* getMimePixmap()
//   {
//   }

    mutable QPixmap file_pixmap;

};

ZFileDialog::ZFileDialog( const QString title, const QString &path, Mode mode, QWidget *parent ) :
    QDialog( parent, "zfiledialog", true ), mode( mode ), customFilter( NULL ) {
    setCaption( title );

    //  setMinimumWidth( 200 );
    // -4 since the dialog has a frame
    //  setMinimumHeight( parentWidget()->height()-4 );

    QBoxLayout *l = new QVBoxLayout( this );
    l->setMargin( 2 );

    QVBox* vbox = new QVBox( this );
    vbox->setSpacing(2);
    vbox->setMargin(2);
    l->addWidget( vbox );

    QHBox* hbox;

    if( mode == AnyFile ) {
        QHBox* hbox = new QHBox( vbox );
        hbox->setSpacing(2);
        new QLabel( tr( "File" ), hbox );
        selection = new QLineEdit( hbox );
    }

    hbox = new QHBox( vbox );
    fstabCB = new QComboBox( hbox );
    QPixmap pixmap = Resource::loadPixmap( "folderup" );
    QPushButton *parentBT = new QPushButton( pixmap, QString::null, hbox );
    parentBT->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );
    parentBT->setFixedWidth( pixmap.width() );
    //p  pixmap = Resource::loadPixmap( "newfolder" );
    //   QPushButton *createDirBT = new QPushButton( pixmap, QString::null, hbox );
    //   createDirBT->setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum ) );  
    //   createDirBT->setFixedWidth( pixmap.width() );

    fileLVHBox = new QHBox( vbox );

    fileLV = new QListView( fileLVHBox );
    fileLV->addColumn( tr( "Name" ) );
    fileLV->addColumn( tr( "Size" ) );
    fileLV->addColumn( tr( "Date" ) );
    if( mode == ExistingFiles )
        fileLV->setMultiSelection( true );

    imagePreviewBox = new QVGroupBox( tr( "ImagePreview" ), fileLVHBox );
    imagePreviewBox->hide();
    imagePreview = new QLabel( imagePreviewBox );

    showImagePreview = new QCheckBox( tr( "ShowImagePreview" ), vbox );
    showImagePreview->setChecked( false );
    connect( showImagePreview, SIGNAL( toggled( bool ) ), this, SLOT( toggleImagePreview( bool ) ) );

    showMaximized();
    currentDir.setSorting( QDir::Name | QDir::DirsFirst | QDir::IgnoreCase );

    setDir( path );

    //  adjustSize();

    connect( fstabCB, SIGNAL( activated( int ) ), this, SLOT( dirSelected( int ) ) );
    connect( fileLV, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( fileClicked( QListViewItem* ) ) );
    connect( parentBT, SIGNAL( clicked() ), this, SLOT( parentDirClicked() ) );

    if( mode == AnyFile ) {
        connect( fileLV, SIGNAL( selectionChanged() ), this, SLOT( itemSelected() ) );
    }  
}

void ZFileDialog::insertDirEntry( const QString &label, const QString &path, QPixmap *pixmap ) {
    if( pixmap == NULL ) 
        fstabCB->insertItem( label );
    else
        fstabCB->insertItem( *pixmap, label );

    dir_list.append( path );
}

int ZFileDialog::insertDirTree( const QString &label, QString basePath, const QString &path, bool subCheck, QPixmap *pixmap ) {  
    insertDirEntry( label, basePath, pixmap );
    if( basePath == path ) 
        fstabCB->setCurrentItem( fstabCB->count()-1 );

    if( !subCheck ) 
        return false;

    int level = 1;
    bool finish = false;
    while( !finish && path.left( basePath.length() ) == basePath ) {
        int start_ind = basePath.length();
        while( path[start_ind] == '/' )
          start_ind++;
        int nextDir = path.find( '/', start_ind );
        if( nextDir == -1 ) {
          finish = true;
          nextDir = path.length();
        }
        if( nextDir <= start_ind ) 
            break;

        basePath = path.left( nextDir );

        QString tab;
        for( int l = 0; l < level; l++ )
          tab += "  ";

        insertDirEntry( tab + path.mid( start_ind, nextDir-start_ind ), basePath );
        level++;
    }
    if( finish ) {
        fstabCB->setCurrentItem( fstabCB->count()-1 );
        return true;
    }

    return false;
}

void ZFileDialog::setDir( QString path ) {
    currentDir.setPath( path );
    if( !currentDir.exists() ) {
        path = "/";
        currentDir.setPath( path );
    }

    if( isVisible() )
        refreshFileList();
}

void ZFileDialog::refreshFileList() {
    fileLV->clear();

    QString path = currentDir.path();

    QStringList flist = currentDir.entryList();
    if( customFilter != NULL ) {
        customFilter->filter( flist );
    }

    dir_list.clear();
    fstabCB->clear();

    QPixmap homePixmap = Resource::loadPixmap( "home" );
    QPixmap sdPixmap = Resource::loadPixmap( "SDDeviceS" );
    QPixmap cfPixmap = Resource::loadPixmap( "CFDeviceS" );

    bool selected = false;
    selected |= insertDirTree( "Home", "/home/zaurus", path, !selected, &homePixmap );
    selected |= insertDirTree( "SD Card", "/mnt/card", path, !selected, &sdPixmap );
    selected |= insertDirTree( "CF Card", "/mnt/cf", path, !selected, &cfPixmap );
    selected |= insertDirTree( "Root", "/", path, !selected );


    for( QStringList::Iterator itf = flist.begin(); itf != flist.end(); itf++ ) {
        //    printf( "f: %s\n", (*itf).latin1() );  
        const QFileInfo finfo( currentDir, *itf );
        if( finfo.fileName().left( 1 ) == "." ) continue;
        if( finfo.isFile() && !filters.isEmpty() ) {
            bool matched = false;
            for( QStringList::Iterator it = filters.begin(); it != filters.end(); it++ ) {
                if( finfo.fileName().right( (*it).length() ) == *it ) {
                    matched = true;
                    break;
                }
            }
            if( !matched )
                continue;
        }
        fileLV->insertItem( new ZFileItem( fileLV, finfo ) );
    }
}

void ZFileDialog::show() {
    QDialog::show();
    refreshFileList();
}

void ZFileDialog::fileClicked( QListViewItem *it ) {
    if( it == NULL ) 
        return;
    ZFileItem *zitem = (ZFileItem*)it;
    if( zitem->finfo.isDir() ) {
        setDir( zitem->finfo.absFilePath( ) );
    }
}

void ZFileDialog::dirSelected( int idx ) {
    setDir( dir_list[idx] );
}

void ZFileDialog::parentDirClicked() {
    if( currentDir.isRoot() ) 
        return;
    QString path = currentDir.path();
    int lastDir = path.length()-1;
    if( lastDir <= 0 ) 
        return;
    while( path[lastDir] == '/' )
        lastDir--;
    lastDir = path.findRev( '/', lastDir );
    if( lastDir < 0 ) 
        return;
    setDir( path.left( lastDir ) );
}

void ZFileDialog::setFilters( const QStringList &filters ) {
    this->filters = filters;
}

QStringList ZFileDialog::selectedFiles() const {
    QStringList selected;

    for( QListViewItem *item = fileLV->firstChild(); item != NULL; item = item->nextSibling() ) {
        ZFileItem *fitem = (ZFileItem*)item;
        if( fileLV->isSelected(item) ) {
            selected.append( fitem->finfo.filePath() );
        }
    }
    return selected;
}

QString ZFileDialog::selectedFile() const {
    if( mode == AnyFile ) {
        return currentDir.path() + "/" + selection->text();
    } 
    else {
        ZFileItem *fitem = (ZFileItem*)fileLV->selectedItem();
        if( fitem == NULL ) return QString::null;
            return fitem->finfo.filePath();
    }
}

void ZFileDialog::setSelection( const QString &name ) {
    selection->setText( name );
}

void ZFileDialog::itemSelected( ) {
    QListViewItem *it = fileLV->selectedItem();
    if( it == NULL ) 
        return;
    ZFileItem *zitem = (ZFileItem*)it;
    if( zitem->isSelected() && zitem->finfo.isFile() ) {
        selection->setText( zitem->finfo.fileName() );
    }
}

void ZFileDialog::toggleImagePreview( bool isOn ) {
    if( isOn )
        imagePreviewBox->show();
    else
        imagePreviewBox->hide();
}
