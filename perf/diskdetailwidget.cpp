#include "diskdetailwidget.h"
#include "ui_diskdetailwidget.h"

namespace Perf
{

DiskDetailWidget::DiskDetailWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DiskDetailWidget)
{
    this->ui->setupUi(this);

    // Disk graph: green like Task Manager's disk panel
    this->ui->graphWidget->setColor(QColor(0x66, 0xbb, 0x44),
                                    QColor(0x33, 0x66, 0x22, 120));
    this->ui->graphWidget->setSampleCapacity(HISTORY_SIZE);
    this->ui->graphWidget->setGridColumns(6);
    this->ui->graphWidget->setGridRows(4);
}

DiskDetailWidget::~DiskDetailWidget()
{
    delete this->ui;
}

void DiskDetailWidget::setProvider(PerfDataProvider *provider)
{
    if (this->m_provider)
        disconnect(this->m_provider, &PerfDataProvider::updated,
                   this, &DiskDetailWidget::onUpdated);

    this->m_provider = provider;

    if (this->m_provider)
    {
        connect(this->m_provider, &PerfDataProvider::updated,
                this, &DiskDetailWidget::onUpdated);
        this->onUpdated();
    }
}

void DiskDetailWidget::setDiskIndex(int index)
{
    this->m_diskIndex = index;
    this->onUpdated();
}

void DiskDetailWidget::onUpdated()
{
    if (!this->m_provider || this->m_diskIndex < 0 || this->m_diskIndex >= this->m_provider->diskCount())
        return;

    const QString name = this->m_provider->diskName(this->m_diskIndex);
    const QString model = this->m_provider->diskModel(this->m_diskIndex);
    const QString type = this->m_provider->diskType(this->m_diskIndex);
    const double active = this->m_provider->diskActivePercent(this->m_diskIndex);
    const double readBps = this->m_provider->diskReadBytesPerSec(this->m_diskIndex);
    const double writeBps = this->m_provider->diskWriteBytesPerSec(this->m_diskIndex);

    this->ui->titleLabel->setText(tr("Disk (%1)").arg(name));
    this->ui->modelLabel->setText(model);

    this->ui->activeValueLabel->setText(QString::number(active, 'f', 0) + "%");
    this->ui->readValueLabel->setText(formatRate(readBps));
    this->ui->writeValueLabel->setText(formatRate(writeBps));
    this->ui->typeValueLabel->setText(type);
    this->ui->deviceValueLabel->setText("/dev/" + name);

    this->ui->graphWidget->setHistory(this->m_provider->diskActiveHistory(this->m_diskIndex));
}

QString DiskDetailWidget::formatRate(double bytesPerSec)
{
    if (bytesPerSec >= 1024.0 * 1024.0)
        return QString::number(bytesPerSec / (1024.0 * 1024.0), 'f', 1) + tr(" MB/s");
    return QString::number(bytesPerSec / 1024.0, 'f', 0) + tr(" KB/s");
}

} // namespace Perf
