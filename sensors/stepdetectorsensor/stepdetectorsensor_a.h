/**
   @file stepdetectorsensor_a.h
   @brief D-Bus Adaptor for StepDetectorSensor

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

#ifndef STEPDETECTOR_SENSOR_H
#define STEPDETECTOR_SENSOR_H

#include <QtDBus/QtDBus>

#include "datatypes/unsigned.h"
#include "abstractsensor_a.h"

class StepDetectorSensorChannelAdaptor : public AbstractSensorChannelAdaptor
{
    Q_OBJECT
    Q_DISABLE_COPY(StepDetectorSensorChannelAdaptor)
    Q_CLASSINFO("D-Bus Interface", "local.StepDetectorSensor")
    Q_PROPERTY(Unsigned stepdetector READ stepdetector)
    Q_PROPERTY(int threshold READ threshold WRITE setThreshold)

public:
    StepDetectorSensorChannelAdaptor(QObject* parent);

public Q_SLOTS:
    Unsigned stepdetector() const;
    int threshold() const;
    void resetStepDetector();
    void setThreshold(int value);

Q_SIGNALS:
    void stepDetectorChanged(const Unsigned& stepdetector);
};

#endif
