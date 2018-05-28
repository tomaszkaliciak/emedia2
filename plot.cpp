#include "plot.h"
#include "ui_plot.h"

Plot::Plot(QChartView *parent) :
    QChartView(parent),
    ui(new Ui::Plot)
{
    ui->setupUi(this);
}

Plot::Plot(QChart* plt,QChartView *parent) :
    QChartView(parent),
    ui(new Ui::Plot)
{
    ui->setupUi(this);
    p = plt;
    setChart(plt);
}


Plot::~Plot()
{
    delete ui;
    delete p;
}
