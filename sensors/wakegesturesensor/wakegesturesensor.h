/**
   @file wakegesturesensor.h
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

#ifndef WAKEGESTURE_SENSOR_CHANNEL_H
#define WAKEGESTURE_SENSOR_CHANNEL_H

#include <QObject>

#include "deviceadaptor.h"
#include "abstractsensor.h"
#include "wakegesturesensor_a.h"
#include "dataemitter.h"
#include "datatypes/timedunsigned.h"
#include "datatypes/unsigned.h"

class Bin;
template <class TYPE> class BufferReader;
class FilterBase;

/**
 * @brief Sensor for accessing device wakegesture.
 */
class WakeGestureSensorChannel :
        public AbstractSensorChannel,
        public DataEmitter<TimedUnsigned>
{
    Q_OBJECT;
    Q_PROPERTY(Unsigned wakegesture READ wakegesture);

public:

    /**
     * Factory method for WakeGestureSensorChannel.
     * @return New WakeGestureSensorChannel as AbstractSensorChannel*
     */
    static AbstractSensorChannel* factoryMethod(const QString& id)
    {
        WakeGestureSensorChannel* sc = new WakeGestureSensorChannel(id);
        new WakeGestureSensorChannelAdaptor(sc);

        return sc;
    }

    /**
    * Property method returning current wakegesture.
    * @return Current wakegesture.
    */
    Unsigned wakegesture() const
    {
        return prevWakeGesture;
    }

public Q_SLOTS:
    bool start();
    bool stop();

signals:
    /**
     * Sent whenever wakegesture interpretation has changed.
     * @param wakegesture New wakegesture.
     */
    void wakegestureChanged(const int& wakegesture);

protected:
    WakeGestureSensorChannel(const QString& id);
    virtual ~WakeGestureSensorChannel();

private:
    TimedUnsigned                         prevWakeGesture;
    Bin*                             filterBin_;
    Bin*                             marshallingBin_;

    DeviceAdaptor*                   wakeGestureAdaptor_;

    BufferReader<TimedUnsigned>*   wakegestureReader_;

    RingBuffer<TimedUnsigned>*            outputBuffer_;

    /**
     * Emits new device wakegesture through DBus.
     * @param value WakeGesture value to emit.
     */
    void emitData(const TimedUnsigned& value);
};

#endif // WAKEGESTURE_SENSOR_CHANNEL_H
