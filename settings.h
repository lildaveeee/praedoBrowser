#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QColor>
#include <QVector>
#include <QPair>

class QLineEdit;
class QPushButton;
class QLabel;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    QString defaultSearchEngine() const;
    QString backgroundColor()    const;
    QString toggleTopBarShortcut() const;
    QString toggleCentralSearchShortcut() const;
signals:
    void settingsUpdated();

private slots:
    void onOkClicked();
    void onCancelClicked();
    void onRandomizeClicked();

private:
    // Search‐engine
    QLineEdit *m_searchEngineEdit;

    // Theme color buttons
    QPushButton *m_backgroundColorButton;
    QPushButton *m_tabListColorButton;
    QPushButton *m_topBarColorButton;
    QPushButton *m_accent1ColorButton;
    QPushButton *m_accent2ColorButton;
    QPushButton *m_fontColorButton;
    QPushButton *m_altfontColorButton;
    QPushButton *m_buttonColorButton;
    QPushButton *m_linkColorButton;
    QPushButton *m_highlightColorButton;
    QPushButton *m_alternateBaseColorButton;
    QPushButton *m_randomizePaletteButton;

    // Core shortcuts
    QLineEdit *m_newTabShortcutEdit;
    QLineEdit *m_searchbarShortcutEdit;
    QLineEdit *m_closeTabShortcutEdit;
    QLineEdit *m_cycleTabShortcutEdit;
    QLineEdit *m_toggleTopBarShortcutEdit;
    QLineEdit *m_toggleCentralSearchShortcutEdit;
    // Dynamic custom URL⇔shortcut pairs
    QVector<QPair<QLineEdit*, QLineEdit*>> m_customShortcutsEdits;
    QPushButton *m_addShortcutButton;

    // Color values
    QColor m_backgroundColor;
    QColor m_tabListColor;
    QColor m_topBarColor;
    QColor m_accent1Color;
    QColor m_accent2Color;
    QColor m_fontColor;
    QColor m_altfontColor;
    QColor m_buttonColor;
    QColor m_linkColor;
    QColor m_highlightColor;
    QColor m_alternateBaseColor;

    // Dialog action buttons
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
};

#endif // SETTINGSDIALOG_H
