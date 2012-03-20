/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/*
 * $Id: rvcolor.h,v 1.1 2000/05/03 08:49:54 ali Exp ali $
 *
 * $Log: rvcolor.h,v $
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 *
 *
 */
#ifndef __COLOR_H
#define __COLOR_H

#include "rvbasetypes.h"

//! re-volt basic type for colors
/*!
 * frontend to the 32-bit field used for color definitions.
 */
class RVColor
{
public:

  /*! sets Red channel value to \a val */
  inline void setRed(rvulong val) { setChannel(1, val); }
  /*! sets Green channel value to \a val */
  inline void setGreen(rvulong val) { setChannel(2, val); }
  /*! sets Blue channel value to \a val */
  inline void setBlue(rvulong val) { setChannel(3, val); }
   /*! sets Alpha channel value to \a val */
  inline void setAlpha(rvulong val) { setChannel(0, val); }

  /*! returns Red channel value */
  inline rvulong red(void) { return channel(1); }
  /*! returns Green channel value */
  inline rvulong green(void) { return channel(2); }
  /*! returns Blue channel value */
  inline rvulong blue(void) { return channel(3); }

  /*! returns Alpha channel value */
  inline rvulong alpha(void) { return channel(0); }


  /*! shades by adding or substracting values */
  void shadeAdd(int r, int g, int b);

  /*! return the value for the channel \a ch, where
   * \arg 0 is Alpha
   * \arg 1 is Red
   * \arg 2 is Green
   * \arg 3 is Blue
   */
  rvulong channel(int ch);

  /*! sets the value for the channel \a ch to \a val, where
   * \arg 0 is Alpha
   * \arg 1 is Red
   * \arg 2 is Green
   * \arg 3 is Blue
   */
  void setChannel(int ch, rvulong val);

  /*! sets the R, G and B channels, leaving the Alpha channel untouched */
  void setRGB(rvulong r, rvulong g, rvulong b);

  /*! sets the R, G and B channels, leaving the Alpha channel untouched */
  inline void setRGBf(rvfloat r, rvfloat g, rvfloat b)
    { setRGB( (rvulong)(r*255.0), (rvulong)(g*255.0), (rvulong)(b*255.0) ); }

  /*! \return the brightness. Always between 0.0 and 1.0 */
  rvfloat brightness(void);

  /*! brightens or darkens the color by \e multiplying with the
   * given factors */
  void shade(float sr, float sg, float sb);

  inline void shade(float s) { shade(s,s,s); };


  /*! value as found in re-volt files */
  rvulong value;
};

#define COLOR_STORAGE_SIZE_OKAY (sizeof(RVColor) == sizeof(rvlong))


#endif
