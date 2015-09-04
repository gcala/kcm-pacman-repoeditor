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

#include "repoentry.h"

QRegExp RepoEntry::nameFilter = QRegExp( "" );
QString RepoEntry::commentString = "";
QString RepoEntry::repoFormat = "";

RepoEntry::RepoEntry()
    : valid( false )
    , active( false )
{
}

RepoEntry::RepoEntry( QString name, bool active )
    : name( name )
    , valid( true )
    , active( active )
{
}

QString RepoEntry::getName() const
{
    if( !nameFilter.isValid() || !isValid() )
        return "";

    QString ret = name;

    return ret.remove( nameFilter );
}

void RepoEntry::addDetail( const QString & detail )
{
    details << detail;
}

void RepoEntry::setDetails( const QStringList &details )
{
    this->details = details;
}

void RepoEntry::addComment( const QString &comment )
{
    this->comments << comment;
}

void RepoEntry::setName( const QString &name )
{
    this->name = formatRepoName( name );
    valid = true;
}

QString RepoEntry::toString() const
{
    QStringList list,list2,retList;

    if( !isValid() )
        return "";

    if( comments.isEmpty() )
        list << ( active ? name : commentString + name );
    else
        list << comments.join( "\n" ) << name;

    if( detailsComments.isEmpty() ) {
        if( !active && !details.isEmpty() ) {
            QStringList detailsCopy = details;
            detailsCopy[0] = commentString + details.at( 0 );
            list2 << detailsCopy;
        } else
            list2 << details;
    } else
        list2 << detailsComments.join( "\n" ) << details;

    retList <<  list.join( QString( "\n" ) + ( active ? "" : commentString ) );
    retList << list2.join( QString( "\n" ) + ( active ? "" : commentString ) );

    return retList.join( "\n" );
}

void RepoEntry::setRealName( const QString &realName )
{
    name = realName;
    valid = true;
}

bool RepoEntry::setActive( bool active )
{
    if( !valid )
        return false;

    this->active = active;

    return true;
}
