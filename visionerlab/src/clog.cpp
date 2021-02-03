#include "clog.h"

CLog clog;
CLog::CLog()
{

}
void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // 加锁
       static QMutex mutex;
       mutex.lock();

       QByteArray localMsg = msg.toLocal8Bit();

       QString strMsg("");
       switch(type)
       {
       case QtDebugMsg:
           strMsg = QString("Debug:");
           break;
       case QtWarningMsg:
           strMsg = QString("Warning:");
           break;
       case QtCriticalMsg:
           strMsg = QString("Critical:");
           break;
       case QtFatalMsg:
           strMsg = QString("Fatal:");
           break;
       }

       // 设置输出信息格式
       QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss ddd");
       QString strMessage = QString("[%1  File:'%2'  Line:(%3)  Function:'%4']   %5")
               .arg(strDateTime).arg(context.file).arg(context.line).arg(context.function).arg(localMsg.constData());

       // 输出信息至文件中（读写、追加形式）
       QFile file("D://QTProj//ImLab//VisionerLabLog.txt");
       file.open(QIODevice::ReadWrite | QIODevice::Append);
       QTextStream stream(&file);
       stream << strMessage << "\r\n";
       file.flush();
       file.close();

       // 解锁
       mutex.unlock();
}
