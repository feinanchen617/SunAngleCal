#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include<QLineEdit>
#include<QPushButton>
#include<QLabel>
#include<QTimer>
#include<QtCharts>

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void calculateSunAngle();
    void updateCurrentTime();
    void estimateTimeFromElevation();
    void estimateLocationFromAngles();
private:
    QLineEdit *targetAzEdit;
    QLineEdit *dateEdit;
    QLineEdit *timeEdit;
    QLineEdit *latEdit;
    QLineEdit *lonEdit;
    QLineEdit *targetElevEdit;
    QLabel *resultLabel;
    QLabel *clockLabel;
    //    QPolarChart *elevationChart;
    //    QPolarChart *azimuthChart;
    //    QChartView *elevationChartView;
    //    QChartView *azimuthChartView;
    QTimer *timer;
    double calculateSolarElevation(double lat, double lon, QDateTime datetime);
    double calculateSolarAzimuth(double lat, double lon, QDateTime datetime);
};
#endif // MAINWINDOW_H
