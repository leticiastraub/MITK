#ifndef MITKRENDERWINDOW_H_HEADER_INCLUDED_C1EBD0AD
#define MITKRENDERWINDOW_H_HEADER_INCLUDED_C1EBD0AD

#include <qgl.h>
#include <widget.h>

class iilItem;

namespace mitk {
//##ModelId=3E3ECC1201B2
//##Documentation
//## @brief Abstract window/widget class used for rendering
//## @ingroup Renderer
class RenderWindow : public RenderWindowBase
{
public:
    //##ModelId=3E3ECC130358
    RenderWindow(QGLFormat glf, QWidget *parent = 0, const char *name = 0) : RenderWindowBase(glf, parent, name)
    {
    };

    //##ModelId=3E3ECC13036D
    RenderWindow(QWidget *parent = 0, const char *name = 0) : RenderWindowBase(parent, name)
    {
    };

    //##ModelId=3EF1627602DF
    //##Documentation
    //## @brief Makes the renderwindow the current widget for 
    //## (e.g., if the renderwindow is an OpenGL-widget) 
    //## OpenGL operations, i.e. makes the widget's rendering context the current 
    //## OpenGL rendering context.
    virtual void MakeCurrent() {};

    //iil compatibility
    //##ModelId=3E3ECC130380
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual float x () const = 0;

    //##ModelId=3E3ECC130382
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual float y () const = 0;

    //##ModelId=3E3ECC130384
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual unsigned int imageWidth () const = 0;

    //##ModelId=3E3ECC13038B
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual unsigned int imageHeight () const = 0;

    //##ModelId=3E3ECC13038D
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual bool itemInViewport (const iilItem* const item) = 0;

    //##ModelId=3E3ECC2D02B6
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual int xOriginInDisplayUnits() const = 0;

    //##ModelId=3E3ECC2F0290
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual int yOriginInDisplayUnits() const = 0;

    //##ModelId=3E3ECC3101CB
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual unsigned int DisplayHeightInDisplayUnits() const = 0;

    //##ModelId=3E3ED0B003BB
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
	virtual unsigned int curHeightInDisplayUnits() const {  return m_CurHeightInDisplayUnits; }

    //##ModelId=3E3ED4960370
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual float scaleFactorUnitsToDisplay() const = 0;

    //##ModelId=3E3F8970034D
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    virtual void pointUnitsToDisplayUnits(int & x, int& y) const = 0;

    //##ModelId=3E3ED0D301A8
    //##Documentation
    //## TEMPORARY FOR IIL COMPATIBILITY - DO NOT USE!!!
    unsigned int m_CurHeightInDisplayUnits;

};

} // namespace mitk

#endif /* MITKRENDERWINDOW_H_HEADER_INCLUDED_C1EBD0AD */

