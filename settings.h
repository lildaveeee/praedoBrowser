#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = nullptr);
    ~SettingsDialog();

    // Returns the user-set default search engine.
    QString defaultSearchEngine() const;

private slots:
    void onOkClicked();
    void onCancelClicked();

private:
    QLineEdit *m_searchEngineEdit;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
};

#endif // SETTINGS_H
