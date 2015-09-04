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

#include "checkboxdelegate.h"

#include <QApplication>
#include <QCheckBox>

CheckBoxDelegate::CheckBoxDelegate( QObject *parent )
    : QStyledItemDelegate(parent)
{
}

void CheckBoxDelegate::paint( QPainter *painter,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index ) const
{
    bool value = index.data().toBool();
    QStyleOptionButton cbo;

    cbo.rect = option.rect;
    cbo.rect.setLeft( option.rect.center().x() - 10 );
    cbo.state = value ? QStyle::State_On : QStyle::State_Off;
    cbo.text = "";
    cbo.state = cbo.state | QStyle::State_Enabled;


    qApp->style()->drawControl( QStyle::CE_CheckBox,&cbo, painter );
}

bool CheckBoxDelegate::editorEvent( QEvent *event,
                                    QAbstractItemModel *model,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index )
{
    Q_UNUSED( option )
    if( event->type() == QEvent::MouseButtonRelease ) {
        emit toggled(true);
        return false;
    }

    model->setData( index, !index.data().toBool() );

    return event->type() == QEvent::MouseButtonDblClick ? true : false;
}
