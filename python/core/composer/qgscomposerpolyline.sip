class QgsComposerPolyline: QgsComposerNodesItem
{
%TypeHeaderCode
#include <qgscomposerpolyline.h>
%End
  public:

    QgsComposerPolyline( QgsComposition* c );
    QgsComposerPolyline( const QPolygonF& polyline, QgsComposition* c );
    ~QgsComposerPolyline();

    /** Overridden to return shape name */
    virtual QString displayName() const;

    /** Returns the QgsSymbol used to draw the shape. */
    QgsLineSymbol* polylineStyleSymbol();

    /** Set the QgsSymbol used to draw the shape. */
    void setPolylineStyleSymbol( QgsLineSymbol* symbol );

    /** Overridden to return shape type */
    virtual int type() const;

  protected:

    bool _addNode( const int indexPoint, QPointF newPoint, const double radius );

    bool _removeNode( const int indexPoint );

    /** Draw points for the current shape. */
    void _draw( QPainter *painter );

    /** Read symbol in XML. */
    void _readXmlStyle( const QDomElement &elmt );

    /** Write the symbol in an XML document. */
    void _writeXmlStyle( QDomDocument &doc, QDomElement &elmt ) const;

    /** Create a default symbol. */
    void createDefaultPolylineStyleSymbol();
};

