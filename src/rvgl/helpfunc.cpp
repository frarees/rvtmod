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
 * $Id: helpfunc.cpp,v 1.1 2000/05/03 08:50:04 ali Exp ali $
 *
 * $Log: helpfunc.cpp,v $
 * Revision 1.1  2000/05/03 08:50:04  ali
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

#include "print.h"
#include "helpfunc.h"


char *breakfname(char *fname)
{
  char *dot;

  dot=strrchr(fname,'.');
  if(dot == NULL)
    {
      dot=&fname[strlen(fname)];
    }
  else
    {
      *dot=0;
      dot++;
    };
  return dot;
}



bool filehastype(const char *fname, const char *suff)
{
  bool res;
  char *dups;
  
  dups=strdup(fname);
  MEM(dups);
  
  res= strcasecmp(breakfname(dups),suff)==0;

  free(dups);
  return res;
}



void replaceExt(char *fname, const char *suff) // vag 2001-05-08
{
  char *dot=breakfname(fname);
  *dot=0;
  *(--dot)='.';
  strcat(fname,suff);
}



void replaceFname(char *fname, const char *newname) // vag 2001-05-08
{
  char *slash;

  slash=strrchr(fname,'/');
  if(slash == NULL)
    {
      slash=strrchr(fname,'\\');
    }
  if(slash == NULL)
    {
      slash=strrchr(fname,':');
    }
  if(slash == NULL)
    {
      fname[0]=0;
    }
  else
    {
      slash++;
      *slash=0;
    }
  strcat(fname,newname);
}

static char *next=NULL;

char *strqtok(char *str, const char *delimiters, const char *quotes) // vag 2001-05-08
{
  char *s;
  char *c;
  if(str)
    next=str;
  if(!next)  // vag 2002-07-08 (avoid the crash)
    return NULL;
  s=next + strspn(next, delimiters); // skip leading delimiters
  if(!*s)
  {
    // No more tokens (previous ended with trailing delimiters).
    next = NULL;
    return NULL;
  }
  c=strpbrk(s, quotes); // first quote (if any).
  if(c==s)
  {
    // Token begins with quote. Get closing quote.
    char quote=*c;
    s++;
    c=strchr(s, quote);
    if(c)
    {
      // Closing quote - terminate token.
      next=c+1;
      *c=0;
      return s;
    }
    // Last token; quote is unclosed. Ignore (return rest of line).
    next=NULL;
    return s;
  }
  else
  {
    // No quote - look for delimiters (like strtok).
    const char *del;
    c=s;
    while(*(++c))
    {
      for(del=delimiters; *del; del++)
      {
        if(*c == *del)
        {
          *c=0; // Closing delimiter - terminate token.
          next=c+1;
          return s;
        }
      }
    }
    next=NULL; // Last token.
    return s;
  }
  /*NOTREACHED*/
  return NULL;
}
