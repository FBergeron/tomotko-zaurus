#ifndef VOCAB_MANAGER_H
#define VOCAB_MANAGER_H

#include <iostream.h>
#include <qclipboard.h>
#include <qpe/qcopenvelope_qws.h>
#include <qpe/qpeapplication.h>
#include <qdialog.h>
#include <qhbox.h>
#include <qheader.h>
#include <qinputdialog.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qmime.h>
#include <qmultilineedit.h>
#include <qpe/fileselector.h>
#include <qpushbutton.h>
#include <qtabwidget.h>
#include <qtextstream.h>
#include <qtooltip.h>
#include <qurloperator.h>
#include <qvaluelist.h>
#include <qvbox.h>
#include <qwidget.h>
#include <qwidgetstack.h>
#include <qpe/resource.h>
#include "BilingualKey.h"
#include "Controller.h"
#include "DigraphLineEdit.h"
#include "DigraphMultiLineEdit.h"
#include "PropertiesPanel.h"
#include "SearchDialog.h"
#include "SmartListView.h"
#include "TranslationSelectionDialog.h"
#include "TermDialog.h"
#include "TermListItem.h"
#include "Util.h"
#include "VocabTreeView.h"
#include "VocabTreeItem.h"
#include "ZFileDialog.h"

extern QAction* action[ ACTION_COUNT ];

class VocabularyManagerFrame : public QWidget {

    static const int panelFolderIndex   = 0;
    static const int panelVocabIndex    = 1;

    Q_OBJECT

public:

    VocabularyManagerFrame( Controller* controller, QWidget* parent = 0, const char* name = 0 );
    ~VocabularyManagerFrame();

    void updateShownItems();

    void loadData();
    bool saveData();

    bool isExportAllowed() const;
    bool isImportAllowed() const;

    bool isDigraphEnabled() const;

public slots:

    void copy();
    void cut();
    void paste();

    void search();

    void importData();
    void exportData();
    void exportVocabulary( Vocabulary* vocab );
    void exportFolder( Folder* folder );

    void updateUi();
    void updateFonts();
    void setDigraphEnabled( bool isEnabled );
    void retranslateUi();

    bool areDetailsMaximized() const;
    void toggleMaximizeDetails( bool isOn );
    void updateGeometry();

signals:

    void selectionChanged( QListViewItem* item );

protected:

    virtual void resizeEvent( QResizeEvent* event );
    void showEvent( QShowEvent* showEvt );

private slots:

    void updateTree();
    void updateCurrentFolder( FolderTreeItem* folderItem );
    void updateCurrentVocab( VocabTreeItem* vocabItem );
    void updateTermList();

    void keepSelection();
    void restoreTreeSelection();
    void restoreVocabSelection();

    FolderTreeItem* addFolder();
    VocabTreeItem* addVocab();
    void removeItem();
    void checkAllTerms();
    void inverseCheckedTerms();
    void addTerm();
    void editTerm();
    void removeTerms();
    void showTerm( const TermKey& termKey );

    void updateCurrentFolderTitle( const QString& title );

    void updateCurrentVocabTitle( const QString& title );

private:

    FolderTreeItem* addFolder( Folder* folder, QMap<int,Vocabulary>* vocabularies = NULL );
    VocabTreeItem* addVocab( Vocabulary* vocab );

    FolderTreeItem* buildTreeRec( VocabTreeView* vocabTreeView, FolderTreeItem* parentItem, Folder* folder, bool closeFolder = false );
    VocabTreeItem* buildTreeRec( FolderTreeItem* parentItem, Vocabulary* vocab );
    void doRemoveTerms( bool allowSelectTrans = true, bool confirmBeforeRemove = true );
    void doRemoveItem( bool confirmBeforeRemove = true );

    void copyTerms() const;
    void pasteTerms();
    void pasteVocabulary();
    void pasteFolder();

    void addListeners();
    void removeListeners();

    void maximizeDetailsPanel();
    void restoreDetailsPanel();

    uint getSelectedTermCount() const;

    int                     currentFolderId;
    int                     currentVocabId;
    int                     currentTermId;
    QValueList<int>         selectedTermIdList;

    QVBox*                  treePanel;
    QHBox*                  treeButtonPanel;
    QPushButton*            addFolderButton;
    QPushButton*            addVocabButton;
    QPushButton*            removeItemButton;
    VocabTreeView*          vocabTreeView;
    FolderTreeItem*         vocabTreeRoot;

    QWidgetStack*           detailsPanel;
    QVBox*                  folderDetailsPanel;
    QLabel*                 folderDetailsTitle;
    QHBox*                  folderDetailsFolderHeaderPanel;
    QHBox*                  folderDetailsFolderTitlePanel;
    QLabel*                 folderDetailsFolderTitleLabel;
    DigraphLineEdit*        folderDetailsFolderTitleLineEdit;
    QPushButton*            folderDetailsFolderMaximizeDetailsButton;
    PropertiesPanel*        folderDetailsPropsPanel; 

    QVBox*                  vocabDetailsPanel;
    QHBox*                  vocabDetailsVocabHeaderPanel;
    QLabel*                 vocabDetailsTitle;
    QLabel*                 vocabDetailsVocabTitleLabel;
    DigraphLineEdit*        vocabDetailsVocabTitleLineEdit;
    QPushButton*            vocabDetailsVocabMaximizeDetailsButton;
    QTabWidget*             vocabDetailsTabWidget;
    QVBox*                  vocabDetailsTermsPanel;
    PropertiesPanel*        vocabDetailsPropsPanel;
    QHBox*                  vocabDetailsVocabTitlePanel;
    SmartListView*          termList;
   
    QWidget*                termControlPanel;
    QHBoxLayout*            termControlPanelLayout;

    QHBox*                  checkControlPanel;
    QPushButton*            checkAllTermsButton;
    QPushButton*            inverseCheckedTermsButton;

    QHBox*                  addRemoveTermPanel;
    QPushButton*            addTermButton;
    QPushButton*            editTermButton;
    QPushButton*            removeTermButton;

    SearchDialog*           searchDialog;

    Controller*             controller;

};

#endif
