/**
   @file topedgeinterpreter.h
   @brief TopEdgeInterpreterFilter

   <p>
   Copyright (C) 2009-2010 Nokia Corporation

   @author Timo Rongas <ext-timo.2.rongas@nokia.com>

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

#ifndef TOPEDGEINTERPRETER_H
#define TOPEDGEINTERPRETER_H

#include <QObject>
#include "posedata.h"
#include "datatypes/unsigned.h"
#include "orientationdata.h"
#include "sensord/filter.h"
#include "sensord/filterproperty.h"

/**
 * @brief Filter for calculating the device topedge.
 *
 * Filter for calculating the device topedge. The orientation is given in
 * 2 axis and 2 directions per axis.
 *
 * Orientation of the face is handled in the #FaceInterpreter filter.
 */
class TopEdgeInterpreter : public QObject, public Filter<TimedXyzData, TopEdgeInterpreter, PoseData>, public PropertyTracker
{
    Q_OBJECT;

    /**
     * Holds the threshold value for the orientation calculation.
     */
    Q_PROPERTY(int threshold READ threshold_ WRITE threshold_);
    Q_PROPERTY(PoseData topEdge READ topEdge);

public:
    /**
     * Factory method.
     * @return New TopEdgeInterpreter instance as FilterBase*.
     */
    static FilterBase* factoryMethod()
    {
        return new TopEdgeInterpreter();
    }

    /**
     * Property method for getting the current orientation
     * @returns Current top edge.
     */
    PoseData topEdge() const
    {
        return pose;
    }


private:
    FilterProperty<int> threshold_;

    /**
     * Default constructor.
     */
    TopEdgeInterpreter();

    void interpret(unsigned, const TimedXyzData*);
    PoseData pose;
    PoseData newPose;
};

#endif // TOPEDGEINTERPRETER_H