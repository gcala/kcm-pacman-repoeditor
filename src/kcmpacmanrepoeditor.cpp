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

#include "kcmpacmanrepoeditor.h"

#include "repoconf.h"
#include "checkboxdelegate.h"
#include "optionsdelegate.h"

#include <config.h>

#include <QFileDialog>
#include <QProcess>

#include <KAboutData>
#include <KPluginFactory>
#include <KMessageBox>
#include <KAuth>

using namespace KAuth;

K_PLUGIN_FACTORY(kcmpacmanrepoeditorFactory, registerPlugin<KcmPacmanRepoEditor>();)

KcmPacmanRepoEditor::KcmPacmanRepoEditor( QWidget *parent, const QVariantList &args )
    : KCModule( parent, args )
{
    KAboutData *about = new KAboutData(QStringLiteral("kcmpacmanrepoeditor"),
                                     i18n("kcmpacmanrepoeditor"),
                                     KCM_PACMANREPOEDITOR_VERSION,
                                     i18n("A KDE Control Module for Chakra's repositories management"),
                                     KAboutLicense::GPL_V3,
                                     QStringLiteral("Copyright (C) 2014-2015 Giuseppe Calà"),
                                     QStringLiteral(),
                                     QStringLiteral("https://github.com/gcala/kcmpacmanrepoeditor"));
    
    about->addAuthor(QStringLiteral("Giuseppe Calà"), i18n("Main Developer"), QStringLiteral("jiveaxe@gmail.com"));
    
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
}

void KcmPacmanRepoEditor::loadBackup()
{
    RepoConf conf;
    QString file = QFileDialog::getOpenFileName( this,
                                                 i18n("Select backup file"),
                                                 "/etc",
                                                 i18n("All files (*.*)")                                                 
                                               );
    if( file.isEmpty() )
        return;

    conf.loadConf( file );

    if( !conf.count() ) {
        displayMsgWidget( KMessageWidget::Error, i18n( "Can't load backup file.\nSelected file is not valid." ) );
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

    Action saveAction = authAction();
    saveAction.setHelperId("org.kde.kcontrol.kcmpacmanrepoeditor");
    saveAction.setArguments(helperArgs);
    
    ExecuteJob *job = saveAction.execute();
    
    if (!job->exec())
        displayMsgWidget(KMessageWidget::Error, i18n("Unable to authenticate/execute the action: %1", job->error()));
    else
        displayMsgWidget(KMessageWidget::Positive, i18n("Repositories successfully written to /etc/pacman.conf"));
}

void KcmPacmanRepoEditor::displayMsgWidget(KMessageWidget::MessageType type, QString msg)
{
  KMessageWidget *msgWidget = new KMessageWidget;
  msgWidget->setText(msg);
  msgWidget->setMessageType(type);
  ui.verticalLayoutMsg->insertWidget(0, msgWidget);
  msgWidget->animatedShow();
}

#include "kcmpacmanrepoeditor.moc"