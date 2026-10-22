#include "chartcontroller.h"
#include <QtCharts/QLineSeries>
#include <algorithm>
#include <limits>

ChartController::ChartController(QObject *parent)
    : QObject(parent)
    , m_seriesList(CHANNEL_COUNT, nullptr)
    , m_points(CHANNEL_COUNT)
    , m_lastY(CHANNEL_COUNT, 0.0)
    , m_channelEnabled(CHANNEL_COUNT, true)
{
    m_xMin = 0.0;
    m_xMax = m_visiblePoints * m_xStep;
}

ChartController::~ChartController() = default;

// ---------------------------------------------------------------------------
// Вспомогательное: буфер канала, пересчитанный в единицы времени
// ---------------------------------------------------------------------------

QVector<QPointF> ChartController::scaledBuffer(int channel) const
{
    const QVector<QPointF> &buf = m_points[channel];
    QVector<QPointF> out;
    out.reserve(buf.size());
    for (const QPointF &pt : buf)
        out.append(QPointF(pt.x() * m_xStep, pt.y()));
    return out;
}

// ---------------------------------------------------------------------------
// Свойства
// ---------------------------------------------------------------------------

void ChartController::setMaxPoints(int n)
{
    if (n < 0) n = 0;
    if (m_maxPoints == n) return;
    m_maxPoints = n;
    emit maxPointsChanged();
    for (int ch = 0; ch < CHANNEL_COUNT; ++ch)
        applyWindow(ch);
    if (!m_manualX) updateXRange();
}

void ChartController::setVisiblePoints(int n)
{
    if (n < 1) n = 1;
    if (m_visiblePoints == n) return;
    m_visiblePoints = n;
    emit visiblePointsChanged();
    if (m_autoFollow && !m_manualX) updateXRange();
}

// ---------------------------------------------------------------------------
// Установка шага X. Вызывать из C++. QML этот метод не вызывает.
// ---------------------------------------------------------------------------

void ChartController::setXStep(qreal step)
{
    if (step <= 0) return;
    step = step / 1000; // меняю в qml, а там значения в мс

    if (qFuzzyCompare(step, m_xStep)) return;

    const qreal oldStep = m_xStep;
    m_xStep = step;
    emit xStepChanged();

    // Пересчитать все серии: x = index * newStep
    rebuildAllSeries();

    // Пропорционально пересчитать видимый диапазон,
    // чтобы визуально график не «прыгал».
    const qreal ratio = step / oldStep;
    m_xMin *= ratio;
    m_xMax *= ratio;
    emit xRangeChanged();

    if (m_autoFollow && !m_manualX)
        updateXRange();
}

void ChartController::setAutoFollow(bool on)
{
    if (m_updatingAutoFollow) return;
    if (m_autoFollow == on) return;

    m_updatingAutoFollow = true;
    m_autoFollow = on;
    emit autoFollowChanged();

    if (m_autoFollow) {
        m_manualX = false;
        m_manualY = false;
        updateXRange();
        recomputeYRange();
        emit xRangeChanged();
        emit yRangeChanged();
    }
    m_updatingAutoFollow = false;
}

// ---------------------------------------------------------------------------
// Регистрация серий
// ---------------------------------------------------------------------------

void ChartController::registerSeries(QLineSeries *series, int channel)
{
    if (channel < 0 || channel >= CHANNEL_COUNT) return;
    m_seriesList[channel] = series;
}

// ---------------------------------------------------------------------------
// Включение/выключение приёма данных
// ---------------------------------------------------------------------------

void ChartController::setChannelEnabled(int channel, bool enabled)
{
    if (channel < 0 || channel >= CHANNEL_COUNT) return;
    if (m_channelEnabled[channel] == enabled) return;
    m_channelEnabled[channel] = enabled;
}

bool ChartController::isChannelEnabled(int channel) const
{
    if (channel < 0 || channel >= CHANNEL_COUNT) return false;
    return m_channelEnabled[channel];
}

