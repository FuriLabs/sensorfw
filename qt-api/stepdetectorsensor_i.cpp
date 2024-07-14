/**
   @file stepdetectorsensor_i.cpp
   @brief Interface for step detector sensor

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
#include "stepdetectorsensor_i.h"
#include "socketreader.h"

const char* StepDetectorSensorChannelInterface::staticInterfaceName = "local.StepDetectorSensor";

AbstractSensorChannelInterface* StepDetectorSensorChannelInterface::factoryMethod(const QString& id, int sessionId)
{
    return new StepDetectorSensorChannelInterface(OBJECT_PATH + "/" + id, sessionId);
}

StepDetectorSensorChannelInterface::StepDetectorSensorChannelInterface(const QString& path, int sessionId)
    : AbstractSensorChannelInterface(path, StepDetectorSensorChannelInterface::staticInterfaceName, sessionId)
{
}

const StepDetectorSensorChannelInterface* StepDetectorSensorChannelInterface::listenInterface(const QString& id)
{
    return dynamic_cast<const StepDetectorSensorChannelInterface*> (interface(id));
}

StepDetectorSensorChannelInterface* StepDetectorSensorChannelInterface::controlInterface(const QString& id)
{
    return interface(id);
}

StepDetectorSensorChannelInterface* StepDetectorSensorChannelInterface::interface(const QString& id)
{
    SensorManagerInterface& sm = SensorManagerInterface::instance();
    if ( !sm.registeredAndCorrectClassName( id, StepDetectorSensorChannelInterface::staticMetaObject.className() ) )
    {
        return 0;
    }

    return dynamic_cast<StepDetectorSensorChannelInterface*>(sm.interface(id));
}

bool StepDetectorSensorChannelInterface::dataReceivedImpl()
{
    QVector<TimedUnsigned> values;
    if(!read<TimedUnsigned>(values))
        return false;
    foreach(const TimedUnsigned& data, values)
        emit stepDetected(data);
    return true;
}

Unsigned StepDetectorSensorChannelInterface::steps()
{
    return getAccessor<Unsigned>("steps");
}
