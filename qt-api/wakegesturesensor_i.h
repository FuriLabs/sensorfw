/**
   @file wakegesturesensor_i.h
   @brief Interface for wake gesture sensor

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

#ifndef WAKEGESTURESENSOR_I_H
#define WAKEGESTURESENSOR_I_H

#include <QtDBus/QtDBus>

#include "datatypes/unsigned.h"
#include "abstractsensor_i.h"

/**
 * Client interface for accessing wake gesture sensor.
 * Provides signal on change of gesture events.
 * Previous gesture events can be queried any time. Provided
 * values are in \e gestures.
 */
class WakeGestureSensorChannelInterface : public AbstractSensorChannelInterface
{
    Q_OBJECT
    Q_DISABLE_COPY(WakeGestureSensorChannelInterface)
    Q_PROPERTY(Unsigned gestures READ gestures)

public:
    /**
     * Name of the D-Bus interface for this class.
     */
    static const char* staticInterfaceName;

    /**
     * Create new instance of the class.
     *
     * @param id Sensor ID.
     * @param sessionId Session ID.
     * @return Pointer to new instance of the class.
     */
    static AbstractSensorChannelInterface* factoryMethod(const QString& id, int sessionId);

    /**
     * Get latest gesture events from sensor daemon.
     *
     * @return gesture events since reboot.
     */
    Unsigned gestures();

    /**
     * Constructor.
     *
     * @param path      path.
     * @param sessionId session ID.
     */
    WakeGestureSensorChannelInterface(const QString& path, int sessionId);

    /**
     * Request a listening interface to the sensor.
     *
     * @param id sensor ID.
     * @return Pointer to interface, or NULL on failure.
     * @deprecated use interface(const QString&) instead.
     */
    static const WakeGestureSensorChannelInterface* listenInterface(const QString& id);

    /**
     * Request a control interface to the sensor.
     *
     * @param id sensor ID.
     * @return Pointer to interface, or NULL on failure.
     * @deprecated use interface(const QString&) instead.
     */
    static WakeGestureSensorChannelInterface* controlInterface(const QString& id);

    /**
     * Request an interface to the sensor.
     *
     * @param id sensor ID.
     * @return Pointer to interface, or NULL on failure.
     */
    static WakeGestureSensorChannelInterface* interface(const QString& id);

protected:
    virtual bool dataReceivedImpl();

Q_SIGNALS:
    /**
     * Sent when a gesture event has occurred.
     *
     * @param value gesture event reading.
     */
    void WakeGestureChanged(const Unsigned& value);
};

namespace local {
  typedef ::WakeGestureSensorChannelInterface WakeGestureSensor;
}

#endif // WAKEGESTURESENSOR_I_H
