#ifndef MEDIA_H
#define MEDIA_H

#include <QWidget>
#include <QWebEnginePage>

class QListWidget;
class QListWidgetItem;
class QStackedWidget;
class QWebEngineView;
class QComboBox;
class QPushButton;

class Media : public QWidget
{
    Q_OBJECT
public:
    explicit Media(const QString &extraQuery, QWidget *parent = nullptr);
    void setExtraText(const QString &text);
    void searchImdb(const QString &query);

public slots:
    void updateBackgroundColor();

private:
    void toggleFullScreen(QWebEngineView *view, bool enable);

    QListWidget *listWidget_;
    QStackedWidget *stackedWidget_;
    QWidget *embedContainer_;
    QWebEngineView *embedView_;
    QComboBox *seasonBox_;
    QComboBox *episodeBox_;
    QPushButton *loadEpisodeButton_;

    QWidget *fullscreenWindow_;
};

#endif // MEDIA_H
