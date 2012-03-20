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
 * $Id: simplemods.h,v 1.1 2000/05/03 08:48:42 ali Exp ali $
 *
 * $Log: simplemods.h,v $
 * Revision 1.1  2000/05/03 08:48:42  ali
 * Initial revision
 *
 *
 */
#ifndef __SIMPLE_MODIFIER_H
#define __SIMPLE_MODIFIER_H

#include "parsedmod.h"
#include "parsedcolor.h"


//! implementation of \c surface option
class Mod_Surface : public ParsedMod
{
public:
  Mod_Surface();

  virtual bool apply(Polygon*);
  virtual bool bad(void);
  virtual bool parse(Parser &);

  void dump(const char *pref);

private:
  bool parsed;
  rvlong surface;
};








//! implementation of \c transparency option
class Mod_Transparency : public ParsedMod
{
public:
  Mod_Transparency();

  virtual bool apply(Polygon*);
  virtual bool bad(void);
  virtual bool parse(Parser &);  

  virtual void dump(const char *pref);

private:
  bool parsed;
  int value;
};


//! implementation of \c invisible option
/*!
 * as the option requires no argument, ParsedMod::parse() isn't reimplemented
 */
class Mod_Invisible : public ParsedMod
{
public:
  virtual bool apply(Polygon*);
  virtual void dump(const char *pref);
};


//! implementation of \c nocoll option
/*!
 * as the option requires no argument, ParsedMod::parse() isn't reimplemented
 */
class Mod_NoColl : public ParsedMod
{
public:
  virtual bool apply(Polygon*);
  virtual void dump(const char *pref);
};


//! implementation of \c off option
/*!
 * as the option requires no argument, ParsedMod::parse() isn't reimplemented
 */
class Mod_Off : public ParsedMod
{
public:
  virtual bool apply(Polygon*);
  virtual void dump(const char *pref);
};




//! implementation of \c doublesided option
/*!
 * as the option requires no argument, ParsedMod::parse() isn't reimplemented
 */
class Mod_DoubleSided : public ParsedMod
{
public:
  virtual bool apply(Polygon*);
  virtual void dump(const char *pref);
};




//! implementation of \c lower_coll option
/*!
 * as the option requires no argument, ParsedMod::parse() isn't reimplemented
 */
class Mod_LowerColl : public ParsedMod  // vag 2001-06-28
{
public:
  Mod_LowerColl();

  virtual bool apply(Polygon*);
  virtual void dump(const char *pref);

private:
  bool parsed;
  rvfloat value;
};

#endif

