/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/
#ifndef MITKCOLORSEQUENCERAINBOW_H_URTESEINDEUTIGEKENNUNG_02
#define MITKCOLORSEQUENCERAINBOW_H_URTESEINDEUTIGEKENNUNG_02

#include "MitkDataTypesExtExports.h"
#include "mitkColorSequence.h"

namespace mitk
{
  class MITKDATATYPESEXT_EXPORT ColorSequenceRainbow : public ColorSequence
  {
  public:
    ColorSequenceRainbow();
    ~ColorSequenceRainbow() override;

    /*!
    \brief method to return another color
    */
    Color GetNextColor() override;

    /*!
    \brief method to set the color-index to begin again
    */
    void GoToBegin() override;

  private:
    /*!
    \brief method that fills the pre-defnied colors
    */
    void InitColorList();

    /*!
    \brief global color-object that holds the actual color and changes to the next one
    */
    mitk::Color m_Color;

    /*!
    \brief color-index to iterate through the colors
    */
    unsigned int m_ColorIndex;

    /*!
    \brief vector-List with all pre-defined colors
    */
    std::vector<Color> m_ColorList;
  };
}

#endif
