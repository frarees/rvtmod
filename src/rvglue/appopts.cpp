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
 * $Id: appopts.cpp,v 1.1 2000/05/03 08:49:36 ali Exp ali $
 *
 * $Log: appopts.cpp,v $
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
#include "rvgl/rvtypes.h"

#include "appopts.h"
#include "errdesc.h"

/* definition of extern object in appopts.h */
AppOptions appopt;




AppOptions::AppOptions()
{
  msglevel=1;
  errhelp=true;
  baseoutname=woutname=ncpoutname=inputfname=NULL;
}

AppOptions::~AppOptions()
{
  if(woutname != NULL)
    free(woutname);
  if(ioutname != NULL)
    free(ioutname);
  if(ncpoutname != NULL)
    free(ncpoutname);
  if(baseoutname != NULL)
    free(baseoutname);
}

void AppOptions::usage(void)
{
  printf("rvglue version %s\n\n",APPVERSION);
  
  printf("Usage: rvglue [options] command-file\n");
  printf("where options are:\n");

  printf("  -o=filename, -output=filename\n");
  printf("                writes to <filename>.w and <filename>.ncp,\n");
  printf("                ignores \"create\" statement.\n");
  //  printf("  -f, -final    Does lengthy optimizations before writing\n");
  printf("  -v, -verbose  Prints what is currently being done\n");
  printf("  -d, -debug    Prints lots of information while working\n");
  printf("  -q, -quiet    Suppresses most output\n");
  printf("  -n, -nohelp   Suppresses printing of help texts when\n");
  printf("                reporting errors.\n");
  printf("  -h, -help     Print this help message and exit.\n");
}


bool AppOptions::setOutName(const char *basename)
{
  int blgt;
  char *dupped, *suff;

  dprintf("o","AppOptions::setOutName(%s)\n",basename);

  if(woutname != NULL)
    return true;

  dupped=strdup(basename); MEM(dupped);

  suff=breakfname(dupped);
  
  if((strcasecmp(suff,SUFFIX_PRM_STD)==0) || (strcasecmp(suff,SUFFIX_PRM_IM)==0))
    create_instance=true;
  else if((strcasecmp(suff,SUFFIX_W_STD)==0) || (strcasecmp(suff,SUFFIX_W_IM)==0) || (*suff==0))
    create_instance=false;
  else
    {
      perror(0,ERROR_OFILE_SUFFIX,"Cannot determinate target format: '%s' is illegal suffix\n",suff);
      return false;
    };

  blgt=strlen(dupped);
  woutname=(char*)malloc(blgt + 5); MEM(woutname);
  ioutname=(char*)malloc(blgt + 5); MEM(ioutname);
  ncpoutname=(char*)malloc(blgt + 5); MEM(ncpoutname);

  baseoutname=strdup(dupped); MEM(baseoutname);

  strcpy(woutname,dupped);
  strcpy(ioutname,dupped);
  if(create_instance)
    {
      strcat(woutname,SUFFIX_DOT_PRM_STD); 
      strcat(ioutname,SUFFIX_DOT_PRM_IM);
    }
  else
    {
      strcat(woutname,SUFFIX_DOT_W_STD);
      strcat(ioutname,SUFFIX_DOT_W_IM);
    };
  strcpy(ncpoutname,dupped); strcat(ncpoutname,SUFFIX_DOT_NCP);

  dprintf("o","AppOptions: Outnames W(%s), I(%s), NCP(%s)\n",
	  woutname,ioutname,ncpoutname);

  free(dupped);

  return true;
}


bool AppOptions::trySetOutName(char *fn)
{
  if(woutname != NULL)
    {
      perror(0,ERROR_DUPLICATE_OFILE,"Multiple output file names ('%s' and '%s')",woutname,fn);
      return false;
    }
  else if(fn[0]==0)
    {
      perror(0,ERROR_EMPTY_OFILE,"Empty output file name");
      return false;
    }
  else
    {
      return setOutName(fn);
    };
}

bool AppOptions::parseArgs(int argc, char **argv)
{
  int i;

  for(i=1;i<argc;i++)
    if(argv[i][0]!=0)
      {
	if((strcasecmp(argv[i],"-v")==0)||(strcasecmp(argv[i],"-verbose")==0))
	  msglevel=2;
	else if((strcasecmp(argv[i],"-d")==0)||(strcasecmp(argv[i],"-debug")==0))
	  msglevel=3;
	else if(strncmp(argv[i],"-XD",3)==0)
	  strcpy(debugsyms, &argv[i][3]);
	else if((strcasecmp(argv[i],"-q")==0)||(strcasecmp(argv[i],"-quiet")==0))
	  {
	    msglevel=0;
	    errhelp=false;
	  }
	else if((strcasecmp(argv[i],"-n")==0)||(strcasecmp(argv[i],"-nohelp")==0))
	  errhelp=false;
	else if((strcasecmp(argv[i],"-h")==0)||(strcasecmp(argv[i],"-help")==0))
	  {
	    usage();
	    return false;
	  }
	else if(strncasecmp(argv[i],"-o=",3)==0)
	  {
	    if(!trySetOutName(&argv[i][3]))
	      return false;
	  }
	else if(strncasecmp(argv[i],"-outname=",9)==0)
	  {
	    if(!trySetOutName(&argv[i][9]))
	      return false;
	  }
	else if(argv[i][0]=='-')
	  {
	    printf("Unknown option '%s' ignored\n",argv[i]);
	  }
	else if(inputfname != NULL)
	  {
	    perror(0,ERROR_DUPLICATE_IFILE,"Multiple input files specified ('%s' and '%s')",inputfname,argv[i]);
	    return false;
	  }
	else
	  inputfname=argv[i];
      };
  return true;
}
