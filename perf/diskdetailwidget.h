#ifndef PERF_DISKDETAILWIDGET_H
#define PERF_DISKDETAILWIDGET_H

#include "perfdataprovider.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class DiskDetailWidget; }
QT_END_NAMESPACE

namespace Perf
{

class DiskDetailWidget : public QWidget
{
    Q_OBJECT

    public:
        explicit DiskDetailWidget(QWidget *parent = nullptr);
        ~DiskDetailWidget();

        void setProvider(PerfDataProvider *provider);
        void setDiskIndex(int index);

    private slots:
        void onUpdated();

    private:
        static QString formatRate(double bytesPerSec);

        Ui::DiskDetailWidget *ui;
        PerfDataProvider     *m_provider { nullptr };
        int                   m_diskIndex { -1 };
};

} // namespace Perf

#endif // PERF_DISKDETAILWIDGET_H
