#include "media.h"

#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QRegularExpression>
#include <QRegularExpressionMatchIterator>
#include <QDebug>
#include <QPixmap>
#include <QLabel>
#include <QHBoxLayout>
#include <QTextDocument>
#include <QStackedWidget>
#include <QPushButton>
#include <QWebEngineView>
#include <QComboBox>
#include <QSettings>
#include <QString>
#include <QNetworkRequest>
#include <QWebEngineSettings>
#include <QWebEngineFullScreenRequest>

Media::Media(const QString &extraQuery, QWidget *parent)
    : QWidget(parent)
    , embedContainer_(nullptr)
    , embedView_(nullptr)
    , seasonBox_(nullptr)
    , episodeBox_(nullptr)
    , loadEpisodeButton_(nullptr)
    , fullscreenWindow_(nullptr)
{
    QSettings settings("PraedoBrowser", "PraedoBrowser");
    QString bgColor = settings.value("backgroundColor", "black").toString();
    if (bgColor.compare("Custom...", Qt::CaseInsensitive) == 0) {
        bgColor = "#C8C8C8";
    }

    setStyleSheet(QString("background-color: %1;").arg(bgColor));

    QVBoxLayout *layout = new QVBoxLayout(this);

    listWidget_ = new QListWidget(this);
    listWidget_->setStyleSheet(QString("color: white; font-size: 16px; background-color: %1;").arg(bgColor));
    listWidget_->setViewMode(QListView::IconMode);
    listWidget_->setFlow(QListView::LeftToRight);
    listWidget_->setResizeMode(QListView::Adjust);
    listWidget_->setMovement(QListView::Static);
    listWidget_->setWrapping(true);
    listWidget_->setSpacing(20);
    listWidget_->setGridSize(QSize(110, 170));

    stackedWidget_ = new QStackedWidget(this);
    stackedWidget_->addWidget(listWidget_);
    layout->addWidget(stackedWidget_);
    setLayout(layout);

    connect(listWidget_, &QListWidget::itemClicked, this, [this](QListWidgetItem *item) {
        QString imdbId = item->data(Qt::UserRole).toString();
        if (!imdbId.isEmpty()) {
            QString imdbPageUrl = QString("https://www.imdb.com/title/%1/").arg(imdbId);
            QUrl url(imdbPageUrl);
            QNetworkRequest request(url);
            request.setRawHeader("User-Agent",
                                 "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                                 "AppleWebKit/537.36 (KHTML, like Gecko) "
                                 "Chrome/115.0.0.0 Safari/537.36");
            request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
            QNetworkAccessManager *nam = new QNetworkAccessManager(this);
            connect(nam, &QNetworkAccessManager::finished, this, [this, imdbId, nam](QNetworkReply *reply) {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray data = reply->readAll();
                    QString html = QString::fromUtf8(data);

                    bool isTVSeries = false;
                    QRegularExpression reType("\"@type\"\\s*:\\s*\"(TVSeries|Movie)\"");
                    QRegularExpressionMatch matchType = reType.match(html);
                    if (matchType.hasMatch()){
                        QString type = matchType.captured(1);
                        isTVSeries = (type.compare("TVSeries", Qt::CaseInsensitive) == 0);
                    }
                    qDebug() << "Media type:" << (isTVSeries ? "TV Series" : "Movie");

                    QString embedUrl;
                    if (isTVSeries)
                        embedUrl = QString("https://vidsrc.xyz/embed/tv?imdb=%1").arg(imdbId);
                    else
                        embedUrl = QString("https://vidsrc.xyz/embed/movie?imdb=%1").arg(imdbId);

                    if (embedContainer_ != nullptr) {
                        int idx = stackedWidget_->indexOf(embedContainer_);
                        if (idx != -1) {
                            QWidget *oldEmbed = embedContainer_;
                            stackedWidget_->removeWidget(oldEmbed);
                            oldEmbed->deleteLater();
                        }
                        embedContainer_ = nullptr;
                        seasonBox_ = nullptr;
                        episodeBox_ = nullptr;
                        loadEpisodeButton_ = nullptr;
                        embedView_ = nullptr;
                    }

                    embedContainer_ = new QWidget();
                    QVBoxLayout *embedLayout = new QVBoxLayout(embedContainer_);

                    QPushButton *backButton = new QPushButton("Back", embedContainer_);
                    backButton->setStyleSheet("font-size: 12px; padding: 5px;");
                    connect(backButton, &QPushButton::clicked, this, [this]() {
                        if (embedView_)
                            embedView_->page()->runJavaScript("document.querySelector('video')?.pause();");
                        stackedWidget_->setCurrentWidget(listWidget_);
                        toggleFullScreen(embedView_, false);
                    });
                    embedLayout->addWidget(backButton);

                    embedView_ = new QWebEngineView(embedContainer_);
                    embedView_->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled, true);
                    connect(embedView_->page(), &QWebEnginePage::fullScreenRequested, this,
                            [this](QWebEngineFullScreenRequest request) {
                                qDebug() << "Fullscreen request:" << request.toggleOn();
                                request.accept();
                                toggleFullScreen(embedView_, request.toggleOn());
                            });
                    embedLayout->addWidget(embedView_);

                    stackedWidget_->addWidget(embedContainer_);

                    if (isTVSeries) {
                        QHBoxLayout *selectionLayout = new QHBoxLayout();

                        seasonBox_ = new QComboBox(embedContainer_);
                        for (int i = 1; i <= 5; i++) {
                            seasonBox_->addItem(QString("Season %1").arg(i), i);
                        }
                        selectionLayout->addWidget(seasonBox_);

                        episodeBox_ = new QComboBox(embedContainer_);
                        for (int i = 1; i <= 10; i++) {
                            episodeBox_->addItem(QString("Episode %1").arg(i), i);
                        }
                        selectionLayout->addWidget(episodeBox_);

                        loadEpisodeButton_ = new QPushButton("Load Episode", embedContainer_);
                        selectionLayout->addWidget(loadEpisodeButton_);

                        embedLayout->insertLayout(1, selectionLayout);

                        disconnect(loadEpisodeButton_, nullptr, nullptr, nullptr);
                        connect(loadEpisodeButton_, &QPushButton::clicked, this, [this, imdbId]() {
                            int season = seasonBox_->currentData().toInt();
                            int episode = episodeBox_->currentData().toInt();
                            QString updatedEmbedUrl = QString("https://vidsrc.xyz/embed/tv?imdb=%1&season=%2&episode=%3")
                                                          .arg(imdbId).arg(season).arg(episode);
                            embedView_->load(QUrl(updatedEmbedUrl));
                        });

                        int season = seasonBox_->currentData().toInt();
                        int episode = episodeBox_->currentData().toInt();
                        QString updatedEmbedUrl = QString("https://vidsrc.xyz/embed/tv?imdb=%1&season=%2&episode=%3")
                                                      .arg(imdbId).arg(season).arg(episode);
                        embedView_->load(QUrl(updatedEmbedUrl));
                    } else {
                        embedView_->load(QUrl(embedUrl));
                    }

                    stackedWidget_->setCurrentWidget(embedContainer_);
                } else {
                    qDebug() << "Error fetching IMDb page:" << reply->errorString();
                }
                reply->deleteLater();
                nam->deleteLater();
            });
            nam->get(request);
        }
    });

    if (!extraQuery.isEmpty())
        setExtraText(extraQuery);
}

