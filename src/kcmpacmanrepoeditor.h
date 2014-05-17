/*******************************************************************************
 * Copyright (C) 2014 Giuseppe Calà <jiveaxe6@gmail.com>                       *
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

#ifndef KCMPACMANREPOEDITOR_H
#define KCMPACMANREPOEDITOR_H

#include "repoconf.h"
#include "addrepo.h"

#include <KCModule>
#include "ui_kcmpacmanrepoeditor.h"

#include <QItemSelection>

class QProcess;

class KcmPacmanRepoEditor : public KCModule
{
  Q_OBJECT

  public:
    explicit KcmPacmanRepoEditor(QWidget *parent = 0, const QVariantList &list = QVariantList());
    
    RepoConf *repoConf;
    AddRepo *addRepoDialog;
    
    QString toString() const {
        return repoConf->toString();
    }
    
    void defaults();
    void save();
        
  private:
    Ui::KcmPacmanRepoEditor ui;
    QProcess *kdeConfig;
    QString kdePrefix;

  private slots:
    void slotKdeConfig();
    
    void moveUp();
    void moveDown();

    void updateMovers( const QItemSelection &, const QItemSelection & );

    void removeEntry();
    void addEntry();
    void editEntry();

    void loadBackup();
};

#endif // KCMPACMANREPOEDITOR_H
