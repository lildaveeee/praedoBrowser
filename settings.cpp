// settings.cpp

#include "settings.h"
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QHBoxLayout>
#include <QColorDialog>
#include <QRandomGenerator>
#include <QMenu>
#include <QAction>
#include <QListWidget>
#include <QApplication>
#include <QTabWidget>
#include <QFrame>
#include <QLabel>
#include <QPair>
#include <QtGlobal>

static void updateButtonColor(QPushButton *button, const QColor &color)
{
    if (button) {
        button->setStyleSheet(
            QString("background-color: %1; border: 0px;")
                .arg(color.name()));
        button->setText(color.name());
    }
}

static QColor generateRandomColor()
{
    int hue        = QRandomGenerator::global()->bounded(360);
    int saturation = QRandomGenerator::global()->bounded(128, 256);
    int lightness  = QRandomGenerator::global()->bounded(128, 256);
    return QColor::fromHsl(hue, saturation, lightness);
}

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Settings");
    setMinimumSize(300, 700);

    QSettings settings("PraedoBrowser", "PraedoBrowser");

    //
    // 1) Search‐engine field
    //
    m_searchEngineEdit = new QLineEdit(this);
    m_searchEngineEdit->setText(
        settings.value("defaultSearchEngine",
                       "https://www.duckduckgo.com/search?q=").toString()
        );
    m_searchEngineEdit->setStyleSheet("border: 0px;");

    //
    // 2) Load or fallback each color
    //
    m_backgroundColor = QColor(settings.value("backgroundColor", "#000000").toString());
    if (!m_backgroundColor.isValid())     m_backgroundColor = Qt::black;

    m_tabListColor    = QColor(settings.value("tabListColor", "#2A2118").toString());
    if (!m_tabListColor.isValid())        m_tabListColor = QColor("#2A2118");

    m_topBarColor     = QColor(settings.value("topBarColor", "#2A2118").toString());
    if (!m_topBarColor.isValid())         m_topBarColor = QColor("#2A2118");

    m_accent1Color    = QColor(settings.value("accent1Color", "#FFFFFF").toString());
    if (!m_accent1Color.isValid())        m_accent1Color = QColor("#FFFFFF");

    m_accent2Color    = QColor(settings.value("accent2Color", "#FFFFFF").toString());
    if (!m_accent2Color.isValid())        m_accent2Color = QColor("#FFFFFF");

    m_fontColor       = QColor(settings.value("fontColor", "#000000").toString());
    if (!m_fontColor.isValid())           m_fontColor = QColor("#000000");

    m_altfontColor       = QColor(settings.value("altfontColor", "#ffffff").toString());
    if (!m_altfontColor.isValid())           m_altfontColor = QColor("#ffffff");

    m_buttonColor     = QColor(settings.value("buttonColor", "#424242").toString());
    if (!m_buttonColor.isValid())         m_buttonColor = QColor("#424242");

    m_linkColor       = QColor(settings.value("linkColor", "#D4B398").toString());
    if (!m_linkColor.isValid())           m_linkColor = QColor("#D4B398");

    m_highlightColor  = QColor(settings.value("highlightColor", "#FDFF00").toString());
    if (!m_highlightColor.isValid())      m_highlightColor = QColor("#FDFF00");

    m_alternateBaseColor = QColor(settings.value("alternateBaseColor", "#FFFFFF").toString());
    if (!m_alternateBaseColor.isValid())  m_alternateBaseColor = QColor("#FFFFFF");

    //
    // 3) Create and initialize color‐pick buttons
    //
    m_backgroundColorButton    = new QPushButton(this);
    m_tabListColorButton       = new QPushButton(this);
    m_topBarColorButton        = new QPushButton(this);
    m_accent1ColorButton       = new QPushButton(this);
    m_accent2ColorButton       = new QPushButton(this);
    m_fontColorButton          = new QPushButton(this);
    m_altfontColorButton       = new QPushButton(this);
    m_buttonColorButton        = new QPushButton(this);
    m_linkColorButton          = new QPushButton(this);
    m_highlightColorButton     = new QPushButton(this);
    m_alternateBaseColorButton = new QPushButton(this);

    updateButtonColor(m_backgroundColorButton,    m_backgroundColor);
    updateButtonColor(m_tabListColorButton,       m_tabListColor);
    updateButtonColor(m_topBarColorButton,        m_topBarColor);
    updateButtonColor(m_accent1ColorButton,       m_accent1Color);
    updateButtonColor(m_accent2ColorButton,       m_accent2Color);
    updateButtonColor(m_fontColorButton,          m_fontColor);
    updateButtonColor(m_altfontColorButton,       m_altfontColor);
    updateButtonColor(m_buttonColorButton,        m_buttonColor);
    updateButtonColor(m_linkColorButton,          m_linkColor);
    updateButtonColor(m_highlightColorButton,     m_highlightColor);
    updateButtonColor(m_alternateBaseColorButton, m_alternateBaseColor);

    auto connectColorBtn = [this](QPushButton *btn, QColor &member, const QString &title) {
        connect(btn, &QPushButton::clicked, this, [=, &member]() {
            QColor chosen = QColorDialog::getColor(member, this, title);
            if (chosen.isValid()) {
                member = chosen;
                updateButtonColor(btn, member);
            }
        });
    };

    connectColorBtn(m_backgroundColorButton,    m_backgroundColor,    "Select Background Color");
    connectColorBtn(m_tabListColorButton,       m_tabListColor,       "Select Tab List Color");
    connectColorBtn(m_topBarColorButton,        m_topBarColor,        "Select Top Bar Color");
    connectColorBtn(m_accent1ColorButton,       m_accent1Color,       "Select Accent1 Color");
    connectColorBtn(m_accent2ColorButton,       m_accent2Color,       "Select Accent2 Color");
    connectColorBtn(m_fontColorButton,          m_fontColor,          "Select Font Color");
    connectColorBtn(m_altfontColorButton,       m_altfontColor,          "Select Alternate Font Color");
    connectColorBtn(m_buttonColorButton,        m_buttonColor,        "Select Button Color");
    connectColorBtn(m_linkColorButton,          m_linkColor,          "Select Link Color");
    connectColorBtn(m_highlightColorButton,     m_highlightColor,     "Select Highlight Color");
    connectColorBtn(m_alternateBaseColorButton, m_alternateBaseColor, "Select Alternate Base Color");

    //
    // 4) Style main dialog
    //
    setStyleSheet(QString(
                      "QDialog {"
                      "    border: none;"
                      "    border-radius: 15px;"
                      "    background-color: %1;"
                      "}"
                      ).arg(m_backgroundColor.name()));

    //
    // 5) Build TabWidget & Theme tab
    //
    QTabWidget *tabWidget = new QTabWidget(this);
    tabWidget->setStyleSheet(QString(
                                 "QTabWidget::tab-bar { alignment: center; }"
                                 "QTabWidget::pane { border: none; border-radius: 15px; }"
                                 "QTabBar::tab {"
                                 "    background: %1;"
                                 "    color: %2;"
                                 "    padding: 10px;"
                                 "    border: none;"
                                 "    border-top-left-radius: 10px;"
                                 "    border-top-right-radius: 10px;"
                                 "    margin: 0 1px;"
                                 "}"
                                 "QTabBar::tab:selected { background: %1; }"
                                 ).arg(m_tabListColor.name(), m_altfontColor.name()));

    // --- Theme Tab ---
    {
        QWidget *themeTab = new QWidget;
        QVBoxLayout *themeLayout = new QVBoxLayout(themeTab);
        QFormLayout *formLayout  = new QFormLayout;

        // Helper to add a row with a colored label
        auto addColoredRow = [&](const QString &text, QWidget *widget, const QColor &labelColor) {
            QLabel *label = new QLabel(text, themeTab);
            label->setStyleSheet(QString("color: %1;").arg(labelColor.name()));
            formLayout->addRow(label, widget);
        };

        // All labels use altfont color, except the “Alternate Font Color:” label
        addColoredRow("Default Search Engine:",  m_searchEngineEdit,        m_altfontColor);
        addColoredRow("Background Color:",       m_backgroundColorButton,   m_altfontColor);
        addColoredRow("Tab List Color:",         m_tabListColorButton,      m_altfontColor);
        addColoredRow("Top Bar Color:",          m_topBarColorButton,       m_altfontColor);
        addColoredRow("Accent1 Color:",          m_accent1ColorButton,      m_altfontColor);
        addColoredRow("Accent2 Color:",          m_accent2ColorButton,      m_altfontColor);
        addColoredRow("Font Color:",             m_fontColorButton,         m_altfontColor);
        addColoredRow("Alternate Font Color:",   m_altfontColorButton,      m_altfontColor);
        addColoredRow("Button Color:",           m_buttonColorButton,       m_altfontColor);
        addColoredRow("Link Color:",             m_linkColorButton,         m_altfontColor);
        addColoredRow("Highlight Color:",        m_highlightColorButton,    m_altfontColor);
        addColoredRow("Alternate Base Color:",   m_alternateBaseColorButton,m_altfontColor);

        themeLayout->addLayout(formLayout);

        m_randomizePaletteButton = new QPushButton("Randomize Colors", this);
        m_randomizePaletteButton->setStyleSheet(QString(
                                                    "QPushButton {"
                                                    "    background-color: %1;"
                                                    "    color: %2;"
                                                    "    border: none;"
                                                    "    padding: 5px;"
                                                    "    border-radius: 10px;"
                                                    "}"
                                                    ).arg(m_buttonColor.name(), m_altfontColor.name()));
        connect(m_randomizePaletteButton, &QPushButton::clicked,
                this, &SettingsDialog::onRandomizeClicked);

        themeLayout->addWidget(m_randomizePaletteButton);
        tabWidget->addTab(themeTab, "Theme");
    }

    //
    // 6) Build Bookmarks tab
    //
    {
        QWidget *bookmarksTab = new QWidget;
        bookmarksTab->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        QVBoxLayout *bookmarkLayout = new QVBoxLayout(bookmarksTab);

        QListWidget *bookmarkList = new QListWidget(bookmarksTab);
        bookmarkList->setContextMenuPolicy(Qt::CustomContextMenu);
        bookmarkList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bookmarkList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        bookmarkList->setFrameShape(QFrame::NoFrame);

        QVariantList bookmarks = settings.value("Bookmarks").toList();
        for (const QVariant &var : bookmarks) {
            QVariantMap bm = var.toMap();
            QListWidgetItem *item =
                new QListWidgetItem(bm["title"].toString(), bookmarkList);
            item->setData(Qt::UserRole, bm["url"].toString());
        }
        bookmarkLayout->addWidget(bookmarkList);

        bookmarksTab->setStyleSheet(QString(
                                        "QWidget { background-color: %1; color: %2; }"
                                        "QListWidget { background-color: %1; color: %2; }"
                                        "QListWidget::item:selected { background-color: %3; }"
                                        "QPushButton {"
                                        "    background-color: %3;"
                                        "    color: %2;"
                                        "    border-radius: 10px;"
                                        "    padding: 5px;"
                                        "}"
                                        ).arg(m_tabListColor.name(),
                                             m_altfontColor.name(),
                                             m_buttonColor.name()));

        connect(bookmarkList, &QListWidget::customContextMenuRequested,
                [this, bookmarkList](const QPoint &pos) {
                    QListWidgetItem *item = bookmarkList->itemAt(pos);
                    if (!item) return;

                    QMenu menu;
                    QAction *del = menu.addAction("Delete Bookmark");
                    connect(del, &QAction::triggered, [=]() {
                        QSettings settings("PraedoBrowser", "PraedoBrowser");
                        QVariantList list = settings.value("Bookmarks").toList();

                        QString url   = item->data(Qt::UserRole).toString();
                        QString title = item->text();
                        for (int i = 0; i < list.size(); ++i) {
                            QVariantMap bm = list[i].toMap();
                            if (bm["url"].toString() == url &&
                                bm["title"].toString() == title)
                            {
                                list.removeAt(i);
                                break;
                            }
                        }
                        settings.setValue("Bookmarks", list);
                        settings.sync();

                        delete bookmarkList->takeItem(bookmarkList->row(item));

                        MainWindow *mw = qobject_cast<MainWindow*>(this->window());
                        if (!mw)
                            mw = qobject_cast<MainWindow*>(QApplication::activeWindow());
                        if (mw)
                            mw->refreshBookmarkBar();
                    });
                    menu.exec(bookmarkList->viewport()->mapToGlobal(pos));
                });

        tabWidget->addTab(bookmarksTab, "Bookmarks");
    }

    //
    // 7) Build Shortcuts tab with dynamic URL⇔sequence pairs
    {
        QWidget *shortcutsTab = new QWidget;
        QVBoxLayout *shortcutsLayout = new QVBoxLayout(shortcutsTab);
        QFormLayout *shortcutsForm = new QFormLayout;

        // built-in shortcuts
        m_newTabShortcutEdit    = new QLineEdit(shortcutsTab);
        m_searchbarShortcutEdit = new QLineEdit(shortcutsTab);
        m_closeTabShortcutEdit  = new QLineEdit(shortcutsTab);
        m_cycleTabShortcutEdit  = new QLineEdit(shortcutsTab);

        m_newTabShortcutEdit->setText(
            settings.value("newTabShortcut", "Ctrl+N").toString());
        m_searchbarShortcutEdit->setText(
            settings.value("searchbarShortcut", "Ctrl+S").toString());
        m_closeTabShortcutEdit->setText(
            settings.value("closeTabShortcut", "Ctrl+Q").toString());
        m_cycleTabShortcutEdit->setText(
            settings.value("cycleTabShortcut", "Ctrl+Tab").toString());

        // make the built-in editors use fontColor
        QString editStyle = QString("color: %1;").arg(m_fontColor.name());
        m_newTabShortcutEdit->setStyleSheet(editStyle);
        m_searchbarShortcutEdit->setStyleSheet(editStyle);
        m_closeTabShortcutEdit->setStyleSheet(editStyle);
        m_cycleTabShortcutEdit->setStyleSheet(editStyle);

        // helper: add a row whose label uses m_altfontColor
        auto addColoredRow = [&](const QString &labelText, QWidget *editor) {
            QLabel *lbl = new QLabel(labelText, shortcutsTab);
            lbl->setStyleSheet(QString("color: %1;")
                                   .arg(m_altfontColor.name()));
            shortcutsForm->addRow(lbl, editor);
        };

        addColoredRow("New Tab Shortcut:",    m_newTabShortcutEdit);
        addColoredRow("Search Bar Shortcut:", m_searchbarShortcutEdit);
        addColoredRow("Close Tab Shortcut:",  m_closeTabShortcutEdit);
        addColoredRow("Cycle Tab Shortcut:",  m_cycleTabShortcutEdit);
        m_toggleTopBarShortcutEdit = new QLineEdit(shortcutsTab);
        m_toggleTopBarShortcutEdit->setText(
            settings.value("toggleTopBarShortcut", "Ctrl+T").toString());
        m_toggleTopBarShortcutEdit->setStyleSheet(editStyle);
        addColoredRow("Toggle Top-Bar Shortcut:",
                      m_toggleTopBarShortcutEdit);
        shortcutsLayout->addLayout(shortcutsForm);

        // ── NEW: Central Search shortcut
        m_toggleCentralSearchShortcutEdit = new QLineEdit(this);
        m_toggleCentralSearchShortcutEdit->setText(
            settings.value("toggleCentralSearchShortcut",
                           "Ctrl+Space").toString());
        addColoredRow("Central Search Shortcut:",
                      m_toggleCentralSearchShortcutEdit);


        // custom URL⇔Shortcut pairs header
        QLabel *customLabel2 = new QLabel("Custom URL⇔Shortcut Pairs:", shortcutsTab);
        customLabel2->setStyleSheet(
            QString("color: %1;").arg(m_altfontColor.name()));
        shortcutsLayout->addWidget(customLabel2);

        QVBoxLayout *customListLayout = new QVBoxLayout;
        shortcutsLayout->addLayout(customListLayout);

        // helper: create one custom row + right-click → delete
        std::function<void(const QString&, const QString&)> addShortcutRow =
            [this, shortcutsTab, customListLayout](const QString &url,
                                                   const QString &seq) {
                QLineEdit *urlEdit = new QLineEdit(url, shortcutsTab);
                QLineEdit *scEdit  = new QLineEdit(seq, shortcutsTab);

                // make the custom editors use fontColor
                QString customEditStyle =
                    QString("color: %1;").arg(m_fontColor.name());
                urlEdit->setStyleSheet(customEditStyle);
                scEdit->setStyleSheet(customEditStyle);

                m_customShortcutsEdits.append(qMakePair(urlEdit, scEdit));

                QWidget *rowWidget = new QWidget(shortcutsTab);
                QHBoxLayout *rowLayout = new QHBoxLayout(rowWidget);
                rowLayout->setContentsMargins(0, 0, 0, 0);
                rowLayout->addWidget(urlEdit);
                rowLayout->addWidget(scEdit);
                rowWidget->setLayout(rowLayout);

                rowWidget->setContextMenuPolicy(Qt::CustomContextMenu);
                connect(rowWidget, &QWidget::customContextMenuRequested,
                        this, [=](const QPoint &pos) {
                            QMenu menu;
                            QAction *del = menu.addAction("Delete Shortcut");
                            connect(del, &QAction::triggered, this, [=]() {
                                for (int i = 0; i < m_customShortcutsEdits.size(); ++i) {
                                    auto p = m_customShortcutsEdits.at(i);
                                    if (p.first == urlEdit && p.second == scEdit) {
                                        m_customShortcutsEdits.removeAt(i);
                                        break;
                                    }
                                }
                                customListLayout->removeWidget(rowWidget);
                                rowWidget->deleteLater();
                            });
                            menu.exec(rowWidget->mapToGlobal(pos));
                        });

                customListLayout->addWidget(rowWidget);
            };

        // load and display saved custom shortcuts
        QVariantList custom = settings.value("customShortcuts").toList();
        for (auto v : custom) {
            QVariantMap m = v.toMap();
            addShortcutRow(m["url"].toString(), m["sequence"].toString());
        }

        // “Add Shortcut” button
        m_addShortcutButton = new QPushButton("Add Shortcut", shortcutsTab);
        m_addShortcutButton->setStyleSheet(QString(
                                               "QPushButton {"
                                               "    background-color: %1;"
                                               "    color: %2;"
                                               "    border: none;"
                                               "    padding: 5px;"
                                               "    border-radius: 10px;"
                                               "}"
                                               ).arg(m_buttonColor.name(), m_altfontColor.name()));

        shortcutsLayout->addWidget(m_addShortcutButton);
        connect(m_addShortcutButton, &QPushButton::clicked, this, [=]() {
            addShortcutRow(QString(), QString());
        });

        tabWidget->addTab(shortcutsTab, "Shortcuts");
    }

    //
    // 8) OK / Cancel
    //
    {
        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(tabWidget);

        m_okButton     = new QPushButton("OK", this);
        m_cancelButton = new QPushButton("Cancel", this);
        m_okButton->setStyleSheet(m_randomizePaletteButton->styleSheet());
        m_cancelButton->setStyleSheet(m_randomizePaletteButton->styleSheet());

        QHBoxLayout *btnLayout = new QHBoxLayout;
        btnLayout->addStretch();
        btnLayout->addWidget(m_okButton);
        btnLayout->addWidget(m_cancelButton);
        mainLayout->addLayout(btnLayout);

        connect(m_okButton,     &QPushButton::clicked,
                this, &SettingsDialog::onOkClicked);
        connect(m_cancelButton, &QPushButton::clicked,
                this, &SettingsDialog::onCancelClicked);
    }
}

