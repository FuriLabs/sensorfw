/**
   @file tiltdetectorsensor_i.cpp
   @brief Interface for tilt detector sensor

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
#include "tiltdetectorsensor_i.h"
#include "socketreader.h"

const char* TiltDetectorSensorChannelInterface::staticInterfaceName = "local.TiltDetectorSensor";

AbstractSensorChannelInterface* TiltDetectorSensorChannelInterface::factoryMethod(const QString& id, int sessionId)
{
    return new TiltDetectorSensorChannelInterface(OBJECT_PATH + "/" + id, sessionId);
}

TiltDetectorSensorChannelInterface::TiltDetectorSensorChannelInterface(const QString& path, int sessionId)
    : AbstractSensorChannelInterface(path, TiltDetectorSensorChannelInterface::staticInterfaceName, sessionId)
{
}

const TiltDetectorSensorChannelInterface* TiltDetectorSensorChannelInterface::listenInterface(const QString& id)
{
    return dynamic_cast<const TiltDetectorSensorChannelInterface*> (interface(id));
}

TiltDetectorSensorChannelInterface* TiltDetectorSensorChannelInterface::controlInterface(const QString& id)
{
    return interface(id);
}

TiltDetectorSensorChannelInterface* TiltDetectorSensorChannelInterface::interface(const QString& id)
{
    SensorManagerInterface& sm = SensorManagerInterface::instance();
    if ( !sm.registeredAndCorrectClassName( id, TiltDetectorSensorChannelInterface::staticMetaObject.className() ) )
    {
        return 0;
    }

    return dynamic_cast<TiltDetectorSensorChannelInterface*>(sm.interface(id));
}

bool TiltDetectorSensorChannelInterface::dataReceivedImpl()
{
    QVector<TimedUnsigned> values;
    if(!read<TimedUnsigned>(values))
        return false;
    foreach(const TimedUnsigned& data, values)
        emit tiltDetected(data);
    return true;
}

Unsigned TiltDetectorSensorChannelInterface::tilts()
{
    return getAccessor<Unsigned>("tilts");
}