void Media::setExtraText(const QString &text)
{
    listWidget_->clear();
    listWidget_->addItem("Searching for \"" + text + "\" ...");
    stackedWidget_->setCurrentWidget(listWidget_);
    searchImdb(text);
}

void Media::searchImdb(const QString &query)
{
    bool isShow = false;
    QString searchQuery = query;
    if (searchQuery.trimmed().toLower() == "tv" ||
        searchQuery.trimmed().toLower().startsWith("tv ")) {
        isShow = true;
        searchQuery = searchQuery.mid(3).trimmed();
    } else if (searchQuery.trimmed().toLower().startsWith("show ")) {
        isShow = true;
        searchQuery = searchQuery.mid(5).trimmed();
    }

    QUrl url("https://www.imdb.com/find");
    QUrlQuery urlQuery;
    urlQuery.addQueryItem("q", searchQuery);
    urlQuery.addQueryItem("s", isShow ? "se" : "tt");
    url.setQuery(urlQuery);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                         "AppleWebKit/537.36 (KHTML, like Gecko) "
                         "Chrome/115.0.0.0 Safari/537.36");
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    QNetworkAccessManager *nam = new QNetworkAccessManager(this);
    connect(nam, &QNetworkAccessManager::finished, this,
            [this, searchQuery, nam](QNetworkReply *reply) {
                if (reply->error() == QNetworkReply::NoError) {
                    QByteArray data = reply->readAll();
                    QString html = QString::fromUtf8(data);
                    qDebug() << "HTML returned:" << html.left(500);

                    QRegularExpression re(
                        "<li[^>]*find-result-item[^>]*>.*?<img[^>]*(?:srcset=[\"']([^\"']+)[\"'])?[^>]*src=[\"']([^\"']+)[\"'][^>]*>.*?<a[^>]*href=[\"']/title/(tt\\d+)/[^>]*>([^<]+)</a>",
                        QRegularExpression::DotMatchesEverythingOption | QRegularExpression::CaseInsensitiveOption);

                    QRegularExpressionMatchIterator it = re.globalMatch(html);
                    listWidget_->clear();

                    while (it.hasNext()) {
                        QRegularExpressionMatch match = it.next();
                        QString srcsetStr = match.captured(1).trimmed();
                        QString fallbackSrc = match.captured(2).trimmed();
                        QString imdbId = match.captured(3);
                        QString title = match.captured(4).trimmed();
                        {
                            QTextDocument doc;
                            doc.setHtml(title);
                            title = doc.toPlainText();
                        }
                        QString displayText =title;

                        QString imageUrl;
                        if (!srcsetStr.isEmpty()) {
                            int bestWidth = 0;
                            QString bestUrl;
                            QStringList parts = srcsetStr.split(",", Qt::SkipEmptyParts);
                            QRegularExpression rePart("([^\\s]+)\\s+(\\d+)w");
                            for (const QString &part : parts) {
                                QString trimmedPart = part.trimmed();
                                QRegularExpressionMatch m = rePart.match(trimmedPart);
                                if (m.hasMatch()) {
                                    QString candidateUrl = m.captured(1);
                                    int width = m.captured(2).toInt();
                                    if (width > bestWidth) {
                                        bestWidth = width;
                                        bestUrl = candidateUrl;
                                    }
                                }
                            }
                            imageUrl = bestUrl.isEmpty() ? fallbackSrc : bestUrl;
                        } else {
                            imageUrl = fallbackSrc;
                        }

                        QWidget *resultWidget = new QWidget();
                        QVBoxLayout *itemLayout = new QVBoxLayout(resultWidget);
                        itemLayout->setContentsMargins(5, 5, 5, 5);
                        itemLayout->setSpacing(5);

                        QLabel *imageLabel = new QLabel();
                        imageLabel->setFixedSize(50, 74);
                        imageLabel->setScaledContents(true);
                        itemLayout->addWidget(imageLabel, 0, Qt::AlignCenter);

                        QLabel *textLabel = new QLabel(displayText);
                        textLabel->setStyleSheet("color: white; font-size: 12px;");
                        textLabel->setAlignment(Qt::AlignCenter);
                        textLabel->setWordWrap(true);
                        itemLayout->addWidget(textLabel);

                        QListWidgetItem *listItem = new QListWidgetItem();
                        listItem->setSizeHint(QSize(170, 280));
                        listItem->setData(Qt::UserRole, imdbId);
                        listWidget_->addItem(listItem);
                        listWidget_->setItemWidget(listItem, resultWidget);

                        QUrl imgUrl(imageUrl);
                        QNetworkRequest imgRequest(imgUrl);
                        QNetworkAccessManager *imgNam = new QNetworkAccessManager(this);
                        connect(imgNam, &QNetworkAccessManager::finished, this,
                                [imageLabel, imgNam](QNetworkReply *imgReply) {
                                    if (imgReply->error() == QNetworkReply::NoError) {
                                        QByteArray imgData = imgReply->readAll();
                                        QPixmap pix;
                                        if (pix.loadFromData(imgData)) {
                                            QPixmap scaled = pix.scaled(imageLabel->size(),
                                                                        Qt::KeepAspectRatio,
                                                                        Qt::SmoothTransformation);
                                            imageLabel->setPixmap(scaled);
                                        }
                                    } else {
                                        qDebug() << "Image download error:" << imgReply->errorString();
                                    }
                                    imgReply->deleteLater();
                                    imgNam->deleteLater();
                                });
                        imgNam->get(imgRequest);
                    }
                    if (listWidget_->count() == 0) {
                        listWidget_->addItem("No results found for \"" + searchQuery + "\".");
                    }
                } else {
                    listWidget_->clear();
                    listWidget_->addItem("Error in search: " + reply->errorString());
                    qDebug() << "Network error:" << reply->errorString();
                }
                reply->deleteLater();
                nam->deleteLater();
            });
    nam->get(request);
}

