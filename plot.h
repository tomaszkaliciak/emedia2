#ifndef PLOT_H
#define PLOT_H

#include <QDialog>
#include <QChartView>
#include <QLineSeries>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class Plot;
}

class Plot : public QChartView
{
    Q_OBJECT
public:
    explicit Plot(QChartView *parent = 0);
    Plot(QChart *plt, QChartView *parent = 0);

    ~Plot();

private:
    Ui::Plot *ui;
};

#endif // PLOT_H
