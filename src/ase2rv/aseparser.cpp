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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

#include "rvgl/print.h"

#include "aseprint.h"
#include "namelist.h"
#include "asematerial.h"
#include "asemesh.h"

#include "asekeys.h"
#include "aseparser.h"

void ASEParser::judgeString(void)
{
  bool hasDot=false;
  bool hasNonDigit=false;
  char *cur;

  if(*tok_text == '*')
    {
      tok_key= key2id(tok_text);
      if(tok_key < 0)
	curtok=PT_STRING;
      else
	curtok=PT_KEYWORD;
      return;
    };

  cur=tok_text;
  while(*cur != 0)
    {
      if(*cur == '.')
	hasDot=true;
      else if((*cur != '-') && ((*cur<'0')||(*cur>'9')))
	hasNonDigit=true;
      cur++;
    };

  if(hasNonDigit)
    curtok=PT_STRING;
  else if(hasDot)
    {
      curtok=PT_FLOAT;
      sscanf(tok_text,"%f",&tok_float);
    }
  else
    {
      curtok=PT_INTEGER;
      sscanf(tok_text,"%d",&tok_int);
    };
}



void ASEParser::gentoken(void)
{
  int pos;

  skipwhite();
  if(gchar() == '"')
    {
      gchar();
      pos=0;
      while((curch!='"')&&(type(curch)!=PT_EOF))
	{
	  if(pos<MAX_TOK_LENGTH-1)
	    {
	      tok_text[pos]=curch;
	      pos++;
	    };
	  gchar();
	};
      tok_text[pos]=0;

      curtok=PT_STRING;
    }
  else if( type(curch) == PT_STRING )
    {
      pos=0;
      do {
	if(pos<MAX_TOK_LENGTH-1)
	  {
	    tok_text[pos]=curch;
	    pos++;
	  };
	gchar();
      } while(type(curch)==PT_STRING);
      push();
      tok_text[pos]=0;
      judgeString();
    }
  else
    {
      curtok=type(curch);
    };
}



void ASEParser::parse
(ASEMeshList *meshes, ASEMaterialList *mats, NameList *names)
{
  lprintf(1,"Parsing ASE File '%s'\n",fname);
  while(curtok != PT_EOF)
  {
    genKeyToken(false);

    if(curtok != PT_KEYWORD)
    {
      error("$$");
      sxpanic("key finding failure");
    }

    switch(tok_key)
    {
      case AK_VERSION:
      {
        gentoken();
        if(curtok != PT_INTEGER)
        sxpanic("ascii export version is no integer");
        if(tok_int != 200)
        interest("ASE Export Version is %d\n",tok_int);
        gentoken();
        break;
      }

      case AK_MATERIAL_LIST:
      {
        gentoken();
        if(mats)
          mats->parse(*this);
        break;
      }

      case AK_GEOMOBJECT:
      {
        gentoken();
        parseGeomObject(meshes, names);
        break;
      }

      default:
      {
        gentoken();
        break;
      }
    }

    genKeyToken(false);
  }
}



void ASEParser::parseGeomObject(ASEMeshList *meshes, NameList *names)
{
  ASEMesh *mesh=NULL;
  bool mesh_seen=false;
  char *name=NULL;
  bool name_seen=false;
  int mat_id=0;
  bool mat_seen=false;
  bool incl_this=false;

  if(curtok != PT_BBEGIN)
    sxpanic("GEOMOBJECT section begins without '{'");
  genKeyToken(true);

  do
  {
    switch(tok_key)
    {
      case AK_NODE_NAME:
      {
        gentoken();
        if(curtok == PT_STRING)
        {
          name_seen=true;
          if(!meshes)
            aprintf("%s\n",tok_text);   // list only
          else
          {
            if(!names->count() || names->find(tok_text,/*touch=*/true))
            {
              name=strdup(tok_text); MEM(name);
              incl_this=true;
              lprintf(1, " (reading object '%s')\n",tok_text);
            }
            else
            {
              incl_this=false;
              lprintf(1, " (skipping object '%s')\n",tok_text);
            }
          }
        }
        else
          sxpanic("%s is no string??", id2keyname(AK_NODE_NAME));
        genKeyToken(true);
        break;
      }

      case AK_MASTER_MAT_ID:
      {
        gentoken();
        if(curtok != PT_INTEGER)
          sxpanic("%s is no integer", id2keyname(AK_MASTER_MAT_ID));
        mat_id=tok_int;
        mat_seen=true;
        genKeyToken(true);
        break;
      }

      case AK_MESH:
      {
        gentoken();
        if(mesh_seen)
          sxpanic("Multiple meshes in one GEOMOBJECT");
        if(!name_seen)
          sxpanic("mesh without name");
        if(incl_this && meshes)
        {
          mesh=new ASEMesh; MEM(mesh);
          mesh->parse(*this);
        }
        else
          genKeyToken(false);   // skip mesh
        break;
      }

      default:
        {
        genKeyToken(true);
        break;
        }
    }
  }
  while((curtok != PT_BEND)&&(curtok != PT_EOF));

  gentoken();

  if(mesh)
  {
    if(mat_seen)
      mesh->setMasterMatId(mat_id);
    if(!name)
      xpanic("mesh should already have the name checked");
    mesh->setName(name);
    free(name);
    if(meshes)
      meshes->add(mesh);
  }
}



