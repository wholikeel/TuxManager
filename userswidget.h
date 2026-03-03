#ifndef USERSWIDGET_H
#define USERSWIDGET_H

#include "os/process.h"

#include <QHash>
#include <QSet>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class UsersWidget;
}
QT_END_NAMESPACE

class UsersWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit UsersWidget(QWidget *parent = nullptr);
        ~UsersWidget();
        void setActive(bool active);
        bool isActive() const { return this->m_active; }

    private slots:
        void onTimerTick();
        void onContextMenu(const QPoint &pos);

    private:
        Ui::UsersWidget *ui;
        QTimer          *m_refreshTimer { nullptr };
        bool             m_active { false };
        QHash<pid_t, quint64> m_prevTicks;
        quint64          m_prevCpuTotalTicks { 0 };
        int              m_numCpus { 1 };
        QSet<uid_t>      m_expandedUsers;
        bool             m_hasExpansionSnapshot { false };

        static quint64 readTotalCpuJiffies();
        static QString formatMemory(quint64 kb);
        void rebuildTree(const QList<OS::Process> &allProcs);
};

#endif // USERSWIDGET_H
