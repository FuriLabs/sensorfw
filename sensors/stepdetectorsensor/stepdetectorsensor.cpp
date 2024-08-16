/**
   @file stepdetectorsensor.cpp
   @brief StepDetectorSensor

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

#include "stepdetectorsensor.h"

#include "sensormanager.h"
#include "bin.h"
#include "bufferreader.h"

StepDetectorSensorChannel::StepDetectorSensorChannel(const QString& id) :
        AbstractSensorChannel(id),
        DataEmitter<TimedUnsigned>(1),
        prevStepDetector(0,0)
{
    SensorManager& sm = SensorManager::instance();

    stepDetectorAdaptor_ = sm.requestDeviceAdaptor("stepdetectoradaptor");
    if (!stepDetectorAdaptor_) {
        setValid(false);
        return;
    }

    stepdetectorReader_ = new BufferReader<TimedUnsigned>(1);

    outputBuffer_ = new RingBuffer<TimedUnsigned>(1);

    // Create buffers for filter chain
    filterBin_ = new Bin;

    filterBin_->add(stepdetectorReader_, "stepdetector");
    filterBin_->add(outputBuffer_, "buffer");

    // Join filterchain buffers
    filterBin_->join("stepdetector", "source", "buffer", "sink");

    // Join datasources to the chain
    connectToSource(stepDetectorAdaptor_, "stepdetector", stepdetectorReader_);

    marshallingBin_ = new Bin;
    marshallingBin_->add(this, "sensorchannel");

    outputBuffer_->join(this);

    setDescription("step detector sensor for step detection");
    setRangeSource(stepDetectorAdaptor_);
    addStandbyOverrideSource(stepDetectorAdaptor_);
    setIntervalSource(stepDetectorAdaptor_);

    setValid(true);
}

StepDetectorSensorChannel::~StepDetectorSensorChannel()
{
    if (isValid()) {
        SensorManager& sm = SensorManager::instance();

        disconnectFromSource(stepDetectorAdaptor_, "stepdetector", stepdetectorReader_);

        sm.releaseDeviceAdaptor("stepdetectoradaptor");

        delete stepdetectorReader_;
        delete outputBuffer_;
        delete marshallingBin_;
        delete filterBin_;
    }
}

bool StepDetectorSensorChannel::start()
{
    sensordLogD() << "Starting StepDetectorSensorChannel";

    if (AbstractSensorChannel::start()) {
        marshallingBin_->start();
        filterBin_->start();
        stepDetectorAdaptor_->startSensor();
    }
    return true;
}

bool StepDetectorSensorChannel::stop()
{
    sensordLogD() << "Stopping StepDetectorSensorChannel";

    if (AbstractSensorChannel::stop()) {
        stepDetectorAdaptor_->stopSensor();
        filterBin_->stop();
        marshallingBin_->stop();
    }
    return true;
}

void StepDetectorSensorChannel::resetStepDetector()
{
    SensorManager& sm = SensorManager::instance();
    if (isValid()) {
        disconnectFromSource(stepDetectorAdaptor_, "stepdetector", stepdetectorReader_);
        sm.releaseDeviceAdaptor("stepdetectoradaptor");
    }

    stepDetectorAdaptor_ = sm.requestDeviceAdaptor("stepdetectoradaptor");

    filterBin_ = new Bin;
    filterBin_->add(stepdetectorReader_, "stepdetector");
    filterBin_->add(outputBuffer_, "buffer");
    filterBin_->join("stepdetector", "source", "buffer", "sink");

    connectToSource(stepDetectorAdaptor_, "stepdetector", stepdetectorReader_);

    outputBuffer_->join(this);

    prevStepDetector = TimedUnsigned(0, 0);
    emitData(prevStepDetector);

    Q_EMIT stepDetectorChanged(prevStepDetector.value_);
}

void StepDetectorSensorChannel::emitData(const TimedUnsigned& value)
{
    prevStepDetector.value_ = value.value_;
    writeToClients((const void *)&value, sizeof(value));
}
