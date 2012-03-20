/*
 *  Part of ase2rv source code,
 *  Copyright (C) 2001 Gabor Varga (bootes@freemail.hu)
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
#ifndef __APP_OPTIONS_H
#define __APP_OPTIONS_H

class NameList;

enum Command
{
  CmdNone,
  CmdWorld,
  CmdPrm,
  CmdTaz,
  CmdVis
};

enum WorkMode
{
  WmNone,
  WmMorph,
  WmAli
};

class AppOptions
{
public:
  AppOptions();
  ~AppOptions();

public:
  const char *appname();
  const char *output_ext();
  void parse(int argc, char **argv);
  void usage();
  void welcome();
  void cmderr(const char *fmt, ...);
  void warn_untouched();

public:

  char ase_name[200];
  char list_name[200];
  char out_name[200];
  Command cmd;
  WorkMode wmode;
  NameList *names;
  int msglevel;
  double safety_dist;
  bool sgdump_only;
  bool listsyntax_only;
  bool listobj_only;
  bool center_grip;
};

#endif
