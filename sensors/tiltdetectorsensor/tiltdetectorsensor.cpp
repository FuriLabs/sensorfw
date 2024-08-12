/**
   @file tiltdetectorsensor.cpp
   @brief TiltDetectorSensor

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

#include "tiltdetectorsensor.h"

#include "sensormanager.h"
#include "bin.h"
#include "bufferreader.h"

TiltDetectorSensorChannel::TiltDetectorSensorChannel(const QString& id) :
        AbstractSensorChannel(id),
        DataEmitter<TimedUnsigned>(1),
        prevTiltDetector(0,0)
{
    SensorManager& sm = SensorManager::instance();

    tiltDetectorAdaptor_ = sm.requestDeviceAdaptor("tiltdetectoradaptor");
    if (!tiltDetectorAdaptor_) {
        setValid(false);
        return;
    }

    tiltdetectorReader_ = new BufferReader<TimedUnsigned>(1);

    outputBuffer_ = new RingBuffer<TimedUnsigned>(1);

    // Create buffers for filter chain
    filterBin_ = new Bin;

    filterBin_->add(tiltdetectorReader_, "tiltdetector");
    filterBin_->add(outputBuffer_, "buffer");

    // Join filterchain buffers
    filterBin_->join("tiltdetector", "source", "buffer", "sink");

    // Join datasources to the chain
    connectToSource(tiltDetectorAdaptor_, "tiltdetector", tiltdetectorReader_);

    marshallingBin_ = new Bin;
    marshallingBin_->add(this, "sensorchannel");

    outputBuffer_->join(this);

    setDescription("tilt detector sensor for tilt detection");
    setRangeSource(tiltDetectorAdaptor_);
    addStandbyOverrideSource(tiltDetectorAdaptor_);
    setIntervalSource(tiltDetectorAdaptor_);

    setValid(true);
}

TiltDetectorSensorChannel::~TiltDetectorSensorChannel()
{
    if (isValid()) {
        SensorManager& sm = SensorManager::instance();

        disconnectFromSource(tiltDetectorAdaptor_, "tiltdetector", tiltdetectorReader_);

        sm.releaseDeviceAdaptor("tiltdetectoradaptor");

        delete tiltdetectorReader_;
        delete outputBuffer_;
        delete marshallingBin_;
        delete filterBin_;
    }
}

bool TiltDetectorSensorChannel::start()
{
    sensordLogD() << "Starting TiltDetectorSensorChannel";

    if (AbstractSensorChannel::start()) {
        marshallingBin_->start();
        filterBin_->start();
        tiltDetectorAdaptor_->startSensor();
    }
    return true;
}

bool TiltDetectorSensorChannel::stop()
{
    sensordLogD() << "Stopping TiltDetectorSensorChannel";

    if (AbstractSensorChannel::stop()) {
        tiltDetectorAdaptor_->stopSensor();
        filterBin_->stop();
        marshallingBin_->stop();
    }
    return true;
}

void TiltDetectorSensorChannel::emitData(const TimedUnsigned& value)
{
    prevTiltDetector.value_ = value.value_;
    writeToClients((const void *)&value, sizeof(value));
}
