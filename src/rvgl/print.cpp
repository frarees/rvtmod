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
 * $Id: print.cpp,v 1.2 2000/05/03 09:15:25 ali Exp ali $
 *
 * $Log: print.cpp,v $
 * Revision 1.2  2000/05/03 09:15:25  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:50:05  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "print.h"


int msglevel;
char debugsyms[30] = "";

time_t last_percent_tt=(time_t)0;


int debugstrshare(const char *s1, const char *s2)
{
  while(*s1 != 0)
    {
      if(*s1 == '+')
	return 1;

      if(strchr(s2, *s1) != NULL)
	return 1;
      else
	s1++;
    };
  return 0;
}

void dprintf(const char *sym, const char *args, ...)
{
  va_list ap;

  va_start(ap, args);
  if(debugstrshare(debugsyms,sym))
    {
      printf("\033[40m[\033[33m%s\033[0m\033[40m]\033[0m ",sym);
      vprintf(args, ap);
    };
  va_end(ap);
}



void lprintf(int level, const char *args, ...)
{
  va_list ap;

  va_start(ap, args);  
  if(msglevel>=level)
    vprintf(args, ap);
  va_end(ap);
}

void lprintff(int level, const char *args, ...)
{
  va_list ap;

  va_start(ap, args);  
  if(msglevel>=level)
    {
      vprintf(args, ap);
      fflush(stdout);
    };
  va_end(ap);
}

void interest(const char *args, ...)
{
  va_list ap;

  printf("\nInteresting: ");
  va_start(ap, args);
  vprintf(args, ap);
  va_end(ap);
  printf("\n");
}

void print_panic(bool show_blabla, const char *args, va_list &ap)
{

  printf("\n\n\nPANIC: ");
  vprintf(args, ap);
  printf("\n");

  if(show_blabla)
    {
      printf("  You encountered a program bug.\n");
      printf("  Look whether there is a newer version of this program available,\n");
      printf("  where this bug is fixed.\n\n");
      printf("  If not, try to send an eMail to\n");
      printf("    alirv@perror.de\n");
      printf("  and include:\n");
      printf("   1. all input files you used for this track or instance\n");
      printf("   2. the command file you used\n");
      printf("   3. the message that stands above behind 'PANIC'\n");
      printf("   4. the program version ('%s')\n\n",APPVERSION);
      printf("  The files this program created are most likely waste.\n");
    };
  exit(127);
}

void sxpanic(const char *args, ...)
{
  va_list ap;

  va_start(ap, args);  
  print_panic(false, args, ap);
  va_end(ap);
}

void xpanic(const char *args, ...)
{
  va_list ap;

  va_start(ap, args);  
  print_panic(true, args, ap);
  va_end(ap);
}


void showPercent(int level, int now, int total, int &last)
{
  int act=(now*100)/total;
  time_t curt=time(NULL);

  if((act > last) && (curt != last_percent_tt))
    {
      lprintff(level, "\r  %3d %%   ",act);
      last_percent_tt=curt;
      last=act;
    };
}

void doneShowPercent(int level)
{
  lprintff(level, "\r           \r");
}


void __pointer_test(void *ptr, const char *ptrname, const char *sfile, int sline)
{
//  fprintf(stderr, "%s (%s %d): %p\n",ptrname,sfile,sline,ptr);

  if(ptr==NULL)
    {
      printf("Not enough memory. Exiting...\n");
      dprintf("m","Pointer '%s' in %s, line %d, is NULL\n",ptrname,sfile,sline);
      exit(127);
    };
}
