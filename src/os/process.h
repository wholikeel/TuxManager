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

#ifndef OS_PROCESS_H
#define OS_PROCESS_H

#include <QList>
#include <QString>
#include <sys/types.h>

namespace OS
{

/// Snapshot of a single Linux process read from /proc.
class Process
{
    public:
        struct LoadOptions
        {
            bool  includeKernelTasks  { true };
            bool  includeOtherUsers   { true };
            uid_t myUid               { 0 };
        };

        pid_t   pid           { 0 };
        pid_t   ppid          { 0 };
        QString name;                     ///< Short name  (/proc/pid/comm)
        QString cmdline;                  ///< Full command (/proc/pid/cmdline)
        char    state         { '?' };    ///< Raw state char: R S D Z T I ...
        uid_t   uid           { 0 };
        QString user;                     ///< Resolved username
        int     priority      { 0 };
        int     nice          { 0 };
        int     threads       { 1 };
        quint64 vmRssKb       { 0 };      ///< Resident set size in KiB
        quint64 vmSizeKb      { 0 };      ///< Virtual memory size in KiB
        quint64 cpuTicks      { 0 };      ///< utime + stime in jiffies (for delta CPU%)
        double  cpuPercent    { 0.0 };    ///< Calculated externally after two samples
        quint64 startTimeTicks{ 0 };      ///< Start time in jiffies since boot
        bool    isKernelThread{ false };   ///< True when PF_KTHREAD flag is set in /proc/pid/stat flags field

        /// Load a snapshot of every running process from /proc.
        static QList<Process> loadAll();
        static QList<Process> loadAll(const LoadOptions &options);

        /// Human-readable description of a raw state character.
        static QString stateString(char state);

    private:
        static bool loadOneStatAndUid(pid_t pid, Process &out);
        static void loadUserAndCmdline(Process &proc);
};

} // namespace Os

#endif // OS_PROCESS_H
