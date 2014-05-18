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

#include "kcmpacmanrepoeditor.h"

#include "repoconf.h"
#include "checkboxdelegate.h"
#include "optionsdelegate.h"

#include <config.h>

#include <KFileDialog>
#include <QProcess>

#include <KAboutData>
#include <KPluginFactory>
#include <KMessageBox>
#include <KAuth/ActionWatcher>
using namespace KAuth;

K_PLUGIN_FACTORY(kcmpacmanrepoeditorFactory, registerPlugin<KcmPacmanRepoEditor>();)
K_EXPORT_PLUGIN(kcmpacmanrepoeditorFactory("KcmPacmanRepoEditor"))

KcmPacmanRepoEditor::KcmPacmanRepoEditor( QWidget *parent, const QVariantList &list )
    : KCModule( kcmpacmanrepoeditorFactory::componentData(), parent, list )
{
    // load translations
    KGlobal::locale()->insertCatalog("kcmpacmanrepoeditor");
    
    KAboutData *about = new KAboutData( "kcmpacmanrepoeditor", 
                                        "kcmpacmanrepoeditor", 
                                        ki18nc( "@title", "Chakra's Repositories Control Module" ), 
                                        KCM_PACMANREPOEDITOR_VERSION, 
                                        ki18nc( "@title", "A KDE Control Module for Chakra's repositories management." ), 
                                        KAboutData::License_GPL_V3, ki18nc( "@info:credit", "Copyright (C) 2014 Giuseppe Calà" ), 
                                        KLocalizedString(), "https://github.com/gcala/kcmpacmanrepoeditor" );
    about->addAuthor( ki18nc( "@info:credit", "Giuseppe Calà" ), 
                      ki18nc( "@info:credit", "Main Developer" ), 
                      "jiveaxe@gmail.com" );
    setAboutData( about );  
    ui.setupUi( this );
    setNeedsAuthorization( true );
  
    repoConf = new RepoConf();
    addRepoDialog = new AddRepo( this );

    ui.tableView->setModel( repoConf );
    
    CheckBoxDelegate *cbd = new CheckBoxDelegate( this );
    connect( cbd, SIGNAL(toggled(bool)), SIGNAL(changed(bool)) );
    
    ui.tableView->setItemDelegateForColumn( 0, cbd );
    ui.tableView->setItemDelegateForColumn( 2, new OptionsDelegate( this) );

    ui.tableView->setColumnWidth( 1, 133 );

    connect( ui.moveUp, SIGNAL( clicked() ),
                        SLOT( moveUp() ) );

    connect( ui.moveDown, SIGNAL( clicked() ),
                          SLOT( moveDown() ) );

    QItemSelectionModel *selModel = ui.tableView->selectionModel();
    
    connect( selModel, SIGNAL( selectionChanged(QItemSelection,QItemSelection) ),
                       SLOT( updateMovers(QItemSelection,QItemSelection) ) );

    connect( ui.remove, SIGNAL( clicked() ),
                        SLOT( removeEntry() ) );

    connect( ui.add, SIGNAL( clicked() ),
                     SLOT( addEntry() ) );

    connect( ui.edit, SIGNAL( clicked() ),
                      SLOT( editEntry() ) );

    connect( ui.loadBackup, SIGNAL( clicked() ),
                            SLOT( loadBackup() ) );

    ui.tableView->selectRow( 0 );
    
    ui.edit->setEnabled(false);  
    ui.moveDown->setEnabled(false);
    ui.remove->setEnabled(false);
  
    // Use kde4-config to get kde prefix
    kdeConfig = new QProcess(this);
    connect(kdeConfig, SIGNAL(readyReadStandardOutput()), this, SLOT(slotKdeConfig()));
    kdeConfig->start("kde4-config", QStringList() << "--prefix");
}

void KcmPacmanRepoEditor::loadBackup()
{
    RepoConf conf;
    QString file = KFileDialog::getOpenFileName( KUrl("/etc"),
                                                 "*.*",
                                                 this,
                                                 i18n("Select backup file")  
                                               );
    if( file.isEmpty() )
        return;

    conf.loadConf( file );

    if( !conf.count() ) {
        KMessageBox::error( this, i18n( "Can't load backup file.\nSelected file is not valid." ) );
    } else {
        repoConf->loadConf( file );
        emit changed(true);
    }
}

void KcmPacmanRepoEditor::addEntry()
{
    addRepoDialog->setRepoName("");
    addRepoDialog->setRepoLocation("");
    if( addRepoDialog->exec() == QDialog::Accepted ) {
        if( !repoConf->exists( addRepoDialog->entry.getName() ) ) {
            repoConf->addEntry( addRepoDialog->entry );
            emit changed(true);
        }
    }
}

