#include "memorybar.h"

#include <QEvent>
#include <QHelpEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QToolTip>

namespace Perf
{

// Segment colours (dark-theme)
static const QColor kColUsed  (0xcc, 0x44, 0xcc);        // bright purple — matches graph
static const QColor kColDirty (0xbb, 0x88, 0x00);        // amber
static const QColor kColCached(0x55, 0x22, 0x55);        // muted dark purple
static const QColor kColFree  (0x11, 0x08, 0x11);        // near-background
static const QColor kColBorder(0x88, 0x44, 0x88);        // subtle purple border

MemoryBar::MemoryBar(QWidget *parent) : QWidget(parent)
{
    this->setMinimumHeight(40);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMouseTracking(true);
}

void MemoryBar::SetSegments(qint64 used, qint64 dirty, qint64 cached, qint64 free, qint64 total)
{
    this->m_used   = qMax(0LL, used);
    this->m_dirty  = qMax(0LL, dirty);
    this->m_cached = qMax(0LL, cached);
    this->m_free   = qMax(0LL, free);
    this->m_total  = (total > 0) ? total : 1;
    this->update();
}

void MemoryBar::paintEvent(QPaintEvent * /*event*/)
{
    QPainter p(this);
    const QRect r = this->rect().adjusted(0, 0, -1, -1);   // leave 1px for border
    int wUsed = 0, wDirty = 0, wCached = 0, wFree = 0;
    this->segmentWidths(wUsed, wDirty, wCached, wFree);

    int x = r.left();
    const int y = r.top();
    const int h = r.height();

    auto drawSeg = [&](int sw, const QColor &col)
    {
        if (sw <= 0) return;
        p.fillRect(x, y, sw, h, col);
        x += sw;
    };

    drawSeg(wUsed,   kColUsed);
    drawSeg(wDirty,  kColDirty);
    drawSeg(wCached, kColCached);
    drawSeg(wFree,   kColFree);

    // Border
    p.setPen(QPen(kColBorder, 1));
    p.setBrush(Qt::NoBrush);
    p.drawRect(this->rect().adjusted(0, 0, -1, -1));
}

bool MemoryBar::event(QEvent *event)
{
    if (event->type() == QEvent::ToolTip)
    {
        auto *he = static_cast<QHelpEvent *>(event);
        const Segment seg = this->segmentAtPos(he->pos());
        if (seg == Segment::None)
        {
            QToolTip::hideText();
            event->ignore();
            return true;
        }
        QToolTip::showText(he->globalPos(), this->segmentTooltip(seg), this);
        return true;
    }
    return QWidget::event(event);
}

void MemoryBar::segmentWidths(int &wUsed, int &wDirty, int &wCached, int &wFree) const
{
    const QRect r = this->rect().adjusted(0, 0, -1, -1);
    const double w = static_cast<double>(r.width());
    const double t = static_cast<double>(this->m_total);

    const auto segW = [&](qint64 val) -> int {
        return static_cast<int>(static_cast<double>(val) / t * w + 0.5);
    };

    wUsed   = segW(this->m_used);
    wDirty  = segW(this->m_dirty);
    // Clean cache = full cache − dirty
    wCached = segW(qMax(0LL, this->m_cached - this->m_dirty));
    wFree   = r.width() - wUsed - wDirty - wCached;  // absorbs rounding
}

MemoryBar::Segment MemoryBar::segmentAtPos(const QPoint &pos) const
{
    const QRect r = this->rect().adjusted(0, 0, -1, -1);
    if (!r.contains(pos))
        return Segment::None;

    int wUsed = 0, wDirty = 0, wCached = 0, wFree = 0;
    this->segmentWidths(wUsed, wDirty, wCached, wFree);

    const int x = pos.x() - r.left();
    if (x < wUsed)
        return Segment::Used;
    if (x < wUsed + wDirty)
        return Segment::Dirty;
    if (x < wUsed + wDirty + wCached)
        return Segment::Cached;
    if (x < wUsed + wDirty + wCached + wFree)
        return Segment::Free;
    return Segment::None;
}

QString MemoryBar::formatKb(qint64 kb) const
{
    const double gb = static_cast<double>(kb) / (1024.0 * 1024.0);
    if (gb >= 10.0)
        return QString::number(gb, 'f', 1) + tr(" GB");
    if (gb >= 1.0)
        return QString::number(gb, 'f', 2) + tr(" GB");
    return QString::number(static_cast<double>(kb) / 1024.0, 'f', 1) + tr(" MB");
}

QString MemoryBar::segmentTooltip(Segment seg) const
{
    qint64 value = 0;
    QString label;

    if (seg == Segment::Used)
    {
        value = this->m_used;
        label = tr("Used");
    } else if (seg == Segment::Dirty)
    {
        value = this->m_dirty;
        label = tr("Dirty");
    } else if (seg == Segment::Cached)
    {
        value = qMax(0LL, this->m_cached - this->m_dirty);
        label = tr("Free (cached)");
    } else if (seg == Segment::Free)
    {
        value = this->m_free;
        label = tr("Free");
    }

    const double pct = (this->m_total > 0)
                       ? static_cast<double>(value) * 100.0 / static_cast<double>(this->m_total)
                       : 0.0;
    return tr("%1: %2 (%3%)")
            .arg(label)
            .arg(this->formatKb(value))
            .arg(QString::number(pct, 'f', 1));
}

} // namespace Perf
