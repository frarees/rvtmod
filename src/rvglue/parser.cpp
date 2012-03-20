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
 * $Id: parser.cpp,v 1.2 2000/05/03 09:49:12 ali Exp ali $
 *
 * $Log: parser.cpp,v $
 * Revision 1.2  2000/05/03 09:49:12  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:47:42  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/print.h"

#include "parser.h"
#include "errdesc.h"
#include "appopts.h"

void Parser::gentoken(void)
{
  int pos;

  skipwhite();
  if( type(gchar()) == PT_TEXT )
    {
      pos=0;
      do {
	if(pos<MAX_TOK_LENGTH-1)
	  {
	    tok_text[pos]=curch;
	    pos++;
	  };
	gchar();
      } while(type(curch)==PT_TEXT);
      push();
      tok_text[pos]=0;
      curtok=PT_TEXT;
    }
  else if( type(curch) == PT_CBEGIN )
    {
      int level=1;
      int start=lineno;
      while((level > 0) && (type(curch) != PT_EOF))
	{
	  switch(type(gchar()))
	    {
	    case PT_CBEGIN:
	      level++; break;
	    case PT_CEND:
	      level--; break;
	    };
	};
      if(type(curch)==PT_EOF)
	error(ERROR_COMMENT_EOF, "Comment at line %d never closed", start);
      gentoken();
    }
  else if( type(curch) == PT_CEND )
    {
      error(ERROR_COMMENT_NOBEGIN, "Unmatched ']'");
      gentoken();
    }
  else if( type(curch) == PT_LINECOM )
    {
      do
	gchar();
      while( (type(curch)!=PT_EOF) && (curch!='\n') );
      gentoken();
    }
  else
    {
      curtok=type(curch);
    };
}

void Parser::skipwhite(void)
{
  while(type(gchar()) == PT_WHITE)
    ;

  if(type(curch) != PT_EOF)
    push();
}

int Parser::gchar(void)
{
  curch=fgetc(f);
  if(curch=='\n')
    lineno++;
  return curch;
}

void Parser::push(void)
{
  if(curch == '\n')
    lineno--;
  ungetc(curch, f);
}



int Parser::type(int ch)
{
  switch(ch)
    {
    case EOF:
      return PT_EOF;
    case ' ':
    case '\t':
    case '\n':
    case '\r':
      return PT_WHITE;
    case '(':
      return PT_BBEGIN;
    case ')':
      return PT_BEND;
    case '[':
      return PT_CBEGIN;
    case ']':
      return PT_CEND;
    case '#':
      return PT_LINECOM;
    default:
      return PT_TEXT;
    };
}



Parser::Parser(const char *_fname)
{
  fname=strdup(_fname);
  f=fopen(_fname,"r");
  lineno=1;
  if(f != NULL)
    gentoken();
  else
    {
      perror(0,ERROR_FILE_ROPEN,"Cannot open input file '%s': %s",_fname,
	     strerror(errno));
      curtok=PT_EOF;
    };
}

Parser::~Parser(void)
{
  if(f!=NULL)
    fclose(f);

  if(fname!=NULL)
    free(fname);
}

void Parser::vprint(const char *args, va_list ap)
{
  char buffer[200];
  char *cur, *nxt;
  //  va_list ap;

  //  va_start(ap, args);  
  vsprintf(buffer,args, ap);
  //  va_end(ap);

  cur=buffer;

  do
    {
      nxt=strstr(cur,"$$");
      if(nxt == NULL)
	{
	  printf("%s",cur);
	  cur=NULL;
	}
      else
	{
	  if(nxt != cur)
	    {
	      *nxt = 0;
	      printf("%s",cur);
	      cur=nxt;
	    };
	  printcurtok();
	  cur+=2;
	};
    }
  while(cur != NULL);
}


void Parser::error(int errnum, const char *args, ...)
{
  va_list ap;

  if(ERROR_IS_WARNING(errnum) && (msglevel<1))
    return;

  if(fname!=NULL)
    printf("%s, ",fname);
  printf("line %d: ",lineno);
  if(errnum>0)
    {
      if(ERROR_IS_WARNING(errnum))
	printf("Warning: ");
      else
	printf("Error: ");
    };
  va_start(ap, args);  
  vprint(args,ap);
  va_end(ap);
  printf("\n");

  if(errnum > 0)
    describe_error(errnum);
}

void Parser::print(const char *args, ...)
{
  va_list ap;

  va_start(ap, args);  
  vprint(args,ap);
  va_end(ap);
}


bool Parser::eatOpen(bool force)
{
  if(curtok == PT_BBEGIN)
    {
      gentoken();
      return true;
    }
  else if(force)
    {
      printf("nanu?\n");
      error(ERROR_BBEGIN_EXPECTED, "Expected opening bracket, not $$");
      return false;
    }
  else
    return false;
}


bool Parser::eatClose(bool force)
{
  if(curtok == PT_BEND)
    {
      gentoken();
      return true;
    }
  else if(force)
    {
      error(ERROR_BEND_EXPECTED, "Expected closing bracket, not $$");
      return false;
    }
  else
    return false;
}

void Parser::printcurtok(void)
{
  switch(curtok)
    {
    case PT_EOF:
      printf("End-of-file");
      break;
    case PT_BBEGIN:
      printf("'('");
      break;
    case PT_BEND:
      printf("')'");
      break;
    case PT_TEXT:
      printf("'%s'",tok_text);
      break;
    };
}


int Parser::eatNumber(void)
{
  char *cur;
  int res;

  if(curtok != PT_TEXT)
    return -1;

  cur=tok_text;

  if(*cur == 0)
    return -1;

  while(*cur != 0)
    if((*cur < '0')||(*cur > '9'))
      return -1;
    else
      cur++;

  sscanf(tok_text,"%d",&res);
  gentoken();
  return res;
}

bool Parser::eatSNumber(int &res)
{
  char *cur;

  if(curtok != PT_TEXT)
    return true;

  cur=tok_text;
  if((*cur == '+') || (*cur == '-'))
    cur++;

  if(*cur==0)
    return false;

  while(*cur != 0)
    if((*cur < '0')||(*cur > '9'))
      return false;
    else
      cur++;

  sscanf(tok_text,"%d",&res);
  gentoken();
  return true;
}
