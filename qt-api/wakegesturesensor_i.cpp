/**
   @file wakegesturesensor_i.cpp
   @brief Interface for wake gesture sensor

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

#include "sensormanagerinterface.h"
#include "wakegesturesensor_i.h"
#include "socketreader.h"

const char* WakeGestureSensorChannelInterface::staticInterfaceName = "local.WakeGestureSensor";

AbstractSensorChannelInterface* WakeGestureSensorChannelInterface::factoryMethod(const QString& id, int sessionId)
{
    return new WakeGestureSensorChannelInterface(OBJECT_PATH + "/" + id, sessionId);
}

WakeGestureSensorChannelInterface::WakeGestureSensorChannelInterface(const QString& path, int sessionId)
    : AbstractSensorChannelInterface(path, WakeGestureSensorChannelInterface::staticInterfaceName, sessionId)
{
}

const WakeGestureSensorChannelInterface* WakeGestureSensorChannelInterface::listenInterface(const QString& id)
{
    return dynamic_cast<const WakeGestureSensorChannelInterface*> (interface(id));
}

WakeGestureSensorChannelInterface* WakeGestureSensorChannelInterface::controlInterface(const QString& id)
{
    return interface(id);
}

WakeGestureSensorChannelInterface* WakeGestureSensorChannelInterface::interface(const QString& id)
{
    SensorManagerInterface& sm = SensorManagerInterface::instance();
    if ( !sm.registeredAndCorrectClassName( id, WakeGestureSensorChannelInterface::staticMetaObject.className() ) )
    {
        return 0;
    }

    return dynamic_cast<WakeGestureSensorChannelInterface*>(sm.interface(id));
}

bool WakeGestureSensorChannelInterface::dataReceivedImpl()
{
    QVector<TimedUnsigned> values;
    if(!read<TimedUnsigned>(values))
        return false;
    foreach(const TimedUnsigned& data, values)
        emit WakeGestureChanged(data);
    return true;
}

Unsigned WakeGestureSensorChannelInterface::gestures()
{
    return getAccessor<Unsigned>("gestures");
}
