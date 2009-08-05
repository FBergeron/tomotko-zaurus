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
#include <qhbox.h>
#include <qlistview.h>
#include <qfileinfo.h>
#include <qpixmap.h>
#include <qlabel.h>
#include <qlineedit.h>

#include <qpe/mimetype.h>
#include <qpe/resource.h>

#include "ZFileDialog.h"
#include "icons/void.xpm"

class ZFileItem : public QListViewItem {

public:

    ZFileItem( QListView *parent, const QFileInfo finfo ) :
        QListViewItem( parent ), finfo( finfo ) {
    }

    QString text( int column ) const {
        if( column == 1 ) {
            if( finfo.isDir() )
                return QObject::tr( "<dir>" );
            else {
                uint fsize = finfo.size();
                if( fsize < 1000 )
                    return QString::number(finfo.size());
                else if( fsize < 1000000 ) {
                    return QString::number(finfo.size()/1000) + " " + QObject::tr( "Kb" );
                } else
                    return QString::number(finfo.size()/1000000) + " " + QObject::tr( "Mb" );        
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

ZFileDialog::ZFileDialog( const QString title, const QString &path, Mode mode, bool useImagePreview, QWidget *parent ) :
    QDialog( parent, "zfiledialog", true ), mode( mode ), previewImage( useImagePreview ), customFilter( NULL ),
        imagePreviewEnabled( false ), imagePreviewDirty( false ) {
    setCaption( title );

    //  setMinimumWidth( 200 );
    // -4 since the dialog has a frame
    //  setMinimumHeight( parentWidget()->height()-4 );

    QBoxLayout *l = new QVBoxLayout( this );
    l->setMargin( 2 );

    vbox = new QVBox( this );
    vbox->setSpacing(2);
    vbox->setMargin(2);
    l->addWidget( vbox );

    if( mode == AnyFile ) {
        QHBox* hboxFilename = new QHBox( vbox );
        hboxFilename->setSpacing(2);
        new QLabel( tr( "File" ), hboxFilename );
        selection = new QLineEdit( hboxFilename );
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


    mainPanel = new QWidget( vbox );
    mainPanelLayout = new QBoxLayout( mainPanel, QBoxLayout::LeftToRight );
    mainPanelLayout->setSpacing( 2 );

    fileLVBox = new QVBox( mainPanel );

    fileLV = new QListView( fileLVBox );
    fileLV->addColumn( tr( "Name" ) );
    fileLV->addColumn( tr( "Size" ) );
    fileLV->addColumn( tr( "Date" ) );
    if( mode == ExistingFiles )
        fileLV->setMultiSelection( true );

    showImagePreview = new QCheckBox( tr( "ShowImagePreview" ), fileLVBox );
    if( previewImage )
        showImagePreview->setChecked( true );
    else {
        showImagePreview->setChecked( false );
        showImagePreview->hide();
    }
    connect( showImagePreview, SIGNAL( toggled( bool ) ), this, SLOT( setImagePreviewEnabled( bool ) ) );

    imagePreviewBox = new QHGroupBox( tr( "ImagePreview" ), mainPanel );

    imagePreviewWrapper = new QVBox( imagePreviewBox );
    imagePreview = new QLabel( imagePreviewWrapper );
    imagePreview->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
    imagePreview->setAlignment( AlignHCenter | AlignVCenter );
    imagePreview->setMinimumSize( 260, 260 );

    mainPanelLayout->addWidget( fileLVBox, 1 );
    mainPanelLayout->addWidget( imagePreviewBox );

    imagePreviewBox->hide();

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

    imagePreview->installEventFilter( this );

    setImagePreviewEnabled( previewImage );
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
    selected |= insertDirTree( tr( "Home" ), "/home/zaurus", path, !selected, &homePixmap );
    selected |= insertDirTree( tr( "SD Card" ), "/mnt/card", path, !selected, &sdPixmap );
    selected |= insertDirTree( tr( "CF Card" ), "/mnt/cf", path, !selected, &cfPixmap );
    selected |= insertDirTree( tr( "Root" ), "/", path, !selected );


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
    if( it == NULL ) {
        clearImagePreview();
        imagePreviewPath = QString();
        return;
    }
    ZFileItem *zitem = (ZFileItem*)it;
    if( zitem->finfo.isDir() ) {
        clearImagePreview();
        imagePreviewPath = QString();
        setDir( zitem->finfo.absFilePath( ) );
    }
    else if( zitem->isSelected() && zitem->finfo.isFile() ) {
        if( imagePreviewEnabled ) { 
            initImagePreview( zitem->finfo.absFilePath() );
        }
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

void ZFileDialog::setImagePreviewEnabled( bool isOn ) {
    if( isOn ) {
        imagePreviewEnabled = true;
        imagePreviewBox->show();
        fileClicked( fileLV->selectedItem() );
    }
    else {
        imagePreviewEnabled = false;
        imagePreviewBox->hide();
        showImagePreview->setChecked( false );
    }
}

bool ZFileDialog::isImagePreviewEnabled() const {
    return( imagePreviewEnabled );
}

void ZFileDialog::clearImagePreview() {
    imagePreview->setPixmap( ZPIXMAP( void_xpm ) );
}

void ZFileDialog::resizeImagePreview() {
    if( imagePreviewPath.isNull() || !imagePreviewEnabled )
        return;
    QPixmap pixmap( imagePreviewPath );
    int w = pixmap.width();
    int h = pixmap.height();
    if( w > imagePreviewWrapper->width() ) {
        h = imagePreviewWrapper->width() * h / w;
        w = imagePreviewWrapper->width();
    }
    if( h > imagePreviewWrapper->height() ) {
        w = imagePreviewWrapper->height() * w / h;
        h = imagePreviewWrapper->height();
    }
    QPixmap scaledPixmap;
    scaledPixmap.convertFromImage( pixmap.convertToImage().smoothScale( w, h ) ); 
    imagePreview->setPixmap( scaledPixmap );
}

void ZFileDialog::initImagePreview( const QString& imagePath ) {
    clearImagePreview();
    if( !imagePath.isNull() ) {
        QFileInfo info( imagePath );
        if( info.exists() ) {
            QString extension = info.extension( false ).upper();
            if( extension == "GIF" || extension == "PNG" ) {
                QString imageFormat = QPixmap::imageFormat( imagePath );
                if( imageFormat == "GIF" || imageFormat == "PNG" ) {
                    if( imageFormat == "GIF" ) {
                        const QMovie& movie( imagePath );
                        imagePreview->setMovie( movie );
                    }
                    else if( imageFormat == "PNG" ) {
                        imagePreviewPath = imagePath;
                        imagePreview->setText( " " ); // To make sure that a resize event will be spawned when calling adjustSize() below. 
                        imagePreview->adjustSize(); // This will generate a Resize event that will trigger resizeImagePreview() in eventFilter().
                        imagePreviewDirty = true;

                    }

                }
            }
        }
    }
}

bool ZFileDialog::eventFilter( QObject* target, QEvent* evt ) {
    // Small hack to manage imagePreviewResize properly.
    if( target == imagePreview ) {
        if( evt->type() == QEvent::Resize && imagePreviewDirty ) {
            resizeImagePreview();
            imagePreviewDirty = false;
        }
    }
   return( QDialog::eventFilter( target, evt ) ); 
}

void ZFileDialog::updateGeometry() {
    QSize size( width(), height() );
    if( size.width() > size.height() ) {
        mainPanelLayout->setDirection( QBoxLayout::LeftToRight );
    }
    else {
        mainPanelLayout->setDirection( QBoxLayout::TopToBottom );
    }

    vbox->layout()->invalidate();
    layout()->invalidate();
    layout()->activate();
}

void ZFileDialog::resizeEvent( QResizeEvent* ) {
    updateGeometry();
}
