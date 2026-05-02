#ifndef SCHMITTTRIGGER_H
#define SCHMITTTRIGGER_H

#include "component.h"
#include "iocomponent.h"
#include "e-element.h"

class LibraryItem;

class SchmittTrigger : public IoComponent, public eElement
{
    public:
        SchmittTrigger( QString type, QString id );
        ~SchmittTrigger();

        void stamp() override;
        void voltChanged() override;
        void runEvent() override { IoComponent::runOutputs(); }

        double vth() { return m_vth; }
        void setVth( double v );

        double vtl() { return m_vtl; }
        void setVtl( double v );

        QPainterPath shape() const override;
        void paint( QPainter* p, const QStyleOptionGraphicsItem* o, QWidget* w ) override;

        static Component* construct( QString type, QString id );
        static LibraryItem* libraryItem();

    private:
        double m_vth;
        double m_vtl;
        bool m_outputState;
};

#endif // SCHMITTTRIGGER_H
