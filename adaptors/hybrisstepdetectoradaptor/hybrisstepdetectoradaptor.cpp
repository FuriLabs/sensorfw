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

#include "hybrisstepdetectoradaptor.h"
#include "logging.h"
#include "datatypes/utils.h"
#include "config.h"

#ifndef SENSOR_TYPE_STEP_DETECTOR
#define SENSOR_TYPE_STEP_DETECTOR (18)
#endif

HybrisStepDetectorAdaptor::HybrisStepDetectorAdaptor(const QString& id) :
    HybrisAdaptor(id,SENSOR_TYPE_STEP_DETECTOR)
{
    buffer = new DeviceAdaptorRingBuffer<TimedUnsigned>(1);
    setAdaptedSensor("hybrisstepdetector", "Internal step detector coordinates", buffer);

    setDescription("Hybris step detector");
    powerStatePath = SensorFrameworkConfig::configuration()->value("stepdetector/powerstate_path").toByteArray();
    if (!powerStatePath.isEmpty() && !QFile::exists(powerStatePath))
    {
        sensordLogW() << "Path does not exists: " << powerStatePath;
        powerStatePath.clear();
    }

    // Set default delay.
    setInterval(200, 0);
}

HybrisStepDetectorAdaptor::~HybrisStepDetectorAdaptor()
{
    delete buffer;
}

bool HybrisStepDetectorAdaptor::startSensor()
{
    if (!(HybrisAdaptor::startSensor()))
        return false;
    if (isRunning() && !powerStatePath.isEmpty())
        writeToFile(powerStatePath, "1");
    sensordLogD() << "Hybris StepDetectorAdaptor start\n";
    return true;
}

void HybrisStepDetectorAdaptor::stopSensor()
{
    HybrisAdaptor::stopSensor();
     if (!isRunning() && !powerStatePath.isEmpty())
        writeToFile(powerStatePath, "0");
    sensordLogD() << "Hybris StepDetectorAdaptor stop\n";
}

void HybrisStepDetectorAdaptor::processSample(const sensors_event_t& data)
{
    TimedUnsigned *d = buffer->nextSlot();
    d->timestamp_ = quint64(data.timestamp * .001);

#ifdef USE_BINDER
    unsigned int stepDetector = data.u.stepDetector;
    // if hal returns a float, then it will end up being 1.0f which is 1065353216
    if (stepDetector == 1065353216)
        stepDetector = 1;
    d->value_ = stepDetector;
#else
#ifdef NO_SENSORS_EVENT_U64
    uint64_t value = 0;
    memcpy(&value, data.data, sizeof value);
    unsigned int stepDetector = static_cast<unsigned int>(value);
    if (stepDetector == 1065353216)
        stepDetector = 1;
    d->value_ = stepDetector;
#else
    unsigned int stepDetector = static_cast<unsigned int>(data.u64.stepDetector);
    if (stepDetector == 1065353216)
        stepDetector = 1;
    d->value_ = stepDetector;
    sensordLogD() << "HybrisStepDetectorAdaptor: processSample() - stepDetector: " << data.u64.stepDetector << ", value_: " << d->value_;
#endif
#endif

    buffer->commit();
    buffer->wakeUpReaders();
}
