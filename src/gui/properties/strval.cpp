/***************************************************************************
 *   Copyright (C) 2021 by Santiago González                               *
 *                                                                         *
 ***( see copyright.txt file at root folder )*******************************/

#include "strval.h"
#include "component.h"
#include "comproperty.h"
//#include "mainwindow.h"
#include "propdialog.h"

StrVal::StrVal( PropDialog* parent, CompBase* comp, ComProperty* prop )
      : PropVal( parent, comp, prop )
{
    setupUi(this);
}
StrVal::~StrVal() {}

void StrVal::setup( bool )
{
    //float scale = MainWindow::self()->fontScale();
    //QFont font = valLabel->font();
    //font.setPixelSize( 11.0*scale );
    //valLabel->setFont( font );

    QFontMetrics fm( valLabel->font() );
    float scale = fm.horizontalAdvance(" ")/2;
    value->setFixedWidth( 192.0*scale );

    valLabel->setText( m_property->capt() );
    updtValues();
    this->adjustSize();
}

void StrVal::on_value_editingFinished()
{
    prepareChange();
    m_property->setValStr( value->text() );
    saveChanges();
}

void StrVal::updtValues()
{
    if( m_blocked ) return;
    m_blocked = true;

    value->setText( m_property->getValStr() );

    m_blocked = false;
}

void StrVal::setFocusToInput()
{
    value->setFocus();
    value->selectAll();
}
