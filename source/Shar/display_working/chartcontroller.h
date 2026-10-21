#ifndef CHARTCONTROLLER_H
#define CHARTCONTROLLER_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QtCharts/QLineSeries>

QT_BEGIN_NAMESPACE
class QLineSeries;
QT_END_NAMESPACE

/**
 * для смены градиента оси X
 *
 В любом месте C++, где у вас есть указатель на ChartController (например, в main.cpp)
controller.setXStep(0.01);   // 1 точка = 0.01, 10 точек = 0.1
controller.setXStep(0.1);    // 1 точка = 0.1,  10 точек = 1.0
controller.setXStep(0.5);    // 1 точка = 0.5,  10 точек = 5.0
controller.setXStep(1.0);    // 1 точка = 1.0,  10 точек = 10.0

Что произойдёт:
m_xStep обновится, emit xStepChanged().
rebuildAllSeries() пересчитает все точки всех 9 серий: x = index * newStep, применит через replace().
Видимый диапазон X пропорционально масштабируется (ratio = newStep / oldStep), чтобы график визуально не «прыгал».
Если включён autoFollow и не ручной режим — updateXRange() подгонит окно под последние точки.
 */

class ChartController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(qreal xMin READ xMin NOTIFY xRangeChanged)
    Q_PROPERTY(qreal xMax READ xMax NOTIFY xRangeChanged)
    Q_PROPERTY(qreal yMin READ yMin NOTIFY yRangeChanged)
    Q_PROPERTY(qreal yMax READ yMax NOTIFY yRangeChanged)

    Q_PROPERTY(bool autoFollow READ autoFollow WRITE setAutoFollow NOTIFY autoFollowChanged)
    Q_PROPERTY(int visiblePoints READ visiblePoints WRITE setVisiblePoints NOTIFY visiblePointsChanged)
    Q_PROPERTY(int maxPoints READ maxPoints WRITE setMaxPoints NOTIFY maxPointsChanged)

    // Шаг по X: 1 точка = xStep единиц времени.
    // Управляется ТОЛЬКО из C++ (setXStep). QML в него не пишет.
    Q_PROPERTY(qreal xStep READ xStep NOTIFY xStepChanged)

public:
    explicit ChartController(QObject *parent = nullptr);
    ~ChartController();

    qreal xMin() const { return m_xMin; }
    qreal xMax() const { return m_xMax; }
    qreal yMin() const { return m_yMin; }
    qreal yMax() const { return m_yMax; }

    bool autoFollow() const { return m_autoFollow; }
    void setAutoFollow(bool on);

    int visiblePoints() const { return m_visiblePoints; }
    void setVisiblePoints(int n);

    int maxPoints() const { return m_maxPoints; }
    void setMaxPoints(int n);

    qreal xStep() const { return m_xStep; }

    // Публичный сеттер шага X — вызывать из C++.
    // QML его не вызывает (нет Q_INVOKABLE и нет WRITE в Q_PROPERTY).
    void setXStep(qreal step);

    Q_INVOKABLE void pushData(int channel, double value);
    Q_INVOKABLE void registerSeries(QLineSeries *series, int channel);
    Q_INVOKABLE void clear();
    Q_INVOKABLE int pointCount(int channel) const;
    Q_INVOKABLE void notifyUserInteraction();
    Q_INVOKABLE void setChannelEnabled(int channel, bool enabled);
    Q_INVOKABLE bool isChannelEnabled(int channel) const;

    Q_INVOKABLE void zoomX(qreal factor, qreal centerRatio);
    Q_INVOKABLE void zoomY(qreal factor, qreal centerRatio);
    Q_INVOKABLE void zoomBoth(qreal factor);
    Q_INVOKABLE void panByPixels(qreal dxPix, qreal dyPix,
                                 qreal plotWidth, qreal plotHeight);
    Q_INVOKABLE void resetToDefault();
    Q_INVOKABLE void autoScaleAll();

signals:
    void yRangeChanged();
    void maxPointsChanged();
    void autoFollowChanged();
    void visiblePointsChanged();
    void xRangeChanged();
    void xStepChanged();

private:
    static constexpr int CHANNEL_COUNT = 9;

    QList<QLineSeries*>       m_seriesList;
    QVector<QVector<QPointF>> m_points;   // хранит ИНДЕКСЫ: x = 0,1,2,...
    QVector<qreal>            m_lastY;
    QVector<bool>             m_channelEnabled;

    qreal m_yMin = 0.0;
    qreal m_yMax = 1.0;
    qreal m_xCounter = 0.0;      // следующий индекс точки
    int   m_maxPoints = 0;       // 0 = «Все»
    bool  m_autoFollow = true;
    int   m_visiblePoints = 200;
    qreal m_xMin = 0.0;
    qreal m_xMax = 20.0;         // 200 * 0.1
    qreal m_xStep = 0.1;         // 1 точка = 0.1

    bool m_manualX = false;
    bool m_manualY = false;
    bool m_updatingAutoFollow = false;

    void recomputeYRange();
    void applyWindow(int channel);
    void updateXRange();
    void rebuildAllSeries();
    QVector<QPointF> scaledBuffer(int channel) const;
};

#endif // CHARTCONTROLLER_H
