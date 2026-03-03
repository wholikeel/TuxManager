/*
 * Tux Manager - Linux system monitor
 * Copyright (C) 2026 Petr Bena <petr@bena.rocks>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "logger.h"

#include <QDateTime>
#include <QTextStream>

Logger *Logger::s_instance = nullptr;

Logger *Logger::instance()
{
    if (!s_instance)
        s_instance = new Logger();
    return s_instance;
}

Logger::Logger(QObject *parent) : QObject(parent)
{}

const char *Logger::levelTag(Level level)
{
    switch (level)
    {
        case Debug:
            return "DEBUG";
        case Info:
            return "INFO ";
        case Warn:
            return "WARN ";
        case Error:
            return "ERROR";
    }
    return "?????";
}

void Logger::Log(Level level, const QString &message)
{
    // Filter based on Verbosity:
    //   Verbosity 0 → show Info and above (suppress Debug)
    //   Verbosity 1+ → show everything
    const Level minLevel = (this->Verbosity >= 1) ? Debug : Info;
    if (level < minLevel)
        return;

    const QString timestamp = QDateTime::currentDateTime().toString("[yyyy-MM-dd HH:mm:ss.zzz]");
    const QString line = QString("%1 %2: %3\n").arg(timestamp, levelTag(level), message);

    // WARN and ERROR stderr, everything else goes to stdout
    if (level >= Warn)
    {
        QTextStream err(stderr);
        err << line;
        err.flush();
    } else
    {
        QTextStream out(stdout);
        out << line;
        out.flush();
    }
}
