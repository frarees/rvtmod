/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *  Copyright (C) 2001 Gabor Varga (vag) (bootes@freemail.hu)
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
 * $Id: texdef.h,v 1.1 2000/05/03 08:48:42 ali Exp ali $
 *
 * $Log: texdef.h,v $
 * Revision 1.1  2000/05/03 08:48:42  ali
 * Initial revision
 *
 *
 */
#ifndef __TEXTURE_DEFINITION_H
#define __TEXTURE_DEFINITION_H

#include "rvgl/rvtypes.h"
#include "parser.h"


#define TEX_UNKNOWN  -10


//! Parsable texture definition
/*!
 * This class provides a common parser for the texture definition syntax in
 * command files together with some functions on them.
 */
class TextureDefinition
{
public:
  /*! ctor */
  TextureDefinition();
  TextureDefinition(rvshort _tex, rvfloat _x0,rvfloat _x1,rvfloat _y0,rvfloat _y1);


  /*!
   * parses the string \a str for a texture definition (style a:1,2:3x4). Error
   * messages are printed through the given parser.
   *
   * If \a allow_asterisk is \c false, wildcards are not allowed for the texture sheet.
   *
   * \a def is a texture definition from which values are copied if parts contain
   * wildcards. It must point to a valid TextureDefinition instance. #default_src_texdef
   * is there if no special values are needed.
   *
   * \a str is modified while parsing it. (Split up into substrings)
   */
  bool parseTexSpec(const char *str, TextureDefinition *def, Parser &, bool allow_asterisk);

  /*!
   * Returns \c true, if \a uv is inside the given definition and \a sheet matches.
   *
   * Does work with TEX_UNKNOWN for both texture definition and argument
   */
  bool uv_in_def(rvshort sheet, RV_UV *uv);

  /*!
   * Rotate right 90 degrees.
   */
  void rotateRight();   // vag 2001-05-08

  /*! dumps the texture definition in a human-readible format. */
  void printtex(FILE *);

  //protected:
  rvfloat x0,x1,y0,y1;

  /*! May be TEX_UNKNOWN */
  rvshort tex;


private:
  bool parseTexLetter(char *buf, Parser &, bool allow_asterisk);
  bool parseSingleNum(char *buf, int *num, Parser &);
  bool parseNumPair(char *obuf, char sepch, int *i1, int *i2, const char *pairname, Parser &);
};


/*!
 * A simple texture definition ( *:0,0:256x256 ) that can be used as default
 * definition for parse()
 */
extern TextureDefinition default_src_texdef;



#endif