void ASEParser::skipwhite(void)
{
  while(type(gchar()) == PT_WHITE)
    ;

  if(type(curch) != PT_EOF)
    push();
}



int ASEParser::gchar(void)
{
  curch=fgetc(f);
  if(curch=='\n')
    lineno++;
  return curch;
}



void ASEParser::push(void)
{
  if(curch == '\n')
    lineno--;
  ungetc(curch, f);
}



int ASEParser::type(int ch)
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
    case '{':
      return PT_BBEGIN;
    case '}':
      return PT_BEND;
    default:
      return PT_STRING;
    };
}



ASEParser::ASEParser(const char *_fname)
{
  fname=strdup(_fname);
  f=fopen(_fname,"r");
  lineno=1;
  if(f != NULL)
    gentoken();
  else
    {
      sxpanic("Cannot open input file '%s': %s",_fname,strerror(errno));
    };
}

ASEParser::~ASEParser(void)
{
  if(f!=NULL)
    fclose(f);

  if(fname!=NULL)
    free(fname);
}

void ASEParser::vprint(const char *args, va_list ap)
{
  char buffer[200];
  char *cur, *nxt;

  vsprintf(buffer,args, ap);
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


void ASEParser::skipBlock(void)
{
  int level=0;

  if(curtok == PT_BBEGIN)
    {
      do
        {
          if(curtok==PT_BBEGIN)
            level++;
          else if(curtok == PT_BEND)
            level--;
          gentoken();
        }
      while ( (curtok != PT_EOF) && (level > 0) );
    };
}

bool ASEParser::genKeyToken(bool do_gentoken)
{
  if(do_gentoken)
    gentoken();
  while(true)
    {
      switch(curtok)
	{
	case PT_KEYWORD:
	  return true;
	case PT_EOF:
	  return false;
	case PT_BEND:
	  return false;
	case PT_BBEGIN:
	  skipBlock();
	  break;
	default:
	  gentoken();
	};
    };
  return true; // never happens. avoids warnings.
}


void ASEParser::error(const char *args, ...)
{
  va_list ap;

  if(fname!=NULL)
    printf("%s, ",fname);
  printf("line %d: ",lineno);
  va_start(ap, args);  
  vprint(args,ap);
  va_end(ap);
  printf("\n");
}

void ASEParser::print(const char *args, ...)
{
  va_list ap;

  va_start(ap, args);  
  vprint(args,ap);
  va_end(ap);
}



/*
bool ASEParser::eatOpen(bool force)
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


bool ASEParser::eatClose(bool force)
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
*/

rvfloat ASEParser::getFloatValue(void)
{
  if(curtok == PT_FLOAT)
    return tok_float;
  else if(curtok==PT_INTEGER)
    return (float)tok_int;
  sxpanic("getFloatValue without float");
  return 1.0;
}


void ASEParser::printcurtok(void)
{
  switch(curtok)
    {
    case PT_EOF:
      printf("End-of-file");
      break;
    case PT_BBEGIN:
      printf("'{'");
      break;
    case PT_BEND:
      printf("'}'");
      break;
    case PT_STRING:
      printf("str'%s'",tok_text);
      break;
    case PT_KEYWORD:
      printf("key'%s'",tok_text);
      break;
    case PT_INTEGER:
      printf("int'%d'",tok_int);
      break;
    case PT_FLOAT:
      printf("float'%g'",getFloatValue());
      break;
    };
}


/*
int ASEParser::eatNumber(void)
{
  char *cur;
  int res;

  if(curtok != PT_TEXT)
    return -1;

  if(*cur == 0)
    return -1;

  cur=tok_text;
  while(*cur != 0)
    if((*cur < '0')||(*cur > '9'))
      return -1;
    else
      cur++;

  sscanf(tok_text,"%d",&res);
  gentoken();
  return res;
}
bool ASEParser::eatSNumber(int &res)
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
*/
