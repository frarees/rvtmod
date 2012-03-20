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
#ifndef __ASE_PARSER_H
#define __ASE_PARSER_H

#include <stdarg.h>
#include "rvgl/rvbasetypes.h"


#define PT_EOF       1
#define PT_WHITE     2
#define PT_BBEGIN    3
#define PT_BEND      4

#define PT_STRING    5
// value in tok_text

#define PT_KEYWORD   6
// value in tok_key

#define PT_INTEGER   7
// value in tok_int

#define PT_FLOAT     8
// value through getFloatValue()


#define MAX_TOK_LENGTH 200


class ASEMeshList;
class ASEMaterialList;
class NameList;

class ASEParser
{
public:
  ASEParser(const char *_fname);
  ~ASEParser();

  void error(const char *args, ...);
  void print(const char *args, ...);

  void printcurtok(void);

  int curtok;
 
  char tok_text[MAX_TOK_LENGTH];
  int  tok_int, tok_key;
  rvfloat getFloatValue(void);

  bool genKeyToken(bool do_gentoken); // reads until key on same level or '}'

  void skipBlock(void);

  void gentoken(void);

  /*  bool eatOpen(bool force);
  bool eatClose(bool force);
  int  eatNumber(void);
  bool eatSNumber(int &);*/

  void parse(ASEMeshList *meshes, ASEMaterialList *mats, NameList *names);

private:
  void parseGeomObject(ASEMeshList *meshes, NameList *names);

private:
  int gchar(void);
  void push(void);
  int type(int);

  void skipwhite(void);

  void vprint(const char *args, va_list ap);

  void judgeString(void);

private:
  rvfloat tok_float;
  char *fname;
  FILE *f;
  int lineno, curch;
};

#endif
