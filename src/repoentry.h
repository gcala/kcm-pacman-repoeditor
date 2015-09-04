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

#ifndef REPOENTRY_H
#define REPOENTRY_H

#include <QString>
#include <QStringList>
#include <QRegExp>

class RepoEntry{

public:
    explicit RepoEntry();
    explicit RepoEntry( QString name, bool active = false );

    static QRegExp nameFilter; //Contains the filter to obtain the unformatted repo name (e.g. [main] -> main)
    static QString commentString;
    static QString repoFormat;

    static QString formatRepoName( const QString & name ) {
        QString aux = repoFormat;
        return aux.replace("%repo%",name);
    }

    QString getName() const; //returns the unformatted repo name
    const QStringList & getDetails() const {
        return details;
    }

    void addDetail( const QString & detail );
    void setDetails( const QStringList & details );

    void addComment( const QString & comment );

    void setComments( const QStringList & comments ) {
        this->comments = comments;
    }

    void addComments( const QStringList & comments ) {
        this->comments << comments;
    }

    const QStringList & getComments() const {
        return comments;
    }

    void addDetailsComment( const QString & comment ) {
        detailsComments << comment;
    }

    const QStringList & getDetailsComments() const {
        return detailsComments;
    }

    void setDetailsComments( const QStringList & comments ) {
        detailsComments = comments;
    }

    const QString & getRealName(){
        return name;
    }

    void setRealName( const QString & realName );
    void setName( const QString & name );

    bool isValid() const {
        return valid;
    }

    bool isActive() const {
        return active;
    }

    bool setActive( bool active );

    QString toString() const;
    
private:
    QString name; //Contains the formatted name (e.g. [main])
    QStringList details;
    QStringList comments;
    QStringList detailsComments;

    bool valid;
    bool active;
};

#endif // REPOENTRY_H
