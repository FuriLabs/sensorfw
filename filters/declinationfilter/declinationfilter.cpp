/**
   @file declinationfilter.cpp
   @brief DeclinationFilter

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Ustun Ergenoglu <ext-ustun.ergenoglu@nokia.com>
   @author Antti Virtanen <antti.i.virtanen@nokia.com>

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

#include <QSettings>
#include <QSettings>

#include "declinationfilter.h"
#include "config.h"
#include "logging.h"

const char* DeclinationFilter::declinationKey = "/system/osso/location/settings/magneticvariation";

DeclinationFilter::DeclinationFilter() :
        Filter<CompassData, DeclinationFilter, CompassData>(this, &DeclinationFilter::correct),
        declinationCorrection_(0)
{
    updateInterval_ = SensorFrameworkConfig::configuration()->value<quint64>("compass/declination_update_interval", 1000 * 60 * 60) * 1000;
    loadSettings();
}

void DeclinationFilter::correct(unsigned, const CompassData* data)
{
    CompassData newOrientation(*data);
    if (newOrientation.timestamp_ - lastUpdate_ > updateInterval_) {
        loadSettings();
        lastUpdate_ = newOrientation.timestamp_;
    }

    newOrientation.correctedDegrees_ = newOrientation.degrees_;
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    if (declinationCorrection_) {
        newOrientation.correctedDegrees_ += declinationCorrection_;
#else
    if (declinationCorrection_.loadAcquire() != 0) {
        newOrientation.correctedDegrees_ += declinationCorrection_.loadAcquire();
#endif
        newOrientation.correctedDegrees_ %= 360;
//        sensordLogT() << "DeclinationFilter corrected degree " << newOrientation.degrees_ << " => " << newOrientation.correctedDegrees_ << ". Level: " << newOrientation.level_;
    }
    orientation_ = newOrientation;
    source_.propagate(1, &orientation_);
}

void DeclinationFilter::loadSettings()
{
    QSettings confFile("/etc/xdg/sensorfw/location.conf", QSettings::IniFormat);
    confFile.beginGroup("location");
    double declination = confFile.value("declination",0).toDouble();
    if (declination != 0) {
        declinationCorrection_ = declination;
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    sensordLogD() << "Fetched declination correction from GConf: " << declinationCorrection_;
#else
    sensordLogD() << "Fetched declination correction from GConf: " << declinationCorrection_.loadAcquire();
#endif
}

int DeclinationFilter::declinationCorrection()
{
    loadSettings();
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    return declinationCorrection_;
#else
    return declinationCorrection_.loadAcquire();
#endif
}