void Media::updateBackgroundColor()
{
    QSettings settings("PraedoBrowser", "PraedoBrowser");
    QString bgColor = settings.value("backgroundColor", "#000000").toString();
    if (bgColor.compare("Custom...", Qt::CaseInsensitive) == 0) {
        bgColor = "#C8C8C8";
    }
    QString textColor = settings.value("fontColor", "#D4B398").toString();
    qDebug() << "Updating Media widget background to:" << bgColor << "and text color to:" << textColor;
    setStyleSheet(QString("background-color: %1;").arg(bgColor));
    if (stackedWidget_ && stackedWidget_->currentWidget() != listWidget_) {
        QWidget *currentWidget = stackedWidget_->currentWidget();
        currentWidget->setStyleSheet(QString("background-color: %1;").arg(bgColor));
    }
    update();
}

void Media::toggleFullScreen(QWebEngineView *view, bool enable)
{
    if (enable) {
        fullscreenWindow_ = new QWidget(nullptr, Qt::Window);
        fullscreenWindow_->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

        QVBoxLayout *layout = new QVBoxLayout(fullscreenWindow_);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);

        view->setParent(fullscreenWindow_);
        layout->addWidget(view);

        fullscreenWindow_->showFullScreen();
    } else {
        if (fullscreenWindow_) {
            view->setParent(embedContainer_);
            QVBoxLayout *embedLayout = qobject_cast<QVBoxLayout*>(embedContainer_->layout());
            if (embedLayout) {
                embedLayout->addWidget(view);
            }
            fullscreenWindow_->close();
            fullscreenWindow_->deleteLater();
            fullscreenWindow_ = nullptr;
        }
    }
}
