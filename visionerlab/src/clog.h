#ifndef CLOG_H
#define CLOG_H

#include <QApplication>

#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDateTime>
#include <QMutex>
#include <QString>
#include <qapplication.h>

void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
class CLog
{
public:
    CLog();
};

#endif // CLOG_H
