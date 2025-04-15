// mainwindow.cpp
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtMath>
#include <QDateTime>
#include <QTimer>
#include <QChartView>
#include <QPolarChart>
#include <QValueAxis>
#include <QScatterSeries>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QGridLayout *layout = new QGridLayout(central);

    QDateTime now = QDateTime::currentDateTimeUtc();
    dateEdit = new QLineEdit(now.date().toString("yyyy-MM-dd"));
    timeEdit = new QLineEdit(now.time().toString("hh:mm:ss"));
    latEdit = new QLineEdit("31.85");
    lonEdit = new QLineEdit("117.25");
    resultLabel = new QLabel("太阳角尚未计算");
    clockLabel = new QLabel("当前系统时间(UTC)：");
    targetElevEdit = new QLineEdit("45.0");
    QPushButton *calcButton = new QPushButton("计算太阳角");
   QPushButton *estTimeButton = new QPushButton("反推时间");
   QPushButton  *estLocationButton = new QPushButton("反推经纬度");
   //addWidget(widget, row, column, rowSpan, columnSpan)
    layout->addWidget(clockLabel,0, 0,1,2);
    layout->addWidget(new QLabel("📅 UTC日期 (yyyy-MM-dd):"),1, 0);
    layout->addWidget(dateEdit,1, 1);
    layout->addWidget(new QLabel("🕒UTC 时间 (hh:mm:ss):"),2,0);
    layout->addWidget(timeEdit,2,1);
    layout->addWidget(new QLabel("纬度 (°):"),3,0);
    layout->addWidget(latEdit,3, 1);
    layout->addWidget(new QLabel("经度 (°):"),4, 0);
    layout->addWidget(lonEdit, 4, 1);
    layout->addWidget(new QLabel("目标太阳高度角 (°):"),5, 0);
     layout->addWidget(targetElevEdit, 5, 1);
     layout->addWidget(new QLabel("目标太阳方位角 (°)（可选）:"), 6, 0);
     targetAzEdit = new QLineEdit();
     layout->addWidget(targetAzEdit, 6, 1);
    layout->addWidget(calcButton, 7, 0);
      layout->addWidget(estLocationButton, 7, 1);
    layout->addWidget(estTimeButton, 8, 0, 1, 2);
    layout->addWidget(resultLabel, 9, 0, 1, 2);

    // 图表初始化
    connect(calcButton, &QPushButton::clicked, this, &MainWindow::calculateSunAngle);
    connect(estTimeButton, &QPushButton::clicked, this, &MainWindow::estimateTimeFromElevation);
    connect(estLocationButton, &QPushButton::clicked, this, &MainWindow::estimateLocationFromAngles);
    setCentralWidget(central);

    // 定时器更新时间
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCurrentTime);
    timer->start(1000);
    updateCurrentTime();
}

MainWindow::~MainWindow() {}

