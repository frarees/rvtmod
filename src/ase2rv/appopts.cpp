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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rvgl/print.h"
#include "rvgl/helpfunc.h"
#include "rvgl/rvtypes.h"
#include "appopts.h"
#include "aseprint.h"
#include "namelist.h"


#define DEFAULT_TAZ_FILE "Tazfile"
#define DEFAULT_VIS_FILE "Visfile"

AppOptions::AppOptions()
{
  ase_name[0]='\0';
  list_name[0]='\0';
  out_name[0]='\0';
  cmd=CmdNone;
  wmode=WmNone;
  names=new NameList(); MEM(names);
  msglevel=1;
  safety_dist=0.0;
  sgdump_only=false;
  listsyntax_only=false;
  listobj_only=false;
  center_grip=false;
}



AppOptions::~AppOptions()
{
  delete names;
}



const char *AppOptions::appname()
{
  switch(cmd)
  {
    case CmdWorld:  return "ase2w";
    case CmdPrm:    return "ase2prm";
    case CmdVis:    return "ase2vis";
    case CmdTaz:    return "ase2taz";
    default:        xpanic("Don't know what asetool I am");
  }
  /*NOTREACHED*/
  return NULL;
}



const char *AppOptions::output_ext()
{
  switch(cmd)
  {
    case CmdWorld:  return SUFFIX_W_STD;
    case CmdPrm:    return SUFFIX_PRM_STD;
    case CmdVis:    return SUFFIX_VIS;
    case CmdTaz:    return SUFFIX_TAZ;
    default:        xpanic("Don't know what asetool I am");
  }
  /*NOTREACHED*/
  return NULL;
}



void AppOptions::parse( int argc, char **argv )
{
  int i;

  if(cmd==CmdNone)
    xpanic("Don't know what asetool I am");

  if(argc==1)
  {
    usage();
    exit(0);
  }

  for(i=1; i<argc; i++)
  {
    if(argv[i][0]=='-')
    {
      if(!strcasecmp(argv[i], "-l"))
      {
        listobj_only=true;
      }
      else if(!strcasecmp(argv[i], "-v"))
      {
        msglevel=2;
      }
      else if(!strcasecmp(argv[i], "-q"))
      {
        msglevel=0;
      }
      else if(!strcasecmp(argv[i], "-ase"))
      {
        aprintf=asemode_printf;
      }
      else if(!strncasecmp(argv[i], "-o=", 3))
      {
        if(out_name[0])
          cmderr("-o specified multiple times");
        strcpy(out_name, argv[i]+3);
        if(!filehastype(out_name, output_ext()))
          cmderr("output file without .%s extension", output_ext());
      }
      else if((cmd==CmdWorld || cmd==CmdPrm) && (!strcasecmp(argv[i], "-morph") || !strcasecmp(argv[i], "-m")))
      {
        if(wmode!=WmNone)
          cmderr("-ali|-morph specified multiple times");
        wmode=WmMorph;
      }
      else if((cmd==CmdWorld || cmd==CmdPrm) && (!strcasecmp(argv[i], "-ali") || !strcasecmp(argv[i], "-a")))
      {
        if(wmode!=WmNone)
          cmderr("-ali|-morph specified multiple times");
        wmode=WmAli;
      }
      else if((cmd==CmdWorld || cmd==CmdPrm) && !strcasecmp(argv[i], "-sg"))
      {
        sgdump_only=true;
        return;
      }
      else if((cmd==CmdPrm) && !strcasecmp(argv[i], "-c"))
      {
        center_grip=true;
      }
      else if((cmd==CmdVis || cmd==CmdTaz) && !strncasecmp(argv[i], "-g=", 3))
      {
        if(safety_dist)
          cmderr("-grow specified multiple times");
        safety_dist=atof(argv[i]+3);
        if(!safety_dist)
          cmderr("-g is invalid or 0");
      }
      else if((cmd==CmdVis || cmd==CmdTaz) && !strncasecmp(argv[i], "-f=", 3))
      {
        if(list_name[0])
          cmderr("-f specified multiple times");
        strcpy(list_name, argv[i]+3);
      }
      else if((cmd==CmdVis || cmd==CmdTaz) && !strcasecmp(argv[i], "-sy"))
      {
        listsyntax_only=true;
        return;
      }
      else
      {
        cmderr("unknown option '%s'", argv[i]);
      }
    }
    else
    {
      if(!ase_name[0])
        strcpy(ase_name, argv[i]);
      else if(names->add(argv[i], true))
        fprintf(stderr, "*** %s: Warning: object '%s' is duplicated\n", appname(), argv[i]);
    }
  }

  if(!ase_name[0])
    cmderr("no input file");

  if(!filehastype(ase_name, "ase"))
    cmderr("input file without .ase extension");

  if(!out_name[0])
  {
    strcpy(out_name, ase_name);
    replaceExt(out_name, output_ext());
  }

  if(cmd==CmdVis && !list_name[0])
  {
    strcpy(list_name, ase_name);
    replaceFname(list_name, DEFAULT_VIS_FILE);
  }
  else if(cmd==CmdTaz && !list_name[0])
  {
    strcpy(list_name, ase_name);
    replaceFname(list_name, DEFAULT_TAZ_FILE);
  }
}



