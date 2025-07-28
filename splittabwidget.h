#ifndef SPLITTABWIDGET_H
#define SPLITTABWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QWebEngineView>
#include <QVBoxLayout>

/*
 * The SplitTabWidget encapsulates a QSplitter that holds several QWebEngineView instances.
 * You can add more views by calling addWebView(), and later extend this class to allow removal,
 * drag-and-drop reordering, or finer focus control.
 */
class SplitTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SplitTabWidget(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        splitter = new QSplitter(orientation, this);
        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->addWidget(splitter);
    }

    // Adds a new QWebEngineView into the splitter.
    void addWebView(QWebEngineView* view) {
        splitter->addWidget(view);
        view->setParent(splitter);
    }

private:
    QSplitter *splitter;
};

#endif // SPLITTABWIDGET_H
