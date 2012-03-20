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
 * $Id: errdesc.cpp,v 1.1 2000/05/03 08:49:36 ali Exp ali $
 *
 * $Log: errdesc.cpp,v $
 * Revision 1.1  2000/05/03 08:49:36  ali
 * Initial revision
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_DMALLOC
#include <dmalloc.h>
#endif

#include "rvgl/rvtypes.h"

#include "errdesc.h"
#include "appopts.h"

void describe_error(int errnum)
{
  if(appopt.errhelp)
    switch(errnum)
      {

	/********* ERRORS ********/

      case ERROR_ITOP_SYNTAX:
	printf("   (The input file contains something at top level that is neither a file name\n");
	printf("    nor one of the keywords like `global' or `create')\n");
	break;
      case ERROR_ITOP_FILETYPE:
	printf("   (You can insert .fin and .w files only. If you rename files, make sure that\n");
	printf("    their extensions are not changed.)\n");
	break;
      case ERROR_FILE_ROPEN:
	printf("   (A file could not be opened. Make sure that the file exists in the current\n");
	printf("    directory, is readable and has the correct name.)\n");
	break;
      case ERROR_EOF_IN_BLOCK:
	printf("   (Reached end of file while reading a block. This is likely due to a\n");
	printf("    forgotten closing bracket.)\n");
	break;
      case ERROR_EXPECTED_FNAME:
	printf("   (A filename was expected, but something else found.)\n");
	break;
      case ERROR_MAXFOPTS_EXCEEDED:
	printf("   (You exceeded an internal limit (MAX_FOPTS in this case). Increase that value\n");
	printf("    in app\\optkeep.h and recompile the program.)\n");
	break;
      case ERROR_DUPLICATE_IFILE:
	printf("   (You specified more than one command file. This may be because you forgot\n");
	printf("    the minus sign '-' of an command line option. Type 'rvglue -help' for a list\n");
	printf("    of supported options)\n");
	break;
      case ERROR_NO_IFILE:
	printf("   (You forgot to specify what command file you want me to process.\n");
	printf("    Type 'rvglue -help' for a list of supported options)\n");
	break;
      case ERROR_DUPLICATE_OFILE:
	printf("   (There are more than one '-o=...' and '-output=...' options.\n");
	printf("    Type 'rvglue -help' for a list of supported options)\n");
	break;
      case ERROR_EMPTY_OFILE:
	printf("   (There is a '-o=...' or '-output=...' option, but no filename is\n");
	printf("    following or there is a space character after the '=')\n");
	break;
      case ERROR_BEND_EXPECTED:
	printf("   (The block that is currently being read should be closed here, because\n");
	printf("    there is nothing that could stand here)\n");
	break;
      case ERROR_UNKNOWN_OPTION:
	printf("   (There is something in an option list that is none of the supported\n");
	printf("    options. Check for typos)\n");
	break;
      case ERROR_EXPECTED_KEY:
	printf("   (The program expected a legal value in this position, like a special\n");
	printf("    keyword or numerical value, but found something invalid.)\n");
	break;
      case ERROR_EXPECTED_TED_KEY:
	printf("   (Something strange is going on...)\n");
	break;
      case ERROR_BBEGIN_EXPECTED:
	printf("   (Expected an opening bracket '(' in this place. Check you command file\n");
	printf("    for errors)\n");
	break;
      case ERROR_NUMBER_RANGE:
	printf("   (The number you wrote here is out of range. There are limitations for\n");
	printf("    most values - read the documentation, it lists valid ranges)\n");
	break;
      case ERROR_OFILE_SUFFIX:
	printf("   (The file name you specified for output has an unsupported suffix. Valid\n");
	printf("    suffices are only '%s', '.%s', '.%s' and '.%s'.)\n",
	       SUFFIX_DOT_W_STD,SUFFIX_DOT_W_IM,SUFFIX_DOT_PRM_STD,SUFFIX_DOT_PRM_IM);
	break;
      case ERROR_EXPECTED_NUMBER:
	printf("   (Expected a decimal number in this place, but found something else.)\n");
	break;
      case ERROR_AMBIGUOUS_NAME:
	printf("   (Found multiple possibilities for an instance. The .fin files only hold\n");
	printf("    the first eight characters of file names, so rvglue tries to guess what\n");
	printf("    the full name is. This failed just now.)\n");
	break;
      case ERROR_MULTIPLE_W_MOD:
	printf("   (Some options have limitations on where they may appear and how often they\n");
	printf("    may appear. This one belongs to them.)\n");
	break;
      case ERROR_TEXDEF_SYNTAX:
	printf("   (The texture definition you specified does not follow the syntax rules for\n");
	printf("    textures. Read the documentation and correct your definition.)\n");
	break;
      case ERROR_TEXDEF_SMALL:
	printf("   (The size of the texture is so small that the result will most likely\n");
	printf("    consist of rounding errors only and would have nothing to do with what\n");
	printf("    you wanted to get.)\n");
	break;
      case ERROR_TEXDEF_WILDCARD:
	printf("   (Using a '*' as a wildcard in texture definitions is not always allowed.\n");
	printf("    Read the documentation for the option you're using)\n");
	break;
      case ERROR_W_IN_AND_OUT:
	printf("   (You seem to be trying to modify a .w file by reading and writing the\n");
	printf("    same file. That is the most common of all mistakes people do with this\n");
	printf("    program. See the FAQ for details (question #21))\n");
	break;





	/************ WARNINGS **********/

      case ERROR_COMMENT_EOF:
	printf("   (Detected a comment that is never closed. Everything that follows the opening\n");
	printf("    bracket `[' is treated as comment.)\n");
	break;
      case ERROR_COMMENT_NOBEGIN:
	printf("   (Detected a `]' where no preceeding `[' was found. This is not fatal, but\n");
	printf("    may be a hint that you screwed up your comments in the command file and\n");
	printf("    get unexpected results or errors.)\n");
	break;
      case ERROR_EMPTY_FILEOPT:
	printf("   (An option set for a given instance name is missing.)\n");
	break;
      case ERROR_DUPLICATE_FILEOPT:
	printf("   (An instance name with options is defined multiple times. Every option set\n");
	printf("    except the first one is ignored.)\n");
	break;
      case ERROR_DUPLICATE_GLOBAL:
	printf("   (The command file contains more than one 'global' section. Only the first\n");
	printf("    is used, the other ones are discarded.)\n");
	break;
      case ERROR_USELESS_BLOCK:
	printf("   (There is a block indicated by brackets '(...)' where no one was expected\n");
	printf("    The block is thus useless and will be ignored)\n");
	break;
      case ERROR_W_ENV_NOT_FOUND:
	printf("   (There are still undecoded parts in some .w files. This one does contain\n");
	printf("    such a part which makes it impossible for rvglue to find the values for\n");
	printf("    the env mapping. Every env-mapping enabled polygon will use a default\n");
	printf("    color for the mapping)\n");
	break;
      case ERROR_IM_HEADER:
	printf("   (An intermediate file contains a corrupt header. This may indicate that\n");
	printf("    the whole file is corrupt, and it is possible that rvglue or re-volt\n");
	printf("    crashes because of that file)\n");
	break;
      case ERROR_TEXDEF_TRAIL:
	printf("   (The texture definition contains something where nothing was expected.\n");
	printf("    That trailing garbage will be ignored. If the result doesn't look like\n");
	printf("    what you wanted, check the texture definition.)\n");
	break;
      case ERROR_FIN_MATCHES_W:
	printf("   (You seem to be trying to merge a .fin into a track's body. Keep in mind\n");
	printf("    that, if the .fin has the same name as the track, instances will still be\n");
	printf("    loaded from the .fin, and thus being displayed two times. See the FAQ for\n");
	printf("    details (question #22))\n");
	break;


      default:
	printf("   (No further help available)\n");
      };
}

void perror(int level, int errnum, const char *args, ...)
{
  va_list ap;

#ifdef _USE_VTCODES
  printf("\e[1m");
#endif
  if(errnum>0)
    {
      if(ERROR_IS_WARNING(errnum))
	printf("Warning: ");
      else
	printf("Error: ");
    }
  va_start(ap, args);  
  vprintf(args, ap);
  va_end(ap);
#ifdef _USE_VTCODES
  printf("\e[0m");
#endif
  printf("\n");

  if(errnum > 0)
    describe_error(errnum);
}

