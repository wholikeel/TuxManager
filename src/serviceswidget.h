#ifndef SERVICESWIDGET_H
#define SERVICESWIDGET_H

#include "os/service.h"

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ServicesWidget;
}
QT_END_NAMESPACE

class ServiceRefreshWorker : public QObject
{
    Q_OBJECT

    public slots:
        void fetch(quint64 token);

    signals:
        void fetched(quint64 token,
                    bool systemdAvailable,
                    const QString &reason,
                    const QList<OS::Service> &services,
                    const QString &error);
};

class ServicesWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit ServicesWidget(QWidget *parent = nullptr);
        ~ServicesWidget();
        void setActive(bool active);
        bool isActive() const { return this->m_active; }

    private slots:
        void onTimerTick();
        void onRefreshFinished(quint64 token,
                               bool systemdAvailable,
                               const QString &reason,
                               const QList<OS::Service> &services,
                               const QString &error);

    signals:
        void requestRefresh(quint64 token);

    private:
        Ui::ServicesWidget *ui;
        QTimer             *m_refreshTimer { nullptr };
        QThread            *m_workerThread { nullptr };
        ServiceRefreshWorker *m_worker { nullptr };
        bool                m_active { false };
        bool                m_refreshInFlight { false };
        bool                m_refreshPending { false };
        quint64             m_refreshToken { 0 };

        void rebuildTable(const QList<OS::Service> &services);
        void startRefresh();
};

#endif // SERVICESWIDGET_H
