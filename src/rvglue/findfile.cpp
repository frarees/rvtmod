/*
 *  Part of rvglue source code,
 *  Copyright (C) 2000 Alexander Kroeller (alilein@gmx.de)
 *  Copyright (C) 2000 Christer Sundin (ces) (christer@sundin.nu)
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
 * $Id: findfile.cpp,v 1.2 2000/05/03 09:44:39 ali Exp ali $
 *
 * $Log: findfile.cpp,v $
 * Revision 1.2  2000/05/03 09:44:39  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:49:37  ali
 * Initial revision
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <sys/stat.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/print.h"
#include "rvgl/helpfunc.h"


#if defined(_GLUE_USE_POSIX_DIRS)
  #include <dirent.h>
#elif defined(_GLUE_USE_WIN_DIRS)
  #include <windows.h>
#else
  #error Neither _GLUE_USE_POSIX_DIRS nor _GLUE_USE_WIN_DIRS defined!
#endif






#ifdef _GLUE_USE_POSIX_DIRS





bool _p_find_file_matches(const char *fname, const char *base, bool allow_longer, const char *suff)
{
  char *bname, *sname;

  bname=strdup(fname); MEM(bname);
  sname=breakfname(bname);

  if(strcasecmp(sname,suff) != 0)
    {
      //      printf("    drop %s: str(suff)\n",fname);
      free(bname);
      return false;
    };

  if(allow_longer)
    {
      if(strncasecmp(bname,base, strlen(base)) != 0)
	{
	  //	  printf("    drop %s: strn(base)\n",fname);
	  free(bname);
	  return false;
	};
    }
  else
    {
      if(strcasecmp(bname,base) != 0)
	{
	  //	  printf("    drop %s: str(base)\n",fname);
	  free(bname);
	  return false;
	};
    };
  
  free(bname);
  return true;
}




bool _p_find_file_regular(const char *fname)
{
  struct stat statinf;

  if(stat(fname,&statinf)!=0)
    return false;

  if(S_ISREG(statinf.st_mode))
    return true;

  return false;
}

int _p_find_file_1(const char *base, bool allow_longer, char **match, const char *suff)
{

  DIR *dp;
  struct dirent *dirp;
  int count=0;

  //  printf("search %s*.%s\n",base,suff);

  if((dp=opendir("."))==NULL)
    return 0;

  while((dirp=readdir(dp))!=NULL)
    {
      if(_p_find_file_matches(dirp->d_name, base, allow_longer, suff))
	if(_p_find_file_regular(dirp->d_name))
	  {
	    if(*match == NULL)
	      {
		*match=strdup(dirp->d_name); MEM(match);
	      }
	    else if(strlen(dirp->d_name) < strlen(*match))
	      {
		// closer match
		free(*match);
		*match=strdup(dirp->d_name); MEM(match);		
	      }
	    count++;
	    //	    printf("  match: %s\n",dirp->d_name);
	  };
    };

  closedir(dp);

  // now, look for an perfect match
  if(*match != NULL)
    {
      if(strlen(*match) == strlen(base) + strlen(suff) + 1)
	count = 1;
    };

  return count;
}



#else

// _GLUE_USE_WIN_DIRS part:

int _p_find_file_1(  // ces 2000-04-07
	const char *base, bool allow_longer, char **match, const char *suff)
{
	int count = 0;
	char Pattern[MAX_PATH];

	if (allow_longer) {
		sprintf(Pattern, "%s*.%s", base, suff);
	} else {
		sprintf(Pattern, "%s.%s", base, suff);
	}

	HANDLE          Handle;
	WIN32_FIND_DATA FindData;

	Handle = FindFirstFile(Pattern, &FindData);

	if(Handle != INVALID_HANDLE_VALUE) {
		do {
			if (!(FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				*match = _strdup(FindData.cFileName);
				count++;
				break;
			}
		} while(FindNextFile(Handle, &FindData) == TRUE);

		FindClose(Handle);
	}

	return count;
}

#endif

int find_file(const char *base, bool allow_longer, char **match, const char *suff, ...)
{
  int res=0;
  va_list ap;

  va_start(ap, suff);
  *match = NULL;
  while (suff != NULL)
    {
      res = _p_find_file_1(base,allow_longer,match,suff);
      if(res != 0)
	break;

      suff = va_arg(ap, const char *);
    };

  va_end(ap);

  return res;
}
