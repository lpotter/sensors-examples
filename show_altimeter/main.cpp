/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore>
#include <qaltimeter.h>
#include <unistd.h>

QT_USE_NAMESPACE

        namespace check{
    static void checkRate(QSensor* sensor, int rate){
        qDebug()<<"data rate set for sensor "<<rate;
        if (rate == sensor->dataRate()) return;
        if (rate!=sensor->dataRate()){
            qrangelist rates = sensor->availableDataRates();
            QString datarates;
            for( int i = 0; i < rates.size(); ++i )
            {
                datarates.append("[");
                QString num;
                datarates.append(num.setNum(rates[i].first));
                datarates.append("..");
                datarates.append(num.setNum(rates[i].second));
                datarates.append("] ");
            }
            qDebug()<<"Rate setting failed, rate must be within range "<<datarates;
        }
    }
}


class AltimeterFilter : public QAltimeterFilter
{
public:
    qtimestamp stamp;
    qreal sum;
    int i;
    int dataRate;
    qreal varianceSum;

    AltimeterFilter(int fq):sum(0),i(0),dataRate(fq), varianceSum(0){}

    bool filter(QAltimeterReading *reading)
    {
        int diff = ( reading->timestamp() - stamp );
        stamp = reading->timestamp();
        QTextStream out(stdout);
        qreal fq = 1000000.0 / diff;
        out << QString("Altitude: %1 x").arg(reading->altitude(), 5, 'f', 1)
                << stamp
                << QString(" (%1 ms since last, %2 Hz)").arg(diff / 1000, 4).arg( fq, 5, 'f', 1) << endl;

        if (dataRate>0){
            if (qAbs(dataRate-fq)<(fq/4)){
                sum +=fq;
                i++;
                if (i>1){
                    qreal average = sum/i;
                    varianceSum += (fq-average)*(fq-average);
                    out<<" amount "<<i<< " average = "<<average<<" variance = "<<varianceSum/i<<endl;
                }
            }
        }
        return false; // don't store the reading in the sensor
    }
};

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();
    int rate_place = args.indexOf("-r");
    int rate_val = 0;
    if (rate_place != -1)
        rate_val = args.at(rate_place + 1).toInt();
    QAltimeter sensor;
    sensor.connectToBackend();
    if (args.indexOf("-a") > 0) {
        sensor.setProperty("alwaysOn",true);
    }

    if (rate_val > 0) {
        sensor.setDataRate(rate_val);
        check::checkRate(&sensor, rate_val);
    }

    int buffer_place = args.indexOf("-b");
    int bufferSize = buffer_place!=-1? args.at(buffer_place + 1).toInt():1;
    sensor.setProperty("bufferSize",bufferSize);

    sensor.setProperty("bufferInOneShot",args.indexOf("-bb")>-1);

    AltimeterFilter filter(rate_val);
    sensor.addFilter(&filter);
    qDebug() << "choosing" << sensor.identifier();
    sleep(2);
    sensor.start();
    if (!sensor.isActive()) {
        qWarning("Altimeter didn't start!");
        return 1;
    }

    return app.exec();
}
