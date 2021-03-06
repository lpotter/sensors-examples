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
#include "tap.h"

bool TapSensorFilter::filter(QTapReading *reading)
{
    QString output;
    switch (reading->tapDirection()) {
        case QTapReading::X:
        case QTapReading::X_Both:
                output = "X";         break;
        case QTapReading::Y:
        case QTapReading::Y_Both:
                output = "Y";         break;
        case QTapReading::Z:
        case QTapReading::Z_Both:  
                output = "Z";         break;
        case QTapReading::X_Pos:     output = "X pos";     break;
        case QTapReading::Y_Pos:     output = "Y pos";     break;
        case QTapReading::Z_Pos:     output = "Z pos";     break;
        case QTapReading::X_Neg:     output = "X neg";     break;
        case QTapReading::Y_Neg:     output = "Y neg";     break;
        case QTapReading::Z_Neg:     output = "Z neg";     break;
        case QTapReading::Undefined: output = "Undefined"; break;
        default: output = "Invalid enum value";
    }
    long timestamp = reading->timestamp();
    emit reading->isDoubleTap()?doubleHappened(output, timestamp):singleHappened(output, timestamp);
    return false; // don't store the reading in the sensor
}

