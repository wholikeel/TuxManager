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

#ifndef OS_PROCESSHELPER_H
#define OS_PROCESSHELPER_H

#include <QString>
#include <sys/types.h>

namespace OS
{
    class ProcessHelper
    {
        public:
            /// Send an arbitrary signal number to a process.
            /// Returns true on success, sets errorMsg on failure.
            static bool sendSignal(pid_t pid, int signal, QString &errorMsg);

            /// Convenience wrappers for common signals.
            static bool kill(pid_t pid, QString &errorMsg);   ///< SIGKILL (9)
            static bool term(pid_t pid, QString &errorMsg);   ///< SIGTERM (15)
            static bool hup(pid_t pid, QString &errorMsg);    ///< SIGHUP  (1)
            static bool stop(pid_t pid, QString &errorMsg);   ///< SIGSTOP (19)
            static bool cont(pid_t pid, QString &errorMsg);   ///< SIGCONT (18)

            /// Change the nice value of a process.
            /// Returns true on success, sets errorMsg on failure.
            static bool renice(pid_t pid, int nice, QString &errorMsg);

            /// Human-readable description of a signal number.
            static QString signalName(int signal);

        private:
            ProcessHelper() = delete;
    };
} // namespace Os

#endif // OS_PROCESSHELPER_H
