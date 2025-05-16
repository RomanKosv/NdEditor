#include "chmhelp.h"
#include <QDesktopServices>

ChmHelp::ChmHelp(QObject *parent)
    : QObject{parent}
{}

bool ChmHelp::open()
{
    return QDesktopServices::openUrl(file());
}

bool ChmHelp::open(QString topic) {
    return open();
}

QUrl ChmHelp::file()
{
    return _file;
}

void ChmHelp::setFile(QUrl url)
{
    _file = url;
    emit fileChanged(url);
}
