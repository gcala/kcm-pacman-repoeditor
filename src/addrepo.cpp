/*
Copyright 2011 Simone Tobia

This file is part of AppSet.

AppSet is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

AppSet is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with AppSet; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#include "addrepo.h"
#include "ui_addrepo.h"

#include <KMessageBox>
#include <QFileDialog>
#include <QUrl>

AddRepo::AddRepo( QWidget *parent ) :
    KDialog(parent),
    ui(new Ui::AddRepo)
{
    ui->setupUi( this );

    connect( ui->buttonBox, SIGNAL( accepted() ),
                            SLOT( checkAndApply() ) );

    connect( ui->locationCB, SIGNAL( currentIndexChanged( int ) ),
                             SLOT( locationChanged( int ) ) );

    connect( ui->selectLocalDirectory, SIGNAL( clicked() ),
                                       SLOT( selectLocalRepository() ) );
    connect( ui->selectServerList, SIGNAL( clicked() ),
                                   SLOT( selectServerList() ) );

    ui->locationCB->setCurrentIndex( 1 ); // 0 : Include, 1 : Server
}

AddRepo::~AddRepo()
{
    delete ui;
}

void AddRepo::setRepoName( const QString &name )
{
    ui->repo->setText( name );
}

void AddRepo::setRepoLocation( const QString &location )
{
    ui->location->setText( location );
}

void AddRepo::setLocationType( int type)
{
    ui->locationCB->setCurrentIndex( type );
}

QString AddRepo::getRepoName() const
{
    return ui->repo->text().trimmed();
}

QString AddRepo::getRepoLocation() const
{
    return ui->location->text().trimmed();
}

int AddRepo::getLocationType() const
{
    return ui->locationCB->currentIndex();
}


void AddRepo::checkAndApply()
{
    if( ui->repo->text().remove( RepoConf::commentString ).trimmed().isEmpty() ) {
        KMessageBox::error( this, i18n( "Can't add repository.\nThe repository name field can't be blank." ) );
    } else if ( RepoConf::matchRepo( RepoEntry::formatRepoName( ui->repo->text() ) ) ) {
        QString prefix = "";

        QString location = ui->locationCB->currentText() + " = " + ui->location->text();
        bool valid = true;

        if( !RepoConf::matchRepoDetails( location ) )
            valid = false;

        if( valid ) {
            entry.setName( ui->repo->text() );
            entry.setDetails( QStringList() << location );
            done( QDialog::Accepted );
        } else {
            KMessageBox::error( this, i18n( "Can't add repository.\nThe repository location field is not valid." ) );
        }
    } else {
        KMessageBox::error( this, i18n( "Can't add repository.\nThe repository name field is not valid." ) );
    }
}

void AddRepo::locationChanged( int )
{
    if( ui->locationCB->currentText() == "Server" ) {
        ui->location->setPlaceholderText( i18n( "Address of remote or local packages repository" ) );
        ui->selectLocalDirectory->setVisible( true );
        ui->selectServerList->setVisible( false );
    } else {
        ui->location->setPlaceholderText( i18n( "Path to mirrors list file" ) );
        ui->selectLocalDirectory->setVisible( false );
        ui->selectServerList->setVisible( true );
    }
}

void AddRepo::selectLocalRepository()
{
    QStringList list;
    QFileDialog dialog( this );
    dialog.setDirectory( "/" );
    dialog.setWindowTitle( i18n( "Select local repository" ) );
    dialog.setFileMode( QFileDialog::Directory );
    if( dialog.exec() )
        list = dialog.selectedFiles();
    if( list.count() > 0 )
        ui->location->setText( ( ui->locationCB->currentIndex() == 1 ? QLatin1String( "file://" ) : QString() ) + list.at( 0 ) );
}

void AddRepo::selectServerList()
{
    QStringList list;
    QFileDialog dialog( this );
    dialog.setDirectory( "/" );
    dialog.setWindowTitle( i18n( "Select mirrors list" ) );
    dialog.setFileMode( QFileDialog::AnyFile );
    if ( dialog.exec() )
        list = dialog.selectedFiles();
    if( list.count() > 0 )
        ui->location->setText( list.at( 0 ) );
}

void AddRepo::resizeEvent( QResizeEvent * event )
{
    ui->selectLocalDirectory->setFixedHeight( ui->location->height() );
    ui->selectServerList->setFixedHeight( ui->location->height() );
    QDialog::resizeEvent( event );
}
