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

#ifndef ADDREPO_H
#define ADDREPO_H

#include <QDialog>
#include <KMessageWidget>

#include "repoconf.h"
#include "repoentry.h"

namespace Ui {
    class AddRepo;
}

class AddRepo : public QDialog
{
    Q_OBJECT

public:
    explicit AddRepo( QWidget *parent = 0 );
    ~AddRepo();

    RepoEntry entry;

    void setRepoName( const QString & );
    void setRepoLocation( const QString & );
    void setLocationType( int );

    QString getRepoName() const;
    QString getRepoLocation() const;
    int getLocationType() const;
    
protected:
    void resizeEvent( QResizeEvent * event );

private:
    Ui::AddRepo *ui;
    QString examples;
    void displayMsgWidget(KMessageWidget::MessageType type, QString msg);

private slots:
    void checkAndApply();
    void locationChanged( int );
    void selectLocalRepository();
    void selectServerList();
};

#endif // ADDREPO_H