void KcmPacmanRepoEditor::editEntry()
{
    QModelIndex repoMI = ui.tableView->model()->index(ui.tableView->currentIndex().row(), 1, QModelIndex());
    QModelIndex locationMI = ui.tableView->model()->index(ui.tableView->currentIndex().row(), 2, QModelIndex());

    addRepoDialog->setRepoName(ui.tableView->model()->data(repoMI).toString());

    // take the location
    QString location = ui.tableView->model()->data(locationMI).toString();

    // take the type and address
    QRegExp locationMatch("^(Server|Include)\\s*=\\s*(.+)");
    locationMatch.indexIn(location);
    location = locationMatch.cap(2);

    // fill remaining fields
    addRepoDialog->setRepoLocation(location);
    addRepoDialog->setLocationType(locationMatch.cap(1) == "Server" ? 1 : 0);

    if( addRepoDialog->exec() == QDialog::Accepted ) {
        ui.tableView->model()->setData( repoMI, addRepoDialog->getRepoName() );
        ui.tableView->model()->setData( locationMI, ( addRepoDialog->getLocationType() == 0 ? "Include = " : "Server = " ) + addRepoDialog->getRepoLocation() );
        emit changed(true);
    }
}

void KcmPacmanRepoEditor::updateMovers( const QItemSelection &cur, const QItemSelection &pre)
{
    Q_UNUSED(pre)
    QModelIndexList list = cur.indexes();

    if( !list.count() )
        return;
    
    // enable edit/remove buttons
    ui.edit->setEnabled(true);
    ui.remove->setEnabled(true);

    int row = list.at( 0 ).row();

    if( row > 0 )
        ui.moveUp->setEnabled( true );
    else
        ui.moveUp->setEnabled( false );

    if( row < ui.tableView->model()->rowCount() - 1)
        ui.moveDown->setEnabled( true );
    else
        ui.moveDown->setEnabled( false );
}

void KcmPacmanRepoEditor::moveUp()
{
    QModelIndexList list = ui.tableView->selectionModel()->selectedRows();

    if( !list.count() )
        return;

    int row = list.at( 0 ).row();
    repoConf->moveUp( row );
    ui.tableView->selectRow( row - 1 );
    emit changed(true);
}

void KcmPacmanRepoEditor::moveDown()
{
    QModelIndexList list = ui.tableView->selectionModel()->selectedRows();

    if( !list.count() )
        return;

    int row = list.at( 0 ).row();
    repoConf->moveDown( row );
    ui.tableView->selectRow( row + 1 );
    emit changed(true);
}

void KcmPacmanRepoEditor::removeEntry()
{
    QModelIndexList list = ui.tableView->selectionModel()->selectedRows();

    if( !list.count() )
        return;

    int row = list.at( 0 ).row();
    repoConf->removeRow( row );
    emit changed(true);
    if(ui.tableView->model()->rowCount() == 0) {
        ui.edit->setEnabled(false);
        ui.remove->setEnabled(false);
        ui.moveDown->setEnabled(false);
        ui.moveUp->setEnabled(false);
    }   
}

void KcmPacmanRepoEditor::defaults()
{
    repoConf->loadConf();
}

void KcmPacmanRepoEditor::load()
{
    repoConf->loadConf();
}

void KcmPacmanRepoEditor::save()
{
    QString pacmanConfFileContents = repoConf->toString();
    
    QVariantMap helperArgs;
    helperArgs["pacmanConfFileContents"] = pacmanConfFileContents;
    
    // Call the helper to write the configuration files
    Action *saveAction = authAction();      
    saveAction->setArguments(helperArgs);
    ActionReply reply = saveAction->execute();
    
    // Respond to reply of the helper
    if(reply.failed())
    { // Writing the configuration files failed
        if (reply.type() == ActionReply::KAuthError) // Authorization error
            KMessageBox::error(this, i18n("Unable to authenticate/execute the action: code %1", reply.errorCode()));
        else // Other error
            KMessageBox::error(this, i18n("Unable to write the (%1) file:\n%2", reply.data()["filename"].toString(), reply.data()["errorDescription"].toString()));
    } else // Writing succeeded
        KMessageBox::information(this, i18n("/etc/pacman.conf successfully saved"));
}

void KcmPacmanRepoEditor::slotKdeConfig()
{
  // Set a QString containing the kde prefix
  kdePrefix = QString::fromAscii(kdeConfig->readAllStandardOutput()).trimmed();
}
