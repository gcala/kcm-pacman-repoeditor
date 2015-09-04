/*******************************************************************************
 * Copyright (C) 2014-2015 Giuseppe Calà <jiveaxe@gmail.com>                   *
 *                                                                             *
 * This program is free software: you can redistribute it and/or modify it     *
 * under the terms of the GNU General Public License as published by the Free  *
 * Software Foundation, either version 3 of the License, or (at your option)   *
 * any later version.                                                          *
 *                                                                             *
 * This program is distributed in the hope that it will be useful, but WITHOUT *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for    *
 * more details.                                                               *
 *                                                                             *
 * You should have received a copy of the GNU General Public License along     *
 * with this program. If not, see <http://www.gnu.org/licenses/>.              *
 *******************************************************************************/

#include "helper.h"

#include "../config.h"

#include <QFile>
#include <QTextStream>
#include <QDateTime>

ActionReply Helper::save(QVariantMap args)
{
    ActionReply reply;
  
    // Declare QStrings with file names and contents
    const QString &pacmanConfFileContents = args["pacmanConfFileContents"].toString();

    // backup file
    const QString &suffix = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    if(!QFile::copy("/etc/pacman.conf", "/etc/pacman.conf-" + suffix)) {
        reply = ActionReply::HelperErrorReply();
        reply.addData("errorDescription", "Unable to create backup file");
        //reply.setErrorCode(pacmanFile.error());
        reply.addData("filename", "/etc/pacman.conf-" + suffix);
        return reply;
    }
    
    // write pacman.conf
    QFile pacmanFile("/etc/pacman.conf");
    if (!pacmanFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        reply = ActionReply::HelperErrorReply();
        reply.addData("errorDescription", pacmanFile.errorString());
//         reply.setErrorCode(pacmanFile.error());
        reply.addData("filename", "pacman.conf");
        return reply;
    }
    
    QTextStream sysStream(&pacmanFile);
    sysStream << pacmanConfFileContents;
    pacmanFile.close();
  
  // return a reply
  return reply;
}

KAUTH_HELPER_MAIN("org.kde.kcontrol.kcmpacmanrepoeditor", Helper)