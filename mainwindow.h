#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QTime>
#include <QUrl>
#include <QIcon>
#include <QDialog>
#include <QShortcut>

#include "settings.h"    // Provides SettingsDialog

// Forward declarations of Qt classes to speed up compile times
class QFileSystemWatcher;
class QTimer;
class QToolBar;
class QAction;
class QLineEdit;
class QListWidget;
class QStackedWidget;
class QSplitter;
class QWidget;
class QWebEngineView;
class QPushButton;
class QLabel;
class QPropertyAnimation;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void refreshBookmarkBar();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void addNewTab();
    void switchTab(int index);
    void navigateToUrl();
    void goBack();
    void goForward();
    void reloadPage();
    void goHome();
    void toggleSidebar();
    void toggleTopBar();
    void addBookmark();
    void reloadThemeSettings();
    void toggleCentralSearch();
    void onCentralSearchReturn();

private:
    // Setup routines
    void createNavigationToolbar();
    void createCentralWidgets();
    QWebEngineView* createWebView(const QUrl &url);

    // Helpers
    void updateTabTitle(int index, const QString &title);
    void startScrolling(QLabel *label, const QString &newText);
    void updateTabIcon(int index, const QIcon &icon);
    void updateTabDisplayMode();
    void updateNavigationIcons();
    void applyModernBlueTheme();
    void updateSidebarColor();
    void toggleFullScreen(QWebEngineView *view, bool enable);
    QMap<QString, QString> createBangMap();

    void createBookmarkBar();
    void showBookmarkBar();
    void hideBookmarkBar();
    void hideSidebar();

    // ————————————————————————————
    // Persistent settings watcher
    QFileSystemWatcher  *settingsWatcher       = nullptr;
    QTimer              *settingsReloadTimer   = nullptr;

    // Navigation toolbar
    QToolBar            *navToolBar            = nullptr;
    QAction             *backAction            = nullptr;
    QAction             *forwardAction         = nullptr;
    QAction             *reloadAction          = nullptr;
    QAction             *homeAction            = nullptr;
    QAction             *hamburgerAction       = nullptr;
    QAction             *bookmarkAction        = nullptr;
    QAction             *viewBookmarksAction   = nullptr;

    // Browser UI
    QLineEdit           *addressBar            = nullptr;
    QListWidget         *tabListWidget         = nullptr;
    QPushButton         *addTabButton          = nullptr;
    QStackedWidget      *webStack              = nullptr;
    QSplitter           *mainSplitter          = nullptr;
    QWidget             *sidebarWidget         = nullptr;
    QMap<QString, QString> bangMap;

    // Sidebar auto-hide
    QTimer              *m_sidebarToggleTimer  = nullptr;
    QTimer              *sidebarHideTimer      = nullptr;
    bool                 sidebarShowLock       = false;
    bool                 sidebarVisible        = false;
    QTime                lastSidebarToggleTime;

    // Bookmark bar
    QWidget             *bookmarkBar           = nullptr;
    QPropertyAnimation  *bookmarkBarAnimation  = nullptr;

    // Central-search overlay
    QDialog             *centralSearchDialog   = nullptr;
    QLineEdit           *centralSearchEdit     = nullptr;

    // Settings dialog & shortcut rebinding
    SettingsDialog      *settingsDialog        = nullptr;
    QShortcut           *centralSearchShortcut = nullptr;
};

#endif // MAINWINDOW_H
