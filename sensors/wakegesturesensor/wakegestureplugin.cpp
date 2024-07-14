/**
   @file wakegestureplugin.cpp
   @brief Plugin for WakeGestureSensor

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

#include "wakegestureplugin.h"
#include "wakegesturesensor.h"
#include "sensormanager.h"
#include "logging.h"

void WakeGesturePlugin::Register(class Loader&)
{
    sensordLogD() << "registering wakegesturesensor";
    SensorManager& sm = SensorManager::instance();
    sm.registerSensor<WakeGestureSensorChannel>("wakegesturesensor");
}

QStringList WakeGesturePlugin::Dependencies() {
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
    return QString("wakegestureadaptor").split(":", Qt::SkipEmptyParts);
#else
    return QString("wakegestureadaptor").split(":", QString::SkipEmptyParts);
#endif
}
