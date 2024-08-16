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

#ifndef TILTDETECTOR_SENSOR_CHANNEL_H
#define TILTDETECTOR_SENSOR_CHANNEL_H

#include <QObject>

#include "deviceadaptor.h"
#include "abstractsensor.h"
#include "tiltdetectorsensor_a.h"
#include "dataemitter.h"
#include "datatypes/timedunsigned.h"
#include "datatypes/unsigned.h"

class Bin;
template <class TYPE> class BufferReader;
class FilterBase;

/**
 * @brief Sensor for accessing device tilt detector.
 */
class TiltDetectorSensorChannel :
        public AbstractSensorChannel,
        public DataEmitter<TimedUnsigned>
{
    Q_OBJECT;
    Q_PROPERTY(Unsigned tiltdetector READ tiltdetector);

public:

    /**
     * Factory method for TiltDetectorSensorChannel.
     * @return New TiltDetectorSensorChannel as AbstractSensorChannel*
     */
    static AbstractSensorChannel* factoryMethod(const QString& id)
    {
        TiltDetectorSensorChannel* sc = new TiltDetectorSensorChannel(id);
        new TiltDetectorSensorChannelAdaptor(sc);

        return sc;
    }

    /**
    * Property method returning current tilt detector.
    * @return Current tilt detector.
    */
    Unsigned tiltdetector() const
    {
        return prevTiltDetector;
    }

public Q_SLOTS:
    bool start();
    bool stop();
    void resetTiltDetector();

signals:
    /**
     * Sent whenever tilt detector interpretation has changed.
     * @param tiltdetector New tilt detector.
     */
    void tiltDetectorChanged(const int& tiltdetector);

protected:
    TiltDetectorSensorChannel(const QString& id);
    virtual ~TiltDetectorSensorChannel();

private:
    TimedUnsigned                         prevTiltDetector;
    Bin*                             filterBin_;
    Bin*                             marshallingBin_;

    DeviceAdaptor*                   tiltDetectorAdaptor_;

    BufferReader<TimedUnsigned>*   tiltdetectorReader_;

    RingBuffer<TimedUnsigned>*            outputBuffer_;

    /**
     * Emits new device tilt detector through DBus.
     * @param value TiltDetector value to emit.
     */
    void emitData(const TimedUnsigned& value);
};

#endif // TILTDETECTOR_SENSOR_CHANNEL_H
