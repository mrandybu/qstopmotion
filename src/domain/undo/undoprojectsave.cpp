/******************************************************************************
 *  Copyright (C) 2005-2012 by                                                *
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

#include "undoprojectsave.h"


UndoProjectSave::UndoProjectSave(DomainFacade *df,
                                 const QString &path,
                                 bool sa)
    :UndoBase(df)
{
    projectPath.append(path);
    saveAs = sa;
    setText(QString(QObject::tr("Save project '%1'")).arg(projectPath));
}


UndoProjectSave::~UndoProjectSave()
{

}


void UndoProjectSave::undo()
{
    // TODO: Change handling for undo
    // facade->saveProjectUndo(projectPath, saveAs);
}


void UndoProjectSave::redo()
{
    facade->saveProjectRedo(projectPath, saveAs);
    facade->writeHistoryEntry(QString("redoProjectSave|%1").arg(projectPath));
}