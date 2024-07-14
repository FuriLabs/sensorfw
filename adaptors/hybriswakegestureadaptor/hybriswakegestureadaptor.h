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

#ifndef HYBRISWAKEGESTUREADAPTOR_H
#define HYBRISWAKEGESTUREADAPTOR_H
#include "hybrisadaptor.h"

#include <QString>
#include <QStringList>
#include "deviceadaptorringbuffer.h"
#include "datatypes/orientationdata.h"

/**
 * @brief Adaptor for hybris wake gesture sensor.
 *
 * Adaptor for wake gesture sensor.
 */
class HybrisWakeGestureAdaptor : public HybrisAdaptor
{
    Q_OBJECT

public:
    static DeviceAdaptor* factoryMethod(const QString& id) {
        return new HybrisWakeGestureAdaptor(id);
    }
    HybrisWakeGestureAdaptor(const QString& id);
    ~HybrisWakeGestureAdaptor();

    bool startSensor();
    void stopSensor();

protected:
    void processSample(const sensors_event_t& data);

private:
    DeviceAdaptorRingBuffer<TimedUnsigned>* buffer;
    int sensorType;
    QByteArray powerStatePath;

};
#endif
