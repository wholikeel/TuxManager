#include "service.h"
#include "servicehelper.h"

#include <QRegularExpression>

namespace OS
{

bool Service::isSystemdAvailable(QString *reason)
{
    return ServiceHelper::isSystemdAvailable(reason);
}

QList<Service> Service::loadAll(QString *error)
{
    QList<Service> out;

    QString reason;
    if (!ServiceHelper::isSystemdAvailable(&reason))
    {
        if (error)
            *error = reason;
        return out;
    }

    QList<ServiceHelper::ServiceRecord> rows;
    if (ServiceHelper::listServicesViaSystemdDbus(rows, error))
    {
        out.reserve(rows.size());
        for (const auto &r : rows)
        {
            Service s;
            s.unit = r.unit;
            s.description = r.description;
            s.loadState = r.loadState;
            s.activeState = r.activeState;
            s.subState = r.subState;
            out.append(s);
        }
        if (error)
            error->clear();
        return out;
    }

    // Fallback for environments where sd-bus API isn't available but systemctl
    // is. This still keeps the app functional on more minimal installs.
    QString stdoutText;
    QString stderrText;
    int exitCode = -1;
    const QStringList args {
        "list-units",
        "--type=service",
        "--all",
        "--no-pager",
        "--no-legend",
        "--plain"
    };
    if (!ServiceHelper::runSystemctl(args, stdoutText, stderrText, exitCode) || exitCode != 0)
    {
        if (error)
            *error = stderrText.isEmpty()
                     ? QObject::tr("Unable to query services via sd-bus or systemctl")
                     : stderrText;
        return out;
    }

    const QRegularExpression lineRe("^(\\S+)\\s+(\\S+)\\s+(\\S+)\\s+(\\S+)\\s*(.*)$");
    const QStringList lines = stdoutText.split('\n', Qt::SkipEmptyParts);
    for (const QString &rawLine : lines)
    {
        const QString line = rawLine.trimmed();
        if (line.isEmpty())
            continue;

        const QRegularExpressionMatch m = lineRe.match(line);
        if (!m.hasMatch())
            continue;

        Service s;
        s.unit        = m.captured(1);
        s.loadState   = m.captured(2);
        s.activeState = m.captured(3);
        s.subState    = m.captured(4);
        s.description = m.captured(5).trimmed();
        out.append(s);
    }

    if (error)
        error->clear();
    return out;
}

} // namespace Os