// ---------------------------------------------------------------------------
// Скользящее окно
// ---------------------------------------------------------------------------

void ChartController::applyWindow(int channel)
{
    if (channel < 0 || channel >= CHANNEL_COUNT) return;
    QLineSeries *series = m_seriesList[channel];
    if (!series) return;

    QVector<QPointF> &buf = m_points[channel];
    if (m_maxPoints > 0 && buf.size() > m_maxPoints)
        buf.remove(0, buf.size() - m_maxPoints);
    series->replace(scaledBuffer(channel));
}

// ---------------------------------------------------------------------------
// Приём данных
// ---------------------------------------------------------------------------

void ChartController::pushData(int channel, double value)
{
    if (channel < 0 || channel >= CHANNEL_COUNT) return;
    if (!m_channelEnabled[channel]) return;
    QLineSeries *series = m_seriesList[channel];
    if (!series) return;

    QVector<QPointF> &buf = m_points[channel];
    if (m_maxPoints > 0 && buf.size() >= m_maxPoints)
        buf.removeFirst();

    // В буфере — индекс точки (0,1,2,...)
    buf.append(QPointF(m_xCounter, value));
    series->replace(scaledBuffer(channel));

    m_lastY[channel] = value;
    ++m_xCounter;

    recomputeYRange();
    if (m_autoFollow && !m_manualX)
        updateXRange();
}

// ---------------------------------------------------------------------------
// Очистка
// ---------------------------------------------------------------------------

void ChartController::clear()
{
    for (int ch = 0; ch < CHANNEL_COUNT; ++ch) {
        m_points[ch].clear();
        if (m_seriesList[ch])
            m_seriesList[ch]->clear();
    }
    m_xCounter = 0.0;
    resetToDefault();
}

int ChartController::pointCount(int channel) const
{
    if (channel < 0 || channel >= CHANNEL_COUNT) return 0;
    return m_points[channel].size();
}

void ChartController::notifyUserInteraction()
{
    setAutoFollow(false);
}

// ---------------------------------------------------------------------------
// Ручное управление диапазонами
// ---------------------------------------------------------------------------

void ChartController::zoomX(qreal factor, qreal centerRatio)
{
    if (factor <= 0) return;
    if (centerRatio < 0) centerRatio = 0;
    if (centerRatio > 1) centerRatio = 1;

    if (m_autoFollow) { m_autoFollow = false; emit autoFollowChanged(); }
    m_manualX = true; m_manualY = true;

    const qreal range    = m_xMax - m_xMin;
    const qreal center   = m_xMin + centerRatio * range;
    const qreal newRange = range * factor;

    m_xMin = center - centerRatio * newRange;
    m_xMax = m_xMin + newRange;
    emit xRangeChanged();
}

void ChartController::zoomY(qreal factor, qreal centerRatio)
{
    if (factor <= 0) return;
    if (centerRatio < 0) centerRatio = 0;
    if (centerRatio > 1) centerRatio = 1;

    if (m_autoFollow) { m_autoFollow = false; emit autoFollowChanged(); }
    m_manualX = true; m_manualY = true;

    const qreal range    = m_yMax - m_yMin;
    const qreal center   = m_yMin + centerRatio * range;
    const qreal newRange = range * factor;

    m_yMin = center - centerRatio * newRange;
    m_yMax = m_yMin + newRange;
    emit yRangeChanged();
}

void ChartController::zoomBoth(qreal factor)
{
    if (factor <= 0) return;

    if (m_autoFollow) { m_autoFollow = false; emit autoFollowChanged(); }
    m_manualX = true; m_manualY = true;

    const qreal cx = (m_xMin + m_xMax) / 2;
    const qreal cy = (m_yMin + m_yMax) / 2;
    const qreal xr = (m_xMax - m_xMin) * factor;
    const qreal yr = (m_yMax - m_yMin) * factor;

    m_xMin = cx - xr / 2;
    m_xMax = cx + xr / 2;
    m_yMin = cy - yr / 2;
    m_yMax = cy + yr / 2;

    emit xRangeChanged();
    emit yRangeChanged();
}

