#ifndef SEARCH_DIALOG_H
#define SEARCH_DIALOG_H

#include <iostream.h>
#include <qdialog.h>
#include <qfontmetrics.h>
#include <qheader.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qvbox.h>
#include "Controller.h"
#include "HistoryField.h"
#include "ResultListItem.h"
#include "SmartListView.h"
#include "TermDialog.h"
#include "TranslationSelectionDialog.h"

class SearchDialog : public QDialog {

    Q_OBJECT
    
public:

    SearchDialog( QWidget* parent, Controller* controller );
    ~SearchDialog();

public slots:

    void show();
    void updateFonts();
    void retranslateUi();

signals:

    void showTermRequested( const TermKey& termKey );
    void termsRemoved();

private slots:

    void search();
    void reset();
    void goResultVocab();
    void editResultTerm();
    void removeResultTerms();
    void updateUi();
    
private:

    void init();
    uint getSelectedTermCount() const;
    void doRemoveTerms( bool allowSelectTrans = true, bool confirmBeforeRemove = true );

    Controller*             controller;

    QVBoxLayout*            mainLayout;

    QHBox*                  queryPanel;

    HistoryField*           queryField;
    QPushButton*            searchButton;
    QPushButton*            resetButton;

    QHBox*                  resultsHeaderPanel;
    
    QLabel*                 resultsHeaderLabel;
    QWidget*                resultsHeaderFiller;
    QLabel*                 resultsCounterLabel;

    SmartListView*          resultsListView;

    QHBox*                  resultsButtonsPanel;
    QPushButton*            goResultVocabButton;
    QPushButton*            editResultTermButton;
    QPushButton*            removeResultTermButton;

};

#endif
