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

#include "addrepo.h"
#include "ui_addrepo.h"

#include <KMessageBox>

#include <QFileDialog>
#include <QUrl>

AddRepo::AddRepo( QWidget *parent ) :
    QDialog(parent),
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
    setWindowIcon( QIcon::fromTheme( QLatin1String( "database-chakra" ) ) );
    
    ui->repo->setClearButtonEnabled( true );
    ui->location->setClearButtonEnabled( true );
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
        displayMsgWidget(KMessageWidget::Error, i18n("The repository name field can't be blank"));
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
            displayMsgWidget(KMessageWidget::Error, i18n("The repository location field is not valid"));
        }
    } else {
        displayMsgWidget(KMessageWidget::Error, i18n("The repository name field is not valid"));
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
    QString path = QFileDialog::getExistingDirectory( this,
                                                      i18n("Select local repository"),
                                                      "/",
                                                      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks );
    if( path.isEmpty() )
        return;
    
    ui->location->setText( ( ui->locationCB->currentIndex() == 1 ? QLatin1String( "file://" ) : QString() ) + path );
}

void AddRepo::selectServerList()
{
    QString file = QFileDialog::getOpenFileName( this,
                                                 i18n("Select mirrors list"),
                                                 "/",
                                                 i18n("All files (*.*)")                                                 
                                               );
    if( file.isEmpty() )
        return;
    
    ui->location->setText( file );
}

void AddRepo::resizeEvent( QResizeEvent * event )
{
    ui->selectLocalDirectory->setFixedHeight( ui->location->height() );
    ui->selectServerList->setFixedHeight( ui->location->height() );
    QDialog::resizeEvent( event );
}

void AddRepo::displayMsgWidget(KMessageWidget::MessageType type, QString msg)
{
  KMessageWidget *msgWidget = new KMessageWidget;
  msgWidget->setText(msg);
  msgWidget->setMessageType(type);
  ui->verticalLayoutMsg->insertWidget(0, msgWidget);
  msgWidget->animatedShow();
}
