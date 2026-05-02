#include <QPainter>

#include "schmitttrigger.h"
#include "itemlibrary.h"
#include "iopin.h"
#include "simulator.h"

#include "doubleprop.h"

#define tr(str) simulideTr("SchmittTrigger",str)

Component* SchmittTrigger::construct( QString type, QString id )
{ return new SchmittTrigger( type, id ); }

LibraryItem* SchmittTrigger::libraryItem()
{
    return new LibraryItem(
        tr("Schmitt Trigger"),
        "Other Active",
        "schmitttrigger.svg",
        "SchmittTrigger",
        SchmittTrigger::construct );
}

SchmittTrigger::SchmittTrigger( QString type, QString id )
          : IoComponent( type, id )
          , eElement( id )
          , m_vth( 2.5 )
          , m_vtl( 1.5 )
          , m_outputState( false )
{
    m_width = 4;
    setNumInps( 1,"", 0 );
    setNumOuts( 1,"",-1 );

    m_area = QRect(-18,-8*2, 36, 8*2*2 );
    setLabelPos(-16,-32, 0);

    m_inpPin[0]->setPos( QPoint(-16-8, 0) );
    m_inpPin[0]->setFontSize( 9 );
    m_inpPin[0]->setSpace( 1.7 );
    m_inpPin[0]->setLabelText("Vin");
    m_inpPin[0]->setLabelColor( Qt::red );

    m_outPin[0]->setPos( QPoint( 16+8, 0) );

    addPropGroup( { tr("Electric"),
        IoComponent::outputProps()
       +IoComponent::outputType()
        , 0 } );

    addPropGroup( { tr("Timing"), IoComponent::edgeProps(),0 } );

    addPropGroup( { tr("Hysteresis"), {
        new DoubProp<SchmittTrigger>("Vth", tr("Vth"), "V"
                              , this, &SchmittTrigger::vth, &SchmittTrigger::setVth ),
        new DoubProp<SchmittTrigger>("Vtl", tr("Vtl"), "V"
                              , this, &SchmittTrigger::vtl, &SchmittTrigger::setVtl )
    }, 0 } );
}

SchmittTrigger::~SchmittTrigger(){}

void SchmittTrigger::stamp()
{
    IoComponent::initState();
    for( uint i=0; i<m_inpPin.size(); ++i ) m_inpPin[i]->changeCallBack( this );
}

void SchmittTrigger::voltChanged()
{
    double vin = m_inpPin[0]->getVoltage();

    if( !m_outputState && vin > m_vth )
    {
        m_outputState = true;
        m_nextOutVal = 1;
    }
    else if( m_outputState && vin < m_vtl )
    {
        m_outputState = false;
        m_nextOutVal = 0;
    }
    scheduleOutPuts( this );
}

QPainterPath SchmittTrigger::shape() const
{
    QPainterPath path;

    QVector<QPointF> points;

    points << QPointF(-16,-16 )
           << QPointF(-16, 16 )
           << QPointF( 16,  1 )
           << QPointF( 16, -1 );

    path.addPolygon( QPolygonF(points) );
    path.closeSubpath();
    return path;
}

void SchmittTrigger::paint( QPainter* p, const QStyleOptionGraphicsItem* o, QWidget* w )
{
    Component::paint( p, o, w );

    QPen pen = p->pen();
    pen.setWidth(2);
    p->setPen(pen);

 static const QPointF points[4] = {
        QPointF(-16,-16 ),
        QPointF(-16, 16 ),
        QPointF( 16,  1 ),
        QPointF( 16, -1 )            };

    p->drawPolygon(points, 4);

    // Draw hysteresis symbol (two parallel lines)
    p->drawLine( QPointF(-8, -4), QPointF(-8, 4) );
    p->drawLine( QPointF(-4, -4), QPointF(-4, 4) );

    Component::paintSelected( p );
}

void SchmittTrigger::setVth( double v )
{
    if( v > 0 ) m_vth = v;
}

void SchmittTrigger::setVtl( double v )
{
    if( v > 0 ) m_vtl = v;
}