void ChartController::panByPixels(qreal dxPix, qreal dyPix,
                                  qreal plotWidth, qreal plotHeight)
{
    if (plotWidth <= 0 || plotHeight <= 0) return;

    if (m_autoFollow) { m_autoFollow = false; emit autoFollowChanged(); }
    m_manualX = true; m_manualY = true;

    const qreal xRange = m_xMax - m_xMin;
    const qreal yRange = m_yMax - m_yMin;

    const qreal dxData = dxPix / plotWidth  * xRange;
    const qreal dyData = dyPix / plotHeight * yRange;

    m_xMin -= dxData;
    m_xMax -= dxData;
    m_yMin += dyData;
    m_yMax += dyData;

    emit xRangeChanged();
    emit yRangeChanged();
}

void ChartController::resetToDefault()
{
    m_autoFollow = true;
    m_manualX = false;
    m_manualY = false;
    emit autoFollowChanged();

    m_xMin = 0.0;
    m_xMax = m_visiblePoints * m_xStep;
    emit xRangeChanged();

    m_yMin = 0.0;
    m_yMax = 1.0;
    emit yRangeChanged();
}

void ChartController::autoScaleAll()
{
    m_autoFollow = true;
    m_manualX = false;
    m_manualY = false;
    emit autoFollowChanged();

    const qreal windowLen = m_visiblePoints * m_xStep;
    const qreal lastTime  = (m_xCounter > 0) ? (m_xCounter - 1) * m_xStep : 0.0;

    if (lastTime <= windowLen) {
        m_xMin = 0.0;
        m_xMax = windowLen;
    } else {
        m_xMax = lastTime;
        m_xMin = m_xMax - windowLen;
    }
    emit xRangeChanged();

    recomputeYRange();
    emit yRangeChanged();
}

// ---------------------------------------------------------------------------
// Внутренние пересчёты
// ---------------------------------------------------------------------------

void ChartController::updateXRange()
{
    if (m_manualX) return;

    const qreal windowLen = m_visiblePoints * m_xStep;
    const qreal lastTime  = (m_xCounter > 0) ? (m_xCounter - 1) * m_xStep : 0.0;

    qreal newXMin, newXMax;
    if (lastTime <= windowLen) {
        newXMin = 0.0;
        newXMax = windowLen;
    } else {
        newXMax = lastTime;
        newXMin = newXMax - windowLen;
    }

    if (qFuzzyCompare(newXMin, m_xMin) && qFuzzyCompare(newXMax, m_xMax))
        return;
    m_xMin = newXMin;
    m_xMax = newXMax;
    emit xRangeChanged();
}

void ChartController::recomputeYRange()
{
    if (m_manualY) return;

    qreal lo = std::numeric_limits<qreal>::max();
    qreal hi = std::numeric_limits<qreal>::lowest();
    bool hasData = false;

    for (int ch = 0; ch < CHANNEL_COUNT; ++ch) {
        if (!m_channelEnabled[ch]) continue;
        const auto &buf = m_points[ch];
        for (const QPointF &pt : buf) {
            lo = std::min(lo, pt.y());
            hi = std::max(hi, pt.y());
            hasData = true;
        }
    }

    if (!hasData) { lo = 0.0; hi = 1.0; }
    if (qFuzzyCompare(lo, m_yMin) && qFuzzyCompare(hi, m_yMax))
        return;
    m_yMin = lo;
    m_yMax = hi;
    emit yRangeChanged();
}

void ChartController::rebuildAllSeries()
{
    for (int ch = 0; ch < CHANNEL_COUNT; ++ch) {
        QLineSeries *series = m_seriesList[ch];
        if (!series) continue;
        series->replace(scaledBuffer(ch));
    }
}
