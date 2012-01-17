/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
 *    Ralf Lange (ralf.lange@longsoft.de)                                     *
 *                                                                            *
 *  This program is free software; you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by      *
 *  the Free Software Foundation; either version 2 of the License, or         *
 *  (at your option) any later version.                                       *
 *                                                                            *
 *  This program is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 *  GNU General Public License for more details.                              *
 *                                                                            *
 *  You should have received a copy of the GNU General Public License         *
 *  along with this program; if not, write to the                             *
 *  Free Software Foundation, Inc.,                                           *
 *  59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                 *
 ******************************************************************************/

#include "undoexposureselect.h"


UndoExposureSelect::UndoExposureSelect(DomainFacade *df,
                                       int lastIndex,
                                       int newIndex)
    :UndoBase(df)
{
    lastExposureIndex = lastIndex;
    newExposureIndex = newIndex;
    setText(QObject::tr("Select exposure"));
}


UndoExposureSelect::~UndoExposureSelect()
{
}


void UndoExposureSelect::undo()
{
    /* TODO: Change handling for undo
    facade->undoSelectExposure(fromSceneIndex, fromTakeIndex, fromExposureIndex,
                               toSceneIndex, toTakeIndex, toExposureIndex);
    */
}


void UndoExposureSelect::redo()
{
    /* TODO: Change handling for redo
    facade->redoSelectExposure(fromSceneIndex, fromTakeIndex, fromExposureIndex,
                               toSceneIndex, toTakeIndex, toExposureIndex);
    */
}
