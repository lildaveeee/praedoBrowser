#ifndef MAINWINDOW_COPY_H
#define MAINWINDOW_COPY_H

#include <QMainWindow>
#include <QMap>
#include <QTime>

class QAction;
class QToolBar;
class QLineEdit;
class QListWidget;
class QStackedWidget;
class QSplitter;
class QWidget;
class QWebEngineView;
class QTimer;
class QPropertyAnimation;
class QPushButton;
class QLabel;

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
    void addBookmark();

private:
    void createNavigationToolbar();
    void createCentralWidgets();
    QWebEngineView* createWebView(const QUrl &url);

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

    QToolBar *navToolBar = nullptr;
    QAction *backAction = nullptr;
    QAction *forwardAction = nullptr;
    QAction *reloadAction = nullptr;
    QAction *homeAction = nullptr;
    QAction *hamburgerAction = nullptr;
    QAction *bookmarkAction = nullptr;
    QAction *viewBookmarksAction = nullptr;

    QLineEdit *addressBar = nullptr;
    QListWidget *tabListWidget = nullptr;
    QPushButton *addTabButton = nullptr;
    QStackedWidget *webStack = nullptr;
    QSplitter *mainSplitter = nullptr;
    QWidget *sidebarWidget = nullptr;
    QMap<QString, QString> bangMap;

    QTimer *m_sidebarToggleTimer = nullptr;
    QTimer *sidebarHideTimer;
    bool sidebarShowLock;
    bool sidebarVisible;
    QTime lastSidebarToggleTime;

    QWidget *bookmarkBar = nullptr;
    QPropertyAnimation *bookmarkBarAnimation = nullptr;
};

#endif // MAINWINDOW_COPY_H
