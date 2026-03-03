#ifndef OS_SERVICEHELPER_H
#define OS_SERVICEHELPER_H

#include <QList>
#include <QString>
#include <QStringList>

namespace OS
{
    class ServiceHelper
    {
        public:
            struct ServiceRecord
            {
                QString unit;
                QString description;
                QString loadState;
                QString activeState;
                QString subState;
            };

            static bool isSystemdAvailable(QString *reason = nullptr);
            static bool runSystemctl(const QStringList &args,
                                     QString          &stdoutText,
                                     QString          &stderrText,
                                     int              &exitCode,
                                     int               timeoutMs = 8000);
            static bool listServicesViaSystemdDbus(QList<ServiceRecord> &records,
                                                   QString              *error = nullptr);

        private:
            ServiceHelper() = delete;
    };
} // namespace Os

#endif // OS_SERVICEHELPER_H
