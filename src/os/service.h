#ifndef OS_SERVICE_H
#define OS_SERVICE_H

#include <QList>
#include <QMetaType>
#include <QString>

namespace OS
{
    class Service
    {
        public:
            QString unit;         ///< e.g. ssh.service
            QString loadState;    ///< loaded/not-found/...
            QString activeState;  ///< active/inactive/failed/...
            QString subState;     ///< running/exited/dead/...
            QString description;

            static bool isSystemdAvailable(QString *reason = nullptr);
            static QList<Service> loadAll(QString *error = nullptr);
    };
} // namespace Os

Q_DECLARE_METATYPE(OS::Service)
Q_DECLARE_METATYPE(QList<OS::Service>)

#endif // OS_SERVICE_H
