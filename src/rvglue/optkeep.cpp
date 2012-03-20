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
 * $Id: optkeep.cpp,v 1.1 2000/05/03 08:47:49 ali Exp ali $
 *
 * $Log: optkeep.cpp,v $
 * Revision 1.1  2000/05/03 08:47:49  ali
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

#include "optkeep.h"
#include "errdesc.h"


OptionSetKeeper define_group;


OptionSetKeeper::OptionSetKeeper(OptionSetKeeper *_parent)
{
  parent=_parent;
  cursize=0;
}

OptionSetKeeper::~OptionSetKeeper()
{
  int i;
  
  for(i=0;i<cursize;i++)
    {
      free(inames[i]);
      delete optsets[i];
    };
}



ModList *OptionSetKeeper::lookup(const char *iname, bool useDefaults)
{
  int i;
  ModList *res;
  dprintf("k","LOOKUP(%s)\n",iname);
  for(i=0;i<cursize;i++)
    if(strcasecmp(iname,inames[i])==0)
      {
	dprintf("k","  -> in local list.\n");
	return optsets[i];
      };

  if(!useDefaults)
    return NULL;

  if(parent != NULL)
    {
      dprintf("k","  ask parent\n");
      res=parent->lookup(iname,false);
      if(res != NULL)
	{
	  dprintf("k","  in parent list\n");
	  return res;
	};
    };

  dprintf("k","  find local default\n");
  res=lookup(DEFAULT_GROUP,false);
  if(res != NULL)
    {
      dprintf("k","  -> local default\n");
      return res;
    };

  if(parent != NULL)
    {
      dprintf("k","  find parent default\n");
      res=parent->lookup(DEFAULT_GROUP,false);
      if(res!=NULL)
	{
	  dprintf("k","  -> parent default\n");
	  return res;
	};
    };

  dprintf("k","  -> nothing found\n");
  return &empty_modlist;
}


int OptionSetKeeper::readpart(Parser &p)
{
  char *iname;

  iname=strdup(p.tok_text);
  lprintf(3,"parse-options `%s'\n",iname);
  p.gentoken();

  if(p.curtok != PT_BBEGIN)
    p.error(ERROR_EMPTY_FILEOPT,"ignoring empty option set for instance `%s'", iname);
  else if(lookup(iname, false) != NULL)
    {
      ModList oset;
      oset.parse(p);
      p.error(ERROR_DUPLICATE_FILEOPT,"ignoring duplicate option set for instance `%s'", iname);
    }
  else
    {
      if(cursize>=MAX_FOPTS-1)
	{
	  p.error(ERROR_MAXFOPTS_EXCEEDED,"internal limit MAX_FOPTS exceeded");
	  free(iname);
	  return 100;
	}
      else
	{
	  inames[cursize]=iname;
	  optsets[cursize]=new ModList;
	  cursize++;
	  return !optsets[cursize-1]->parse(p); /* to skip freeing */
	};
    };

  free(iname);
  return 0;
}

bool OptionSetKeeper::readSpecial(Parser &p)
{
  return false;
}

int OptionSetKeeper::read(Parser &p)
{
  int res;

  if(p.curtok == PT_BBEGIN)
    {
      p.gentoken();

      do
	{
	  if(p.curtok==PT_TEXT)
	    {
	      if(!readSpecial(p))
		{
		  res=readpart(p);
		  if(res != 0)
		    return res;
		};
	    }
	  else if(p.curtok == PT_EOF)
	    {
	      p.error(ERROR_EOF_IN_BLOCK, "premature end of file");
	      return 100;
	    }
	  else if(p.curtok != PT_BEND)
	    {
	      p.error(ERROR_EXPECTED_FNAME,"expected file name");
	      return 100;
	    };

	}
      while ( (p.curtok != PT_EOF) && (p.curtok != PT_BEND));

      p.gentoken();
    };

  if(!postParsing())
    return 100;

  return 0;
}

bool OptionSetKeeper::postParsing(void)
{
  return true;
}


void OptionSetKeeper::dump(const char *pref)
{
  int i;

  for(i=0;i<cursize;i++)
    {
      printf("%s'%s':\n",pref,inames[i]);
      optsets[i]->dump(pref);
    };
}
