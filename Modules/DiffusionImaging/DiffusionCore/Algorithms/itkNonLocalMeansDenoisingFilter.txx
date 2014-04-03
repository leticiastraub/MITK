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

#ifndef __itkNonLocalMeansDenoisingFilter_txx
#define __itkNonLocalMeansDenoisingFilter_txx

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "itkImageRegionIterator.h"
#include "itkNeighborhoodIterator.h"
#include <itkImageRegionIteratorWithIndex.h>
#include <vector>

namespace itk {


template< class TPixelType >
NonLocalMeansDenoisingFilter< TPixelType >
::NonLocalMeansDenoisingFilter()
  : m_SearchRadius(4),
    m_ComparisonRadius(1),
    m_UseJointInformation(false),
    m_UseRicianAdaption(false),
    m_Variance(1),
    m_Mask(NULL)
{
  this->SetNumberOfRequiredInputs( 1 );
}

template< class TPixelType >
void
NonLocalMeansDenoisingFilter< TPixelType >
::BeforeThreadedGenerateData()
{
  MITK_INFO << "Variance: " << m_Variance;
  MITK_INFO << "SearchRadius: " << m_SearchRadius;
  MITK_INFO << "ComparisonRadius: " << m_ComparisonRadius;
  MITK_INFO << "Use Joint Information: " << std::boolalpha << m_UseJointInformation;
  MITK_INFO << "Use Rician Adaption: " << std::boolalpha << m_UseRicianAdaption;

  typename InputImageType::Pointer inputImagePointer = static_cast< InputImageType * >( this->ProcessObject::GetInput(0) );
  if (m_Mask.IsNull())
  {
    m_Mask = MaskImageType::New();
    m_Mask->SetRegions(inputImagePointer->GetLargestPossibleRegion());
    m_Mask->Allocate();
    m_Mask->FillBuffer(1);
  }
  else
  {
    ImageRegionIterator< MaskImageType > mit(m_Mask, m_Mask->GetLargestPossibleRegion());
    mit.GoToBegin();
    typename MaskImageType::IndexType minIndex;
    typename MaskImageType::IndexType maxIndex;
    minIndex.Fill(10000);
    maxIndex.Fill(0);
    while (!mit.IsAtEnd())
    {

      if (mit.Get())
      {
        minIndex[0] = minIndex[0] < mit.GetIndex()[0] ? minIndex[0] : mit.GetIndex()[0];
        minIndex[1] = minIndex[1] < mit.GetIndex()[1] ? minIndex[1] : mit.GetIndex()[1];
        minIndex[2] = minIndex[2] < mit.GetIndex()[2] ? minIndex[2] : mit.GetIndex()[2];

        maxIndex[0] = maxIndex[0] > mit.GetIndex()[0] ? maxIndex[0] : mit.GetIndex()[0];
        maxIndex[1] = maxIndex[1] > mit.GetIndex()[1] ? maxIndex[1] : mit.GetIndex()[1];
        maxIndex[2] = maxIndex[2] > mit.GetIndex()[2] ? maxIndex[2] : mit.GetIndex()[2];
      }
      ++mit;
    }
    typename OutputImageType::SizeType size;
    size[0] = maxIndex[0] - minIndex[0];
    size[1] = maxIndex[1] - minIndex[1];
    size[2] = maxIndex[2] - minIndex[2];

    typename OutputImageType::RegionType region (minIndex, size);
    typename OutputImageType::Pointer outputImage =
            static_cast< OutputImageType * >(this->ProcessObject::GetOutput(0));
    outputImage->SetRequestedRegion(region);
  }

  m_CurrentVoxelCount = 0;
}

template< class TPixelType >
void
NonLocalMeansDenoisingFilter< TPixelType >
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, ThreadIdType )
{


  // initialize iterators
  typename OutputImageType::Pointer outputImage =
          static_cast< OutputImageType * >(this->ProcessObject::GetOutput(0));

  ImageRegionIterator< OutputImageType > oit(outputImage, outputRegionForThread);
  oit.GoToBegin();

//  typename MaskImageType::Pointer maskImage =
//          static_cast< MaskImageType* >( this->ProcessObject::GetInput(1) );

  ImageRegionIterator< MaskImageType > mit(m_Mask, outputRegionForThread);
  mit.GoToBegin();



  typedef ImageRegionIteratorWithIndex <InputImageType> InputIteratorType;
  typename InputImageType::Pointer inputImagePointer = NULL;
  inputImagePointer = static_cast< InputImageType * >( this->ProcessObject::GetInput(0) );

  InputIteratorType git(inputImagePointer, outputRegionForThread );
  git.GoToBegin();

  // iterate over complete image region
  while( !git.IsAtEnd() )
  {
    typename OutputImageType::PixelType outpix;
    outpix.SetSize (inputImagePointer->GetVectorLength());

    if (mit.Get() != 0)
    {
      if(!m_UseJointInformation)
      {
        for (int i = 0; i < (int)inputImagePointer->GetVectorLength(); ++i)
        {
          double summw = 0;
          double sumj = 0;
          double w = 0;
          std::vector<double> wj;
          std::vector<double> p;
          typename InputIteratorType::IndexType index;
          index = git.GetIndex();

          for (int x = index.GetElement(0) - m_SearchRadius; x <= index.GetElement(0) + m_SearchRadius; ++x)
          {
            for (int y = index.GetElement(1) - m_SearchRadius; y <= index.GetElement(1) + m_SearchRadius; ++y)
            {
              for (int z = index.GetElement(2) - m_SearchRadius; z <= index.GetElement(2) + m_SearchRadius; ++z)
              {
                typename InputIteratorType::IndexType indexV;
                indexV.SetElement(0, x);
                indexV.SetElement(1, y);
                indexV.SetElement(2, z);
                if (inputImagePointer->GetLargestPossibleRegion().IsInside(indexV))
                {
                  TPixelType pixelJ = inputImagePointer->GetPixel(indexV)[i];
                  double sumk = 0;
                  double size = 0;
                  for (int xi = index.GetElement(0) - m_ComparisonRadius, xj = x - m_ComparisonRadius; xi <= index.GetElement(0) + m_ComparisonRadius; ++xi, ++xj)
                  {
                    for (int yi = index.GetElement(1) - m_ComparisonRadius, yj = y - m_ComparisonRadius; yi <= index.GetElement(1) + m_ComparisonRadius; ++yi, ++yj)
                    {
                      for (int zi = index.GetElement(2) - m_ComparisonRadius, zj = z - m_ComparisonRadius; zi <= index.GetElement(2) + m_ComparisonRadius; ++zi, ++zj)
                      {
                        typename InputIteratorType::IndexType indexI, indexJ;
                        indexI.SetElement(0, xi);
                        indexI.SetElement(1, yi);
                        indexI.SetElement(2, zi);
                        indexJ.SetElement(0, xj);
                        indexJ.SetElement(1, yj);
                        indexJ.SetElement(2, zj);


                        // Compare neighborhoods ni & nj
                        if (inputImagePointer->GetLargestPossibleRegion().IsInside(indexI) && inputImagePointer->GetLargestPossibleRegion().IsInside(indexJ))
                        {
                          int diff = inputImagePointer->GetPixel(indexI)[i] - inputImagePointer->GetPixel(indexJ)[i];
                          sumk += (double)(diff*diff);
                          ++size;
                        }
                      }
                    }
                  }
                  // weight all neighborhoods
                  w = std::exp( - sumk / size / m_Variance);
                  wj.push_back(w);
                  if (m_UseRicianAdaption)
                  {
                    p.push_back((double)(pixelJ*pixelJ));
                  }
                  else
                  {
                    p.push_back((double)(pixelJ));
                  }
                  summw += w;
                }
              }
            }
          }
          for (unsigned int n = 0; n < wj.size(); ++n)
          {
            sumj += (wj[n]/summw) * p[n];
          }
          if (m_UseRicianAdaption)
          {
            sumj -=2 * m_Variance;
          }

          if (sumj < 0)
          {
            sumj = 0;
          }

          TPixelType outval;
          if (m_UseRicianAdaption)
          {
            outval = std::floor(std::sqrt(sumj) + 0.5);
          }
          else
          {
            outval = std::floor(sumj + 0.5);
          }
          outpix.SetElement(i, outval);
        }
      }

      else
      {
        double Z = 0;
        itk::VariableLengthVector<double> sumj;
        sumj.SetSize(inputImagePointer->GetVectorLength());
        sumj.Fill(0.0);
        double w = 0;
        std::vector<double> wj;
        std::vector<itk::VariableLengthVector <double> > p;
        typename InputIteratorType::IndexType index;
        index = git.GetIndex();

        for (int x = index.GetElement(0) - m_SearchRadius; x <= index.GetElement(0) + m_SearchRadius; ++x)
        {
          for (int y = index.GetElement(1) - m_SearchRadius; y <= index.GetElement(1) + m_SearchRadius; ++y)
          {
            for (int z = index.GetElement(2) - m_SearchRadius; z <= index.GetElement(2) + m_SearchRadius; ++z)
            {
              typename InputIteratorType::IndexType indexV;
              indexV.SetElement(0, x);
              indexV.SetElement(1, y);
              indexV.SetElement(2, z);
              if (inputImagePointer->GetLargestPossibleRegion().IsInside(indexV))
              {
                typename InputImageType::PixelType pixelJ = inputImagePointer->GetPixel(indexV);
                double sumk = 0;
                double size = 0;
                for (int xi = index.GetElement(0) - m_ComparisonRadius, xj = x - m_ComparisonRadius; xi <= index.GetElement(0) + m_ComparisonRadius; ++xi, ++xj)
                {
                  for (int yi = index.GetElement(1) - m_ComparisonRadius, yj = y - m_ComparisonRadius; yi <= index.GetElement(1) + m_ComparisonRadius; ++yi, ++yj)
                  {
                    for (int zi = index.GetElement(2) - m_ComparisonRadius, zj = z - m_ComparisonRadius; zi <= index.GetElement(2) + m_ComparisonRadius; ++zi, ++zj)
                    {
                      typename InputIteratorType::IndexType indexI, indexJ;
                      indexI.SetElement(0, xi);
                      indexI.SetElement(1, yi);
                      indexI.SetElement(2, zi);
                      indexJ.SetElement(0, xj);
                      indexJ.SetElement(1, yj);
                      indexJ.SetElement(2, zj);
                      // Compare neighborhoods ni & nj
                      if (inputImagePointer->GetLargestPossibleRegion().IsInside(indexI) && inputImagePointer->GetLargestPossibleRegion().IsInside(indexJ))
                      {
                        typename InputImageType::PixelType diff = inputImagePointer->GetPixel(indexI) - inputImagePointer->GetPixel(indexJ);
                        sumk += (double)(diff.GetSquaredNorm());
                        ++size;
                      }
                    }
                  }
                }
                // weight all neighborhoods
                size *= inputImagePointer->GetVectorLength() + 1;
                w = std::exp( - (sumk / size) / m_Variance);
                wj.push_back(w);
                if (m_UseRicianAdaption)
                {
                  itk::VariableLengthVector <double> m;
                  m.SetSize(inputImagePointer->GetVectorLength());
                  for (unsigned int i = 0; i < inputImagePointer->GetVectorLength(); ++i)
                  {
                    double sp = (double)(pixelJ.GetElement(i) * pixelJ.GetElement(i));
                    m.SetElement(i,sp);
                  }
                  p.push_back(m);
                }
                else
                  ++size;
                {
                  p.push_back(pixelJ);
                }
                Z += w;
              }
            }
          }
        }
        for (unsigned int n = 0; n < wj.size(); ++n)
        {
          sumj = sumj + ((wj[n]/Z) * p[n]);
        }
        if (m_UseRicianAdaption)
        {
          sumj = sumj - (2 * m_Variance);
        }

        for (unsigned int i = 0; i < inputImagePointer->GetVectorLength(); ++i)
        {
          double a = sumj.GetElement(i);
          if (a < 0)
          {
            a = 0;
          }
          TPixelType outval;
          if (m_UseRicianAdaption)
          {
            outval = std::floor(std::sqrt(a) + 0.5);
          }
          else
          {
            outval = std::floor(a + 0.5);
          }
          outpix.SetElement(i, outval);
        }
      }
    }
    else
    {
      outpix.Fill(0);
    }

    oit.Set(outpix);
    ++oit;
    ++m_CurrentVoxelCount;
    ++git;
    ++mit;
  }

  MITK_INFO << "One Thread finished calculation";
}

template< class TPixelType >
void NonLocalMeansDenoisingFilter< TPixelType >::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template< class TPixelType >
void NonLocalMeansDenoisingFilter< TPixelType >::SetInputImage(const InputImageType* image)
{
  this->SetNthInput(0, const_cast< InputImageType* >(image));
}

template< class TPixelType >
void NonLocalMeansDenoisingFilter< TPixelType >::SetInputMask(MaskImageType* mask)
{
  //this->SetNthInput(1, const_cast< MaskImageType* >(mask));
  m_Mask = mask;
}
}

#endif // __itkNonLocalMeansDenoisingFilter_txx
