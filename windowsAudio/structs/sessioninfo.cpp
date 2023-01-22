#include "sessioninfo.h"
#include <QDebug>
#include <QTextStream>

QString printSessionInfo(SessionInfo &info)
{
    QString s;
    QTextStream str(&s);

    str << "DisplayName: " << info.DisplayName << "   \r\n   ";
    str << "groupingParam: " << info.groupingParam.Data1 << " "
            << info.groupingParam.Data2 << " "
            << info.groupingParam.Data3 << " "
            << info.groupingParam.Data4 << "   \r\n   ";
    str << "IconPath: " << info.IconPath << "   \r\n   ";
    str << "state: " << info.state << "   \r\n   ";
    str << "processId: " << info.processId << "   \r\n   ";
    str << "sessionIdentifier: " << info.sessionIdentifier << "   \r\n   ";
    str << "sessionInstanceIdentifier: " << info.sessionInstanceIdentifier << "   \r\n   ";
    str << "isSystemSound: " << info.isSystemSound << "   \r\n   ";

    str << "processIconPath: " << info.processIconPath << "   \r\n   ";
    str << "processName: " << info.processName << "   \r\n   ";
    str << "peakVolume: " << info.peakVolume << "   \r\n   ";

    str << "simpleVolume: " << info.simpleVolume << "   \r\n   ";
    str << "isMute: " << info.isMute << "   \r\n   ";

    qInfo() << s;

    return s;
}