SettingsDialog::~SettingsDialog()
{
}

QString SettingsDialog::defaultSearchEngine() const
{
    return m_searchEngineEdit->text();
}

QString SettingsDialog::backgroundColor() const
{
    return m_backgroundColor.name();
}

QString SettingsDialog::toggleCentralSearchShortcut() const
{
    return m_toggleCentralSearchShortcutEdit->text();
}

void SettingsDialog::onOkClicked()
{
    QSettings settings("PraedoBrowser", "PraedoBrowser");
    settings.setValue("defaultSearchEngine",
                      m_searchEngineEdit->text());
    settings.setValue("backgroundColor",
                      m_backgroundColor.name());
    settings.setValue("tabListColor",
                      m_tabListColor.name());
    settings.setValue("topBarColor",
                      m_topBarColor.name());
    settings.setValue("accent1Color",
                      m_accent1Color.name());
    settings.setValue("accent2Color",
                      m_accent2Color.name());
    settings.setValue("fontColor",
                      m_fontColor.name());
    settings.setValue("altfontColor",
                      m_altfontColor.name());
    settings.setValue("buttonColor",
                      m_buttonColor.name());
    settings.setValue("linkColor",
                      m_linkColor.name());
    settings.setValue("highlightColor",
                      m_highlightColor.name());
    settings.setValue("alternateBaseColor",
                      m_alternateBaseColor.name());

    settings.setValue("newTabShortcut",
                      m_newTabShortcutEdit->text());
    settings.setValue("searchbarShortcut",
                      m_searchbarShortcutEdit->text());
    settings.setValue("closeTabShortcut",
                      m_closeTabShortcutEdit->text());
    settings.setValue("cycleTabShortcut",
                      m_cycleTabShortcutEdit->text());
    settings.setValue("toggleTopBarShortcut",
                      m_toggleTopBarShortcutEdit->text());
    settings.setValue("toggleCentralSearchShortcut",
                      m_toggleCentralSearchShortcutEdit->text());
    QVariantList customToSave;
    for (auto &p : m_customShortcutsEdits) {
        QVariantMap m;
        m["url"]      = p.first->text();
        m["sequence"] = p.second->text();
        customToSave.append(m);
    }
    settings.setValue("customShortcuts", customToSave);

    emit settingsUpdated();
    accept();
}

