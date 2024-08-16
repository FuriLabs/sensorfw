/**
   @file tiltdetectorsensor_a.cpp
   @brief D-Bus Adaptor for TiltDetectorSensor

   <p>
   Copyright (C) 2024 Furi Labs

   @author bardia@furilabs.com

   This file is part of Sensord.

   Sensord is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License
   version 2.1 as published by the Free Software Foundation.

   Sensord is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with Sensord.  If not, see <http://www.gnu.org/licenses/>.
   </p>
*/

#include "tiltdetectorsensor_a.h"

TiltDetectorSensorChannelAdaptor::TiltDetectorSensorChannelAdaptor(QObject* parent) :
    AbstractSensorChannelAdaptor(parent)
{
}

Unsigned TiltDetectorSensorChannelAdaptor::tiltdetector() const
{
    return qvariant_cast<Unsigned>(parent()->property("tiltdetector"));
}

int TiltDetectorSensorChannelAdaptor::threshold() const
{
    return qvariant_cast<int>(parent()->property("threshold"));
}

void TiltDetectorSensorChannelAdaptor::resetTiltDetector()
{
    QMetaObject::invokeMethod(parent(), "resetTiltDetector");
    sensordLogD() << "tilt detector reset to 0";
}

void TiltDetectorSensorChannelAdaptor::setThreshold(int value)
{
    parent()->setProperty("threshold", value);
}
