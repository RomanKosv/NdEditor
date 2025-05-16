#ifndef CHMHELP_H
#define CHMHELP_H

#include <QObject>
#include <QUrl>
#include <qqmlintegration.h>

class ChmHelp : public QObject
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit ChmHelp(QObject *parent = nullptr);
    Q_INVOKABLE bool open();
    Q_INVOKABLE bool open(QString topic);
    Q_PROPERTY(QUrl file READ file WRITE setFile NOTIFY fileChanged FINAL)
    QUrl file();
    void setFile(QUrl);
signals:
    void fileChanged(QUrl);
private:
    QUrl _file;
};

#endif // CHMHELP_H
