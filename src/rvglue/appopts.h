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
 * $Id: appopts.h,v 1.1 2000/05/03 08:48:41 ali Exp ali $
 *
 * $Log: appopts.h,v $
 * Revision 1.1  2000/05/03 08:48:41  ali
 * Initial revision
 *
 *
 */
#ifndef __APPOPTS_H
#define __APPOPTS_H

#include "rvgl/print.h"


//! Holds app-wide info 
/*! appopt in appopts.h is only instance of this class. */
class AppOptions
{
public:
  /*! AppOptions constructor desctip?
   * and this?
   */
  AppOptions();

  /*! dtor */
  ~AppOptions();

  /*! parses command line args.
   * \return is \c false, if there was an error
   * \sa msglevel
   */
  bool parseArgs(int argc, char **argv);

  /*! If \c false, the explanations for errors and warning won't be written */
  bool errhelp;

  /*! Name of command file to process */
  char *inputfname;

  /*! Name of .w or .prm output file */
  char *woutname;

  /*! Name of .ncp output file */
  char *ncpoutname;

  /*! Name of .i-w or .i-p output file */
  char *ioutname;

  /*! Base of output name */
  char *baseoutname;

  /*! If \c true, an instance is to be created */
  bool create_instance;

  /*! Sets the output file names (#woutname, #ncpoutname, #ioutname) according
    to the given basename, including auto-detection of file type */
  bool setOutName(const char *basename);

private:
  /*! Parses the \c -o= command line option */
  bool trySetOutName(char *);

  /*! Prints the "usage" info message */
  void usage(void);
};


/*! global instance of AppOptions class */
extern AppOptions appopt;


#endif