void AppOptions::usage()
{
  const char *outputtype = NULL;
  const char *defaultlistfile = NULL;

  switch(cmd)
  {
    case CmdWorld:
      outputtype="World";
      break;

    case CmdPrm:
      outputtype="Instance";
      break;

    case CmdVis:
      outputtype="Vizibox";
      defaultlistfile=DEFAULT_VIS_FILE;
      break;

    case CmdTaz:
      outputtype="Track Zone";
      defaultlistfile=DEFAULT_TAZ_FILE;
      break;

    default:
      xpanic("Don't know what asetool I am");
  }

  /* 80:  123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 */
  printf("Usage: %s [option]... <file>.ase [object]...\n\n", appname());
  printf("  Converts .ase files to %s files\n\n", outputtype);
  printf("  options:\n");
  printf("    -v             verbose output\n");
  printf("    -q             quiet (suppress most output)\n");
  printf("    -o=<outfile>   output file name (defaults to <file>.%s)\n", output_ext());
  if(cmd==CmdWorld || cmd==CmdPrm)
  {
  printf("    -morph or -m   use morph smoothing group mapping for surface properties\n");
  printf("    -ali or -a     use ali smoothing group mapping for surface properties\n");
  printf("    -sg            print smoothing group meaning (and do nothing more)\n");
  }
  if(cmd==CmdPrm)
  {
  printf("    -c             center grip point on instance\n");
  }
  printf("    -l             list objects (and do nothing more)\n");
  if(cmd==CmdTaz || cmd==CmdVis)
  {
  printf("    -f=<listfile>  listfile name (defaults to <.ase directory>/%s)\n", defaultlistfile);
  printf("    -sy            printf listfile syntax (and do nothing more)\n");
  }
  if(cmd==CmdTaz)
  {
  printf("    -g=<num>       grow Track Zone boxes with <num> each side\n\n");
  }
  else if(cmd==CmdVis)
  {
  printf("    -g=<num>       grow Camera boxes (not Cubes) with <num> each side\n\n");
  }
  printf("  object:\n");
  printf("    Name of mesh object in .ase file.\n");
  printf("    If object list is given, only the listed ones are processed.\n");
  printf("    If omitted, all objects are processed.\n\n");
}



void AppOptions::welcome()
{
  if(cmd==CmdWorld || cmd==CmdPrm)
  {
    /* 80:      123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 */
    lprintf(1, " ------------------------------------------------------------------------------\n");
    lprintf(1, "| %s version %s,\n", appname(), APPVERSION);
    lprintf(1, "| Copyright (C) 2000-2002 Alexander Kroeller, Steven Ellis and Gabor Varga.\n");
    lprintf(1, "| %s is free software, you can redistribute it under certain conditions.\n", appname());
    lprintf(1, "| %s comes with ABSOLUTELY NO WARRANTY; for details see 'doc\\GPL.html'.\n", appname());
    lprintf(1, " ------------------------------------------------------------------------------\n");
  }
  else
  {
    /* 80:      123456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789 */
    lprintf(1, " ------------------------------------------------------------------------------\n");
    lprintf(1, "| %s version %s, Copyright (C) 2001-2002 Gabor Varga.\n", appname(), APPVERSION);
    lprintf(1, "| %s is free software, you can redistribute it under certain conditions.\n", appname());
    lprintf(1, "| %s comes with ABSOLUTELY NO WARRANTY; for details see 'doc\\GPL.html'.\n", appname());
    lprintf(1, " ------------------------------------------------------------------------------\n");
  }
}



void AppOptions::cmderr(const char *fmt, ...)
{
  va_list ap;

  fprintf(stderr, "%s: ", appname());
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);
  fprintf(stderr, "\n");
  exit(1);

}


void AppOptions::warn_untouched()
{
  NameList *untouched=names->untouched();
  if(untouched->count())
  {
    fprintf(stderr, "*** %s: Warning: %d object(s) not found (and ignored):\n",
      appname(), untouched->count());
    for(int i=0; i<untouched->count(); i++)
      fprintf(stderr, " '%s'\n", untouched->at(i));
  }
  delete untouched;
}
