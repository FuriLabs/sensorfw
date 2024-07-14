/**
   @file stepdetectorplugin.cpp
   @brief Plugin for StepDetectorSensor

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

#include "stepdetectorplugin.h"
#include "stepdetectorsensor.h"
#include "sensormanager.h"
#include "logging.h"

void StepDetectorPlugin::Register(class Loader&)
{
    sensordLogD() << "registering stepdetectorsensor";
    SensorManager& sm = SensorManager::instance();
    sm.registerSensor<StepDetectorSensorChannel>("stepdetectorsensor");
}

QStringList StepDetectorPlugin::Dependencies() {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    return QString("stepdetectoradaptor").split(":", Qt::SkipEmptyParts);
#else
    return QString("stepdetectoradaptor").split(":", QString::SkipEmptyParts);
#endif
}
