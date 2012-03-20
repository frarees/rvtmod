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
 * $Id: input.cpp,v 1.1 2000/05/03 08:49:36 ali Exp ali $
 *
 * $Log: input.cpp,v $
 * Revision 1.1  2000/05/03 08:49:36  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/helpfunc.h"

#include "input.h"
#include "parser.h"
#include "errdesc.h"
#include "finf.h"
#include "wf.h"
#include "wmodlist.h"
#include "optkeep.h"
#include "appopts.h"
#include "keys.h"


/* Global variables -- arrggghhhh :-) */
OptionSetKeeper *globalsect = NULL;




void skipbstuff(Parser &p)
{
  int level=0;

  if(p.curtok == PT_BBEGIN)
    {
      do
	{
	  if(p.curtok==PT_BBEGIN)
	    level++;
	  else if(p.curtok == PT_BEND)
	    level--;
	  p.gentoken();
	}
      while ( (p.curtok != PT_EOF) && (level > 0) );
    };
}


int processCreateSect(Parser &p)
{
  bool havebracks;

  p.gentoken();
  havebracks=p.eatOpen(false);

  if(p.curtok == PT_TEXT)
    {
      if(!appopt.setOutName(p.tok_text))
	return 100;
      p.gentoken();
    }
  else
    {
      p.error(ERROR_EXPECTED_FNAME,"Filename in '%s' section expected, not $$",KEY_CREATE_SECTION);
      return 100;
    };

  if(havebracks)
    if(!p.eatClose(true))
      return 100;

  return 0;
}

int processGlobalSect(Parser &p)
{
  p.gentoken();

  if(globalsect == NULL)
    {
      globalsect=new OptionSetKeeper; MEM(globalsect);
      return globalsect->read(p);
    }
  else
    {
      OptionSetKeeper keep;
      keep.read(p);
      p.error(ERROR_DUPLICATE_GLOBAL, "ignoring duplicate global section");
    };
  return 0;
}

int processDefineSect(Parser &p)
{
  p.gentoken();

  return define_group.read(p);
}

int processFileSect(Parser &p, World *world)
{
  char *fname;
  int res;

  fname=strdup(p.tok_text);
  p.gentoken();

  if(filehastype(fname,"w"))
    {
      W_ModList mlist;
      if(!mlist.parse(p))
	return 1;
      //      mlist.dump("");
      res=process_w_file(fname, mlist, world);
    }
  else if(filehastype(fname,"fin"))
    {
      OptionSetKeeper keep(globalsect);
      keep.read(p);
      res=process_fin_file(fname, keep, world);
    }
  else
    {
      p.error(ERROR_ITOP_FILETYPE, "File `%s' has wrong type",fname);
      res=100;
    };
  free(fname);
  return res;
}



int processInput(const char *filename, World *world)
{
  Parser p(filename);
  int res;

  while(p.curtok != PT_EOF)
    {

      if(p.curtok == PT_TEXT)
	{
	  if(strcasecmp(p.tok_text, KEY_GLOBAL_SECTION) == 0)
	    {
	      res=processGlobalSect(p);
	      if(res != 0)
		return res;
	    }
	  else if(strcasecmp(p.tok_text, KEY_CREATE_SECTION) == 0)
	    {
	      res=processCreateSect(p);
	      if(res != 0)
		return res;
	    }
	  else if(strcasecmp(p.tok_text, KEY_DEFINE_SECTION) == 0)
	    {
	      res=processDefineSect(p);
	      if(res != 0)
		return res;
	    }
	  else if(strchr(p.tok_text,'.') != NULL)
	    {
	      res=processFileSect(p, world);
	      if(res != 0)
		return res;
	    }
	  else
	    {
	      p.error(ERROR_ITOP_SYNTAX, "filename or keyword expected, not $$");
	      return 100;
	    }
	}
      else
	{
	  p.error(ERROR_ITOP_SYNTAX, "filename or keyword expected, not $$");
	  return 100;
	};
  };
  return 0;
}
