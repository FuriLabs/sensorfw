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

#include "hybristiltdetectoradaptor.h"
#include "logging.h"
#include "datatypes/utils.h"
#include "config.h"

#ifndef SENSOR_TYPE_TILT_DETECTOR
#define SENSOR_TYPE_TILT_DETECTOR (22)
#endif

HybrisTiltDetectorAdaptor::HybrisTiltDetectorAdaptor(const QString& id) :
    HybrisAdaptor(id, SENSOR_TYPE_TILT_DETECTOR)
{
    buffer = new DeviceAdaptorRingBuffer<TimedUnsigned>(1);
    setAdaptedSensor("hybristiltdetector", "Internal tilt detector coordinates", buffer);

    setDescription("Hybris tilt detector");
    powerStatePath = SensorFrameworkConfig::configuration()->value("tiltdetector/powerstate_path").toByteArray();
    if (!powerStatePath.isEmpty() && !QFile::exists(powerStatePath))
    {
        sensordLogW() << "Path does not exists: " << powerStatePath;
        powerStatePath.clear();
    }

    // Set default delay.
    setInterval(200, 0);
}

HybrisTiltDetectorAdaptor::~HybrisTiltDetectorAdaptor()
{
    delete buffer;
}

bool HybrisTiltDetectorAdaptor::startSensor()
{
    if (!(HybrisAdaptor::startSensor()))
        return false;
    if (isRunning() && !powerStatePath.isEmpty())
        writeToFile(powerStatePath, "1");
    sensordLogD() << "Hybris TiltDetectorAdaptor start\n";
    return true;
}

void HybrisTiltDetectorAdaptor::stopSensor()
{
    HybrisAdaptor::stopSensor();
     if (!isRunning() && !powerStatePath.isEmpty())
        writeToFile(powerStatePath, "0");
    sensordLogD() << "Hybris TiltDetectorAdaptor stop\n";
}

void HybrisTiltDetectorAdaptor::processSample(const sensors_event_t& data)
{
    TimedUnsigned *d = buffer->nextSlot();
    d->timestamp_ = quint64(data.timestamp * .001);

#ifdef USE_BINDER
    unsigned int tiltDetector = data.u.tiltDetector;
    // if hal returns a float, then it will end up being 1.0f which is 1065353216
    if (tiltDetector == 1065353216)
        tiltDetector = 1;
    d->value_ = tiltDetector;
#else
#ifdef NO_SENSORS_EVENT_U64
    uint64_t value = 0;
    memcpy(&value, data.data, sizeof value);
    unsigned int tiltDetector = static_cast<unsigned int>(value);
    if (tiltDetector == 1065353216)
        tiltDetector = 1;
    d->value_ = tiltDetector;
#else
    unsigned int tiltDetector = static_cast<unsigned int>(data.u64.tiltDetector);
    if (tiltDetector == 1065353216)
        tiltDetector = 1;
    d->value_ = tiltDetector;
    sensordLogD() << "HybrisTiltDetectorAdaptor: processSample() - tiltDetector: " << data.u64.tiltDetector << ", value_: " << d->value_;
#endif
#endif

    buffer->commit();
    buffer->wakeUpReaders();
}
