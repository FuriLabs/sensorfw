/**
   @file wakegesturesensor_a.cpp
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

#include "wakegesturesensor_a.h"

WakeGestureSensorChannelAdaptor::WakeGestureSensorChannelAdaptor(QObject* parent) :
    AbstractSensorChannelAdaptor(parent)
{
}

Unsigned WakeGestureSensorChannelAdaptor::wakegesture() const
{
    return qvariant_cast<Unsigned>(parent()->property("wakegesture"));
}

void WakeGestureSensorChannelAdaptor::resetWakeGesture()
{
    QMetaObject::invokeMethod(parent(), "resetWakeGesture");
    sensordLogD() << "wake gesture reset to 0";
}

int WakeGestureSensorChannelAdaptor::threshold() const
{
    return qvariant_cast<int>(parent()->property("threshold"));
}

void WakeGestureSensorChannelAdaptor::setThreshold(int value)
{
    parent()->setProperty("threshold", value);
}
