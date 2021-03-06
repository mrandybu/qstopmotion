/******************************************************************************
 *  Copyright (C) 2005-2015 by                                                *
 *    Bjoern Erik Nilsen (bjoern.nilsen@bjoernen.com),                        *
 *    Fredrik Berg Kjoelstad (fredrikbk@hotmail.com),                         *
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

#include "undoscenemove.h"

#include <QDebug>
#include <QLatin1String>


UndoSceneMove::UndoSceneMove(DomainFacade *df,
                             int           fsi,
                             int           tsi)
    :UndoBase(df)
{
    fromSceneIndex = fsi;
    toSceneIndex = tsi;
    undoFlag = false;
    setText(QString(tr("Move scene (%1,%2)")).arg(fromSceneIndex).arg(toSceneIndex));
}


UndoSceneMove::~UndoSceneMove()
{
}


void UndoSceneMove::undo()
{
    qDebug() << "UndoSceneMove::undo --> Start";

    AnimationProject *animationProject = facade->getAnimationProject();

    animationProject->decAnimationChanges();

    facade->writeHistoryEntry(QLatin1String("undo"));
    undoFlag = true;

    qDebug() << "UndoSceneMove::undo --> Start";
}


void UndoSceneMove::redo()
{
    qDebug() << "UndoSceneMove::redo --> Start";

    AnimationProject *animationProject = facade->getAnimationProject();

    animationProject->moveScene(fromSceneIndex, toSceneIndex);
    animationProject->incAnimationChanges();

    if (undoFlag) {
        facade->writeHistoryEntry(QLatin1String("redo"));
        undoFlag = false;
    }
    else {
        facade->writeHistoryEntry(QString("%1|%2|%3")
                                  .arg(QLatin1String("redoSceneMove"))
                                  .arg(fromSceneIndex)
                                  .arg(toSceneIndex));
    }

    qDebug() << "UndoSceneMove::redo --> Start";
}
