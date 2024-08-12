/**
   @file wakegesturesensor_a.h
   @brief D-Bus Adaptor for WakeGestureSensor

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

#ifndef WAKEGESTURE_SENSOR_H
#define WAKEGESTURE_SENSOR_H

#include <QtDBus/QtDBus>

#include "datatypes/unsigned.h"
#include "abstractsensor_a.h"

class WakeGestureSensorChannelAdaptor : public AbstractSensorChannelAdaptor
{
    Q_OBJECT
    Q_DISABLE_COPY(WakeGestureSensorChannelAdaptor)
    Q_CLASSINFO("D-Bus Interface", "local.WakeGestureSensor")
    Q_PROPERTY(Unsigned wakegesture READ wakegesture)
    Q_PROPERTY(int threshold READ threshold WRITE setThreshold)

public:
    WakeGestureSensorChannelAdaptor(QObject* parent);

public Q_SLOTS:
    Unsigned wakegesture() const;
    void resetWakeGesture();
    int threshold() const;
    void setThreshold(int value);

Q_SIGNALS:
    void wakegestureChanged(const Unsigned& wakegesture);
};

#endif
