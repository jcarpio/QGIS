/***************************************************************************
    qgsstylegroupselectiondialog.h
    ---------------------
    begin                : Oct 2015
    copyright            : (C) 2015 by Alessandro Pasotti
    email                : elpaso at itopen dot it

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "qgsstylegroupselectiondialog.h"
#include "qgsstyle.h"

#include <QStandardItemModel>
#include <QStandardItem>


QgsStyleGroupSelectionDialog::QgsStyleGroupSelectionDialog( QgsStyle *style, QWidget *parent )
    : QDialog( parent )
    , mStyle( style )
{
  setupUi( this );

  QStandardItemModel* model = new QStandardItemModel( groupTree );
  groupTree->setModel( model );

  QStandardItem *allSymbols = new QStandardItem( tr( "All Symbols" ) );
  allSymbols->setData( "all", Qt::UserRole + 2 );
  allSymbols->setEditable( false );
  setBold( allSymbols );
  model->appendRow( allSymbols );

  QStandardItem *group = new QStandardItem( QLatin1String( "" ) ); //require empty name to get first order groups
  group->setData( "groupsheader", Qt::UserRole + 2 );
  group->setEditable( false );
  group->setFlags( group->flags() & ~Qt::ItemIsSelectable );
  buildGroupTree( group );
  group->setText( tr( "Groups" ) );//set title later
  QStandardItem *ungrouped = new QStandardItem( tr( "Ungrouped" ) );
  ungrouped->setData( 0 );
  ungrouped->setData( "group", Qt::UserRole + 2 );
  setBold( ungrouped );
  setBold( group );
  group->appendRow( ungrouped );
  model->appendRow( group );

  QStandardItem *tag = new QStandardItem( tr( "Smart Groups" ) );
  tag->setData( "smartgroupsheader" , Qt::UserRole + 2 );
  tag->setEditable( false );
  tag->setFlags( group->flags() & ~Qt::ItemIsSelectable );
  setBold( tag );
  QgsSymbolGroupMap sgMap = mStyle->smartgroupsListMap();
  QgsSymbolGroupMap::const_iterator i = sgMap.constBegin();
  while ( i != sgMap.constEnd() )
  {
    QStandardItem *item = new QStandardItem( i.value() );
    item->setEditable( false );
    item->setData( i.key() );
    item->setData( "smartgroup" , Qt::UserRole + 2 );
    tag->appendRow( item );
    ++i;
  }
  model->appendRow( tag );

  // expand things in the group tree
  int rows = model->rowCount( model->indexFromItem( model->invisibleRootItem() ) );
  for ( int i = 0; i < rows; i++ )
  {
    groupTree->setExpanded( model->indexFromItem( model->item( i ) ), true );
  }
  connect( groupTree->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( groupTreeSelectionChanged( const QItemSelection&, const QItemSelection& ) ) );
}


QgsStyleGroupSelectionDialog::~QgsStyleGroupSelectionDialog()
{
}


void QgsStyleGroupSelectionDialog::setBold( QStandardItem* item )
{
  QFont font = item->font();
  font.setBold( true );
  item->setFont( font );
}


void QgsStyleGroupSelectionDialog::groupTreeSelectionChanged( const QItemSelection &selected, const QItemSelection &deselected )
{
  QModelIndex index;
  QModelIndexList selectedItems = selected.indexes();
  QModelIndexList deselectedItems = deselected.indexes();

  Q_FOREACH ( index, deselectedItems )
  {
    if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "groupsheader" ) )
    {
      // Ignore: it's the group header
    }
    else if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "all" ) )
    {
      emit allDeselected();
    }
    else if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "smartgroupsheader" ) )
    {
      // Ignore: it's the smartgroups header
    }
    else if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "smartgroup" ) )
    {
      emit smartgroupDeselected( index.data().toString() );
    }
    else if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "group" ) )
    { // It's a group
      emit groupDeselected( index.data().toString() );
    }
  }
  Q_FOREACH ( index, selectedItems )
  {
    if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "groupsheader" ) )
    {
      // Ignore: it's the group header
    }
    else if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "all" ) )
    {
      emit allSelected();
    }
    else if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "smartgroupsheader" ) )
    {
      // Ignore: it's the smartgroups header
    }
    else if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "smartgroup" ) )
    {
      emit smartgroupSelected( index.data().toString() );
    }
    else if ( index.data( Qt::UserRole + 2 ).toString() == QLatin1String( "group" ) )
    {  // It's a group
      emit groupSelected( index.data().toString() );
    }
  }
}


void QgsStyleGroupSelectionDialog::buildGroupTree( QStandardItem* &parent )
{
  QgsSymbolGroupMap groups = mStyle->childGroupNames( parent->text() );
  QgsSymbolGroupMap::const_iterator i = groups.constBegin();
  while ( i != groups.constEnd() )
  {
    QStandardItem *item = new QStandardItem( i.value() );
    item->setData( i.key() );
    item->setData( "group" , Qt::UserRole + 2 );
    item->setEditable( false );
    parent->appendRow( item );
    buildGroupTree( item );
    ++i;
  }
}

