/*
 * This class contains some code borrowed from 
 * keyboard_shortcuts.h (rev. 1.2) and
 * keyboard_shortcuts.cpp (rev. 1.3) of ZBEDic.
 * Copyright (C) 2004 Rafal Mantiuk <rafm@users.sourceforge.org>
 */
#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H 

#include <iostream.h>
#include <qaction.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qdialog.h>
#include <qfontdatabase.h>
#include <qgrid.h>
#include <qhbox.h>
#include <qheader.h>
#include <qhgroupbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlist.h>
#include <qmessagebox.h>
#include <qobjectlist.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qscrollview.h>
#include <qslider.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qtabwidget.h>
#include <qtooltip.h>
#include <qvbox.h>
#include <qvbuttongroup.h>
#include <qvgroupbox.h>
#include "KeyActionListViewItem.h"
#include "OriginalQuiz.h"
#include "Preferences.h"
#include "SequenceDialog.h"
#include "SequenceListItem.h"
#include "SmartListView.h"
#include "StudyLanguageItem.h"

extern QAction* action[ ACTION_COUNT ];

class PreferencesDialog : public QDialog {

    Q_OBJECT
    
public:

    static const QString fontSizeNameList[];
    static const QString interfaceLanguageList[];
    static const QString interfaceLanguageCodeList[];
    static const QString firstLanguageList[];
    static const QString studyLanguageList[];
    static const QString studyLanguageCodeList[];

    PreferencesDialog( QWidget* parent, Preferences* prefs );
    ~PreferencesDialog(); 

public slots:

protected slots:

    virtual void accept();

protected:

    virtual void resizeEvent( QResizeEvent* evt );

private slots:

    void resetDefaultLabelsFont();
    void resetDefaultFont();
    void addFontOverride( const QString& language );
    void removeFontOverride( const QString& language );
    void clearAccelKey();
    void setAccelKey();
    void resetAccelKey();
    void cancelSetAccelKey();
    void keyActionClicked( QListViewItem* );
    void addSequence();
    void removeSequence();
    void updateUi();

private:

    void init();
    void initFontFamilyValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontSizeValues( QComboBox* comboBox, bool withEmptyEntry = false ) const;
    void initFontOverrides();
    void initSequences();
    void initStudyLanguageValues() const;
    void selectFontFamily( QComboBox* comboBox, const QString& fontFamily );
    void selectFontSize( QComboBox* comboBox, int fontSizeModifier, bool withEmptyEntry = false );
    void selectLanguage( QComboBox* comboBox, const QString& langCode );
    bool isRevealingSequenceSelectionValid() const;
    bool isRevealingSequenceDefined( const QString& seqStr ) const;
    bool isStudyLanguageSelectionValid() const;
    void keyPressEvent( QKeyEvent *evt );

    Preferences*                prefs;
    LanguageUpdateNotifier*     languageUpdateNotifier;

    QVBoxLayout*        mainLayout;
    QTabWidget*         tab;

    QWidget*            languagePage;
    QWidget*            quizPage;
    QWidget*            fontPage;
    QWidget*            interfacePage;

    QVBoxLayout*        languageLayout;

    QVBox*              languagesPanel;

    QWidget*            languagesRightPanel;
    QVBoxLayout*        languagesRightPanelLayout;

    SmartListView*      studyLanguagesListView;

    QVBoxLayout*        quizLayout;

    QWidget*            quizAlgorithmsPanel;
    QHBoxLayout*        quizAlgorithmsPanelLayout;
    QVButtonGroup*      quizAlgorithmOptionsPanel;
    QVGroupBox*         quizLengthOptionsPanel;
    QHGroupBox*         revealingOptionsPanel;

    QVBox*              sequencesViewPanel;
    SmartListView*      sequencesView;
    QHBox*              sequencesViewButtons;
    QWidget*            sequencesViewButtonsFiller;
    QPushButton*        addSequenceButton;
    QPushButton*        removeSequenceButton;

    QVGroupBox*         sequencesLabelBox;
    QLabel*             sequencesLabel;

    QRadioButton*       quizAlgoOriginalRadioButton;
    QRadioButton*       quizAlgoSuperMemo2RadioButton;

    QSlider*            quizLengthSlider;
    QHBox*              quizLengthLabelsPanel;
    QLabel*             quizLengthShortestLabel;
    QLabel*             quizLengthMediumLabel;
    QLabel*             quizLengthLongestLabel;
   
    QVBoxLayout*        fontPageLayout;
    QVBoxLayout*        interfacePageLayout;
    QHBoxLayout*        keyboardAccelButtonPanelLayout;

    QHBox*              labelsFontPanel;
    QLabel*             labelsFontLabel;
    QComboBox*          labelsFontFamilyComboBox;
    QComboBox*          labelsFontSizeComboBox;
    QPushButton*        resetDefaultLabelsFontButton;

    QVBox*              fontsPanel;

    QHBox*              fontPanel;
    QLabel*             fontLabel;
    QComboBox*          fontFamilyComboBox;
    QComboBox*          fontSizeComboBox;
    QPushButton*        resetDefaultFontButton;

    QVGroupBox*         fontOverridesPanel;
    QScrollView*        fontOverridesScrollView;
    QWidget*            fontOverridesBox;
    QVBoxLayout*        fontOverridesBoxLayout;
    QWidget*            fontOverridesBoxFiller;

    QList<QHBox>        fontOverrideBoxes;
    QList<QLabel>       fontOverrideLabels;
    QList<QComboBox>    fontOverrideFamilyComboBoxes;
    QList<QComboBox>    fontOverrideSizeComboBoxes;

    QGrid*              miscInterfaceOptionsPanel;

    QVGroupBox*         digraphPanel;
    QCheckBox*          digraphCheckBox;

    QVGroupBox*         hideQuizButtonPanel;
    QCheckBox*          hideQuizButtonCheckBox;

    QVGroupBox*         showAltTextInTermListPanel;
    QCheckBox*          showAltTextInTermListCheckBox;

    QVGroupBox*         keyboardAccelPanel;

    QListViewItem*      grabAccelKeyFor;
    SmartListView*      keyboardAccelListView;
    bool                ignoreReturn;
    bool                keyboardAccelModified;

    QWidget*            keyboardAccelButtonPanel;
    QWidget*            keyboardAccelButtonPanelFiller;
    QButton*            clearAccelKeyButton;
    QButton*            setAccelKeyButton;
    QButton*            resetAccelKeyButton;

    QWidget*            languagePageFiller;
    QWidget*            languagesRightPanelFiller;
    QWidget*            quizPageFiller;
    QWidget*            fontPageFiller;
    QWidget*            interfacePageFiller;

};

#endif
