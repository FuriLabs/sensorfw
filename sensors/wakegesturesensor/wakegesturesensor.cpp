/**
   @file wakegesturesensor.cpp
   @brief WakeGestureSensor

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

#include "wakegesturesensor.h"

#include "sensormanager.h"
#include "bin.h"
#include "bufferreader.h"

WakeGestureSensorChannel::WakeGestureSensorChannel(const QString& id) :
        AbstractSensorChannel(id),
        DataEmitter<TimedUnsigned>(1),
        prevWakeGesture(0,0)
{
    SensorManager& sm = SensorManager::instance();

    wakeGestureAdaptor_ = sm.requestDeviceAdaptor("wakegestureadaptor");
    if (!wakeGestureAdaptor_) {
        setValid(false);
        return;
    }

    wakegestureReader_ = new BufferReader<TimedUnsigned>(1);

    outputBuffer_ = new RingBuffer<TimedUnsigned>(1);

    // Create buffers for filter chain
    filterBin_ = new Bin;

    filterBin_->add(wakegestureReader_, "wakegesture");
    filterBin_->add(outputBuffer_, "buffer");

    // Join filterchain buffers
    filterBin_->join("wakegesture", "source", "buffer", "sink");

    // Join datasources to the chain
    connectToSource(wakeGestureAdaptor_, "wakegesture", wakegestureReader_);

    marshallingBin_ = new Bin;
    marshallingBin_->add(this, "sensorchannel");

    outputBuffer_->join(this);

    setDescription("wakegesture sensor for wake up detection");
    setRangeSource(wakeGestureAdaptor_);
    addStandbyOverrideSource(wakeGestureAdaptor_);
    setIntervalSource(wakeGestureAdaptor_);

    setValid(true);
}

WakeGestureSensorChannel::~WakeGestureSensorChannel()
{
    if (isValid()) {
        SensorManager& sm = SensorManager::instance();

        disconnectFromSource(wakeGestureAdaptor_, "wakegesture", wakegestureReader_);

        sm.releaseDeviceAdaptor("wakegestureadaptor");

        delete wakegestureReader_;
        delete outputBuffer_;
        delete marshallingBin_;
        delete filterBin_;
    }
}

bool WakeGestureSensorChannel::start()
{
    sensordLogD() << "Starting WakeGestureSensorChannel";

    if (AbstractSensorChannel::start()) {
        marshallingBin_->start();
        filterBin_->start();
        wakeGestureAdaptor_->startSensor();
    }
    return true;
}

bool WakeGestureSensorChannel::stop()
{
    sensordLogD() << "Stopping WakeGestureSensorChannel";

    if (AbstractSensorChannel::stop()) {
        wakeGestureAdaptor_->stopSensor();
        filterBin_->stop();
        marshallingBin_->stop();
    }
    return true;
}

void WakeGestureSensorChannel::emitData(const TimedUnsigned& value)
{
    prevWakeGesture.value_ = value.value_;
    writeToClients((const void *)&value, sizeof(value));
}
