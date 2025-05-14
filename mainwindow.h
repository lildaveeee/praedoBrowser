#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUrl>
#include <QMap>
#include <QFont>
#include <QWebEnginePage>  // Required for fullScreenRequested
#include <QWebEngineView>
#include <QMouseEvent>

class QToolBar;
class QAction;
class QLineEdit;
class QListWidget;
class QStackedWidget;
class QPushButton;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void navigateToUrl();
    void goBack();
    void goForward();
    void reloadPage();
    void goHome();
    void addNewTab();
    void switchTab(int index);
    void toggleSidebar();
    void toggleFullScreen(QWebEngineView *view, bool enable);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;  // Handles middle-click to close tabs

private:
    // Bang mapping for commands.
    QMap<QString, QString> bangMap;

    // Setup functions.
    void createNavigationToolbar();
    void createCentralWidgets();
    QWebEngineView* createWebView(const QUrl &url);
    void updateTabTitle(int index, const QString &title);
    void applyModernBlueTheme();

    // Members.
    QToolBar *navToolBar;
    QLineEdit *addressBar;
    QAction *backAction;
    QAction *forwardAction;
    QAction *reloadAction;
    QAction *homeAction;
    QAction *hamburgerAction;

    QWidget *sidebarWidget;
    QListWidget *tabListWidget;
    QPushButton *addTabButton;
    QStackedWidget *webStack;

    bool sidebarVisible;
};

#endif // MAINWINDOW_H
