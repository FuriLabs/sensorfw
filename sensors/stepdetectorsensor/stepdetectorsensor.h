/**
   @file stepdetectorsensor.h
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

#ifndef STEPDETECTOR_SENSOR_CHANNEL_H
#define STEPDETECTOR_SENSOR_CHANNEL_H

#include <QObject>

#include "deviceadaptor.h"
#include "abstractsensor.h"
#include "stepdetectorsensor_a.h"
#include "dataemitter.h"
#include "datatypes/timedunsigned.h"
#include "datatypes/unsigned.h"

class Bin;
template <class TYPE> class BufferReader;
class FilterBase;

/**
 * @brief Sensor for accessing device step detector.
 */
class StepDetectorSensorChannel :
        public AbstractSensorChannel,
        public DataEmitter<TimedUnsigned>
{
    Q_OBJECT;
    Q_PROPERTY(Unsigned stepdetector READ stepdetector);

public:

    /**
     * Factory method for StepDetectorSensorChannel.
     * @return New StepDetectorSensorChannel as AbstractSensorChannel*
     */
    static AbstractSensorChannel* factoryMethod(const QString& id)
    {
        StepDetectorSensorChannel* sc = new StepDetectorSensorChannel(id);
        new StepDetectorSensorChannelAdaptor(sc);

        return sc;
    }

    /**
    * Property method returning current step detector.
    * @return Current step detector.
    */
    Unsigned stepdetector() const
    {
        return prevStepDetector;
    }

public Q_SLOTS:
    bool start();
    bool stop();

signals:
    /**
     * Sent whenever step detector interpretation has changed.
     * @param stepdetector New step detector.
     */
    void stepDetectorChanged(const int& stepdetector);

protected:
    StepDetectorSensorChannel(const QString& id);
    virtual ~StepDetectorSensorChannel();

private:
    TimedUnsigned                         prevStepDetector;
    Bin*                             filterBin_;
    Bin*                             marshallingBin_;

    DeviceAdaptor*                   stepDetectorAdaptor_;

    BufferReader<TimedUnsigned>*   stepdetectorReader_;

    RingBuffer<TimedUnsigned>*            outputBuffer_;

    /**
     * Emits new device step detector through DBus.
     * @param value StepDetector value to emit.
     */
    void emitData(const TimedUnsigned& value);
};

#endif // STEPDETECTOR_SENSOR_CHANNEL_H
