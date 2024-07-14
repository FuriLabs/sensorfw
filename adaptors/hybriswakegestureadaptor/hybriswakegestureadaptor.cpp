/****************************************************************************
**
** Copyright (C) 2024 Furi Labs
** Contact: bardia@furilabs.com
**
** $QT_BEGIN_LICENSE:LGPL$
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "hybriswakegestureadaptor.h"
#include "logging.h"
#include "datatypes/utils.h"
#include "config.h"

#ifndef SENSOR_TYPE_WAKE_GESTURE
#define SENSOR_TYPE_WAKE_GESTURE (23)
#endif

HybrisWakeGestureAdaptor::HybrisWakeGestureAdaptor(const QString& id) :
    HybrisAdaptor(id,SENSOR_TYPE_WAKE_GESTURE)
{
    buffer = new DeviceAdaptorRingBuffer<TimedUnsigned>(1);
    setAdaptedSensor("hybriswakegesture", "Internal wake gesture coordinates", buffer);

    setDescription("Hybris wake gesture");
    powerStatePath = SensorFrameworkConfig::configuration()->value("wakegesture/powerstate_path").toByteArray();
    if (!powerStatePath.isEmpty() && !QFile::exists(powerStatePath))
    {
        sensordLogW() << "Path does not exists: " << powerStatePath;
        powerStatePath.clear();
    }

    // Set default delay.
    setInterval(200, 0);
}

HybrisWakeGestureAdaptor::~HybrisWakeGestureAdaptor()
{
    delete buffer;
}

bool HybrisWakeGestureAdaptor::startSensor()
{
    if (!(HybrisAdaptor::startSensor()))
        return false;
    if (isRunning() && !powerStatePath.isEmpty())
        writeToFile(powerStatePath, "1");
    sensordLogD() << "Hybris WakeGestureAdaptor start\n";
    return true;
}

void HybrisWakeGestureAdaptor::stopSensor()
{
    HybrisAdaptor::stopSensor();
     if (!isRunning() && !powerStatePath.isEmpty())
        writeToFile(powerStatePath, "0");
    sensordLogD() << "Hybris WakeGestureAdaptor stop\n";
}

void HybrisWakeGestureAdaptor::processSample(const sensors_event_t& data)
{
    TimedUnsigned *d = buffer->nextSlot();
    d->timestamp_ = quint64(data.timestamp * .001);

#ifdef USE_BINDER
    unsigned int stepCount = data.u.stepCount;
    // if hal returns a float, then it will end up being 1.0f which is 1065353216
    if (stepCount == 1065353216) {
        stepCount = 1;
    }
    d->value_ = stepCount;
#else
#ifdef NO_SENSORS_EVENT_U64
    uint64_t value = 0;
    memcpy(&value, data.data, sizeof value);
    unsigned int stepCount = static_cast<unsigned int>(value);
    if (stepCount == 1065353216)
        stepCount = 1;

    d->value_ = stepCount;
#else
    unsigned int stepCount = static_cast<unsigned int>(data.u64.step_counter);
    if (stepCount == 1065353216)
        stepCount = 1;

    d->value_ = stepCount;
    sensordLogD() << "HybrisWakeGestureAdaptor: processSample() - step_counter: " << data.u64.step_counter << ", value_: " << d->value_;
#endif
#endif

    buffer->commit();
    buffer->wakeUpReaders();
}