void MainWindow::estimateLocationFromAngles()
{
    QDate date = QDate::fromString(dateEdit->text(), "yyyy-MM-dd");
    QTime time = QTime::fromString(timeEdit->text(), "hh:mm:ss");
    QDateTime datetime(date, time, Qt::UTC);
    double targetElev = targetElevEdit->text().toDouble();
    double targetAz = targetAzEdit->text().toDouble();

    double bestLat = 0.0, bestLon = 0.0;
    double minDiff = 999;

    for (double lat = -90.0; lat <= 90.0; lat += 0.5) {
        for (double lon = -180.0; lon <= 180.0; lon += 0.5) {
            double elev = calculateSolarElevation(lat, lon, datetime);
            double az = calculateSolarAzimuth(lat, lon, datetime);
            double diff = qAbs(elev - targetElev) + qAbs(az - targetAz);
            if (diff < minDiff) {
                minDiff = diff;
                bestLat = lat;
                bestLon = lon;
            }
        }
    }

    resultLabel->setText(QString("根据输入时间与太阳角度，预测经纬度为：纬度 %1°, 经度 %2°")
                         .arg(bestLat, 0, 'f', 2).arg(bestLon, 0, 'f', 2));
}
void MainWindow::updateCurrentTime()
{
    QDateTime now = QDateTime::currentDateTimeUtc();
    clockLabel->setText("当前系统时间：" + now.toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::calculateSunAngle()
{
    QDate date = QDate::fromString(dateEdit->text(), "yyyy-MM-dd");
    QTime time = QTime::fromString(timeEdit->text(), "hh:mm:ss");
    QDateTime datetime(date, time, Qt::UTC);
    double lat = latEdit->text().toDouble();
    double lon = lonEdit->text().toDouble();

    double elevation = calculateSolarElevation(lat, lon, datetime);
    double azimuth = calculateSolarAzimuth(lat, lon, datetime);

    resultLabel->setText(QString("太阳高度角: %1°\n太阳方位角: %2°").arg(elevation, 0, 'f', 2).arg(azimuth, 0, 'f', 2));
}

double MainWindow::calculateSolarElevation(double lat, double lon, QDateTime datetime)
{
    // 基于 NOAA SPA 精度模型的近似实现
       double jd = datetime.toSecsSinceEpoch() / 86400.0 + 2440587.5;
       double d = jd - 2451545.0;

       // 太阳几何参数计算
       double g = qDegreesToRadians(fmod(357.529 + 0.98560028 * d, 360.0)); // 平近点角
       double q = fmod(280.459 + 0.98564736 * d, 360.0);                    // 平黄经
       double L = fmod(q + 1.915 * qSin(g) + 0.020 * qSin(2 * g), 360.0);   // 真黄经
       double e = 23.439 - 0.00000036 * d;                                  // 黄赤交角

       // 赤纬角 declination
       double decl = qAsin(qSin(qDegreesToRadians(e)) * qSin(qDegreesToRadians(L)));

       // 计算地方真太阳时 (LST)，考虑经度
       double utcHours = datetime.time().hour() + datetime.time().minute() / 60.0 + datetime.time().second() / 3600.0;
       double timeOffset = lon / 15.0;  // 每15°相差1小时
       double LST = utcHours + timeOffset;

       // 太阳时角 H
       double H = qDegreesToRadians(15.0 * (LST - 12.0));

       // 太阳高度角 Elevation
       double elev = qAsin(qSin(qDegreesToRadians(lat)) * qSin(decl) +
                           qCos(qDegreesToRadians(lat)) * qCos(decl) * qCos(H));
       return qRadiansToDegrees(elev);
}

double MainWindow::calculateSolarAzimuth(double lat, double lon, QDateTime datetime)
{
    // 同样重用 NOAA SPA 参数
      double jd = datetime.toSecsSinceEpoch() / 86400.0 + 2440587.5;
      double d = jd - 2451545.0;

      double g = qDegreesToRadians(fmod(357.529 + 0.98560028 * d, 360.0));
      double q = fmod(280.459 + 0.98564736 * d, 360.0);
      double L = fmod(q + 1.915 * qSin(g) + 0.020 * qSin(2 * g), 360.0);
      double e = 23.439 - 0.00000036 * d;
      double decl = qAsin(qSin(qDegreesToRadians(e)) * qSin(qDegreesToRadians(L)));

      double utcHours = datetime.time().hour() + datetime.time().minute() / 60.0 + datetime.time().second() / 3600.0;
      double timeOffset = lon / 15.0;
      double LST = utcHours + timeOffset;

      double H = qDegreesToRadians(15.0 * (LST - 12.0));

      // 方位角 Azimuth（以北为0度，顺时针）
      double latRad = qDegreesToRadians(lat);
      double elev = qAsin(qSin(latRad) * qSin(decl) + qCos(latRad) * qCos(decl) * qCos(H));
      double az = qAcos((qSin(decl) - qSin(elev) * qSin(latRad)) / (qCos(elev) * qCos(latRad)));
      if (qSin(H) > 0)
            az = 2 * M_PI - az;
        return qRadiansToDegrees(az);
}


void MainWindow::estimateTimeFromElevation()
{
    QDate date = QDate::fromString(dateEdit->text(), "yyyy-MM-dd");
    double lat = latEdit->text().toDouble();
    double lon = lonEdit->text().toDouble();
    double targetElev = targetElevEdit->text().toDouble();
    double targetAz = targetAzEdit->text().toDouble();
    bool azProvided = !targetAzEdit->text().trimmed().isEmpty();

    double minDiffAM = 999, minDiffPM = 999;
    QTime bestTimeAM, bestTimePM;

    for (int h = 0; h < 24; ++h) {
        for (int m = 0; m < 60; ++m) {
            for (int s = 0; s < 60; ++s) {
                QDateTime dt(date, QTime(h, m, s), Qt::UTC);
                double elev = calculateSolarElevation(lat, lon, dt);
                double diff = qAbs(elev - targetElev);
                double az = calculateSolarAzimuth(lat, lon, dt);

                if (az < 180.0 && diff < minDiffAM) {
                    minDiffAM = diff;
                    bestTimeAM = QTime(h, m, s);
                } else if (az >= 180.0 && diff < minDiffPM) {
                    minDiffPM = diff;
                    bestTimePM = QTime(h, m, s);
                }
            }
        }
    }

    QString output;
    if (targetElevEdit->text().isEmpty()) {
        output = QString("请提供目标太阳高度角。");
    } else if (azProvided) {
        QTime selectedTime = (targetAz < 180.0) ? bestTimeAM : bestTimePM;
        output = QString("根据用户输入方位角 %1°，预测时间为：%2")
                    .arg(targetAz, 0, 'f', 1).arg(selectedTime.toString("hh:mm:ss"));
    } else {
        output = QString("上午最接近 %1° 的时间约为：%2").arg(targetElev).arg(bestTimeAM.toString("hh:mm:ss")) + QString("下午最接近 %1° 的时间约为：%2").arg(targetElev).arg(bestTimePM.toString("hh:mm:ss"));
    }

    resultLabel->setText(output);
}
