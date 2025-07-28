// qtpasswordmanager.h
#ifndef QTPASSWORDMANAGER_H
#define QTPASSWORDMANAGER_H

#include <QObject>
#include <QSettings>
#include <QUrl>
#include <QDebug>

class QtPasswordManager : public QObject {
    Q_OBJECT
public:
    explicit QtPasswordManager(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    // This slot is now modified to automatically store credentials in QSettings.
    void passwordSubmitted(const QString &url, const QString &username, const QString &password) {
        // Parse the host from the URL
        QUrl urlObj(url);
        QString host = urlObj.host();
        if (host.isEmpty())
            host = url; // fallback if no host is found

        QSettings settings("MyCompany", "PraedoBrowser");
        settings.beginGroup("SavedPasswords");
        // Each site is saved under its host name.
        settings.beginGroup(host);
        // Save username and password in plain text.
        settings.setValue("username", username);
        settings.setValue("password", password);
        settings.endGroup();
        settings.endGroup();

        qDebug() << "Automatically saved credentials for:" << host;
    }
};

#endif // QTPASSWORDMANAGER_H
