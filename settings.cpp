#include "settings.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QHBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");

    // Set the minimum size to at least 300x400.
    setMinimumSize(300, 400);

    // Create a line edit for the default search engine.
    m_searchEngineEdit = new QLineEdit(this);
    // Load an existing setting, or use a default if none exists.
    QSettings settings("MyCompany", "PraedoBrowser");
    m_searchEngineEdit->setText(settings.value("defaultSearchEngine", "https://www.google.com/search?q=").toString());

    // Create OK and Cancel buttons.
    m_okButton = new QPushButton("OK", this);
    m_cancelButton = new QPushButton("Cancel", this);

    // Build a form layout to display the editable options.
    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("Default Search Engine:", m_searchEngineEdit);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(formLayout);

    // Create a horizontal layout for the buttons.
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(buttonLayout);

    // Connect button signals to the appropriate slots.
    connect(m_okButton, &QPushButton::clicked, this, &SettingsDialog::onOkClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &SettingsDialog::onCancelClicked);
}

SettingsDialog::~SettingsDialog()
{
}

QString SettingsDialog::defaultSearchEngine() const
{
    return m_searchEngineEdit->text();
}

void SettingsDialog::onOkClicked()
{
    // Save the current setting.
    QSettings settings("MyCompany", "PraedoBrowser");
    settings.setValue("defaultSearchEngine", m_searchEngineEdit->text());
    accept();
}

void SettingsDialog::onCancelClicked()
{
    reject();
}
