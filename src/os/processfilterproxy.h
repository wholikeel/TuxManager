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

#ifndef OS_PROCESSFILTERPROXY_H
#define OS_PROCESSFILTERPROXY_H

#include "processmodel.h"

#include <QSortFilterProxyModel>
#include <sys/types.h>

namespace OS
{
    /// Extended sort/filter proxy for the process table.
    ///
    /// Combines free-text search (inherited from QSortFilterProxyModel) with
    /// two additional visibility toggles:
    ///   - ShowKernelTasks      – show/hide kernel threads
    ///   - ShowOtherUsersProcs  – show/hide processes owned by other UIDs
    ///
    /// Kernel tasks are identified by a name enclosed in brackets, e.g. [kworker].
    class ProcessFilterProxy : public QSortFilterProxyModel
    {
        Q_OBJECT

        public:
            explicit ProcessFilterProxy(QObject *parent = nullptr);

            bool ShowKernelTasks     { true };
            bool ShowOtherUsersProcs { true };

            /// Call after toggling either flag to re-run the filter.
            void ApplyFilters();

        protected:
            bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

        private:
            uid_t m_myUid;
            static bool isKernelTask(const Process &proc);
    };
} // namespace Os

#endif // OS_PROCESSFILTERPROXY_H
