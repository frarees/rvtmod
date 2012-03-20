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
 * $Id: optkeep.h,v 1.1 2000/05/03 08:48:16 ali Exp ali $
 *
 * $Log: optkeep.h,v $
 * Revision 1.1  2000/05/03 08:48:16  ali
 * Initial revision
 *
 *
 */
#ifndef __OPTKEEP_H
#define __OPTKEEP_H

#include "modlist.h"
#include "parser.h"


/*! maximum number of stored groups */
#define MAX_FOPTS  200

/*! name of the default group */
#define DEFAULT_GROUP "default"


//! Keeper for option sequence and input splitting
class OptionSetKeeper
{
public:
  /*! constructor
   *
   * if \a _parent is \c !=NULL, it will be used for "default" and "global" lookups
   */
  OptionSetKeeper(OptionSetKeeper *_parent=NULL);
  virtual ~OptionSetKeeper();

  /*!
   * abstract.
   *
   * checks whether the current token is some special key. If so, it parses
   * the key, eats it and returns true.
   *
   * Otherwise it return false.
   *
   * This abstract implementation does nothing at all.
   */
  virtual bool readSpecial(Parser &p) hook;

  /*!
   * abstract.
   *
   * post-parsing checks. If return value is \c false, the program will safely exit.
   *
   * This abstract implementation does nothing at all.
   */
  virtual bool postParsing(void) hook;

  int read(Parser &p);


  /*! searches for matching ModList with name \a iname
   *
   * if \a useDefaults is \c true, "default" and "global" lookups are done
   *
   * \return
   * never \c NULL. If no matching group is found, a reference to
   * #empty_modlist is returned
   */
  ModList *lookup(const char *iname, bool useDefaults=true);

  void dump(const char *pref);

private:
  OptionSetKeeper *parent;
  int readpart(Parser &p);

  char *inames[MAX_FOPTS];
  ModList *optsets[MAX_FOPTS];
  int cursize;
};



/*! global instance that holds the "define" section */
extern OptionSetKeeper define_group;


#endif
