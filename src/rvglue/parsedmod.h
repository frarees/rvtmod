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
 * $Id: parsedmod.h,v 1.1 2000/05/03 08:48:42 ali Exp ali $
 *
 * $Log: parsedmod.h,v $
 * Revision 1.1  2000/05/03 08:48:42  ali
 * Initial revision
 *
 *
 */
#ifndef __PARSED_MODIFIER_H
#define __PARSED_MODIFIER_H

#include "rvgl/polymod.h"
#include "parser.h"


//! abstract superclass
/*! extends PolygonModifier with prototypes for parsing
 */
class ParsedMod : public PolygonModifier
{
public:

  /*!
   * abstract prototype
   *
   * \return \c true, if parsing or constructing failed.
   */
  virtual bool bad(void) hook;

  /*!
   * abstract prototype
   *
   * create the modifier's data
   *
   * \return \c true, if parsing succeeded. The prototype always returns \c true.
   */
  virtual bool parse(Parser &) hook;

  /*!
   * abstract prototype
   *
   * debugging function: dump info to stdout with preceeding \a pref
   */
  virtual void dump(const char *pref) hook;
};



#endif