void SettingsDialog::onCancelClicked()
{
    reject();
}

void SettingsDialog::onRandomizeClicked()
{
    const int colorCount = 10;
    QColor newColors[colorCount];
    for (int i = 0; i < colorCount; ++i)
        newColors[i] = generateRandomColor();

    m_backgroundColor    = newColors[0];
    m_tabListColor       = newColors[1];
    m_topBarColor        = newColors[2];
    m_accent1Color       = newColors[3];
    m_accent2Color       = newColors[4];
    m_buttonColor        = newColors[6];
    m_linkColor          = newColors[7];
    m_highlightColor     = newColors[8];
    m_alternateBaseColor = newColors[9];

    updateButtonColor(m_backgroundColorButton,    m_backgroundColor);
    updateButtonColor(m_tabListColorButton,       m_tabListColor);
    updateButtonColor(m_topBarColorButton,        m_topBarColor);
    updateButtonColor(m_accent1ColorButton,       m_accent1Color);
    updateButtonColor(m_accent2ColorButton,       m_accent2Color);
    updateButtonColor(m_buttonColorButton,        m_buttonColor);
    updateButtonColor(m_linkColorButton,          m_linkColor);
    updateButtonColor(m_highlightColorButton,     m_highlightColor);
    updateButtonColor(m_alternateBaseColorButton, m_alternateBaseColor);
}
