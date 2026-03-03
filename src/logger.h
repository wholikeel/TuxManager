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

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QString>

// ── Convenience macros ───────────────────────────────────────────────────────
#define LOG_DEBUG(msg) Logger::instance()->Log(Logger::Debug, QString(msg))
#define LOG_INFO(msg)  Logger::instance()->Log(Logger::Info,  QString(msg))
#define LOG_WARN(msg)  Logger::instance()->Log(Logger::Warn,  QString(msg))
#define LOG_ERROR(msg) Logger::instance()->Log(Logger::Error, QString(msg))

class Logger : public QObject
{
    Q_OBJECT

    public:
        enum Level { Debug = 0, Info, Warn, Error };

        static Logger *instance();

        /// Log a message at the given level.
        void Log(Level level, const QString &message);

        /// Verbosity count (number of -v flags supplied on the command line).
        ///   0 → show INFO, WARN, ERROR  (default)
        ///   1 → also show DEBUG         (-v)
        ///   2+→ reserved for future TRACE levels (-vv / -vvv)
        int Verbosity { 0 };

    private:
        explicit Logger(QObject *parent = nullptr);
        ~Logger() override = default;

        static Logger *s_instance;

        static const char *levelTag(Level level);
};

#endif // LOGGER_H
