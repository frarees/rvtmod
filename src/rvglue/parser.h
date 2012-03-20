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
 * $Id: parser.h,v 1.1 2000/05/03 08:46:47 ali Exp ali $
 *
 * $Log: parser.h,v $
 * Revision 1.1  2000/05/03 08:46:47  ali
 * Initial revision
 *
 *
 */
#ifndef __PARSER_H
#define __PARSER_H

#include <stdarg.h>

#define PT_EOF     1
#define PT_WHITE   2
#define PT_BBEGIN  3
#define PT_BEND    4
#define PT_TEXT    5
#define PT_CBEGIN  6
#define PT_CEND    7
#define PT_LINECOM 8


/*! maximum length of text-type tokens */
#define MAX_TOK_LENGTH 50


//! parser for command files
class Parser
{
public:
  /*! constructor
   *
   * opens \a _fname for reading and initializes data
   */
  Parser(const char *_fname);

  /*! destructor
   *
   * closes command file
   */
  ~Parser();


  /*! prints an error
   *
   * \a errnum is one of the \c ERROR_ or \c WARNING_ constants from errdesc.h
   *
   * calls describe_error
   *
   * \c $$ is replaced by current token
   */
  void error(int errnum, const char *args, ...);

  /*! replaces \c "$$" by current token, including '' */
  void print(const char *args, ...);

  /*! prints current token to screen. Used for \c $$ replacements */
  void printcurtok(void);

  /*! current token type. Value is one of the \c PT_ defines. 
   *
   * class ensures that calling functions never get PT_CBEGIN, PT_CEND and PT_LINECOM.
   */
  int curtok;

  /*! string with the current token if #curtok is PT_TEXT */
  char tok_text[MAX_TOK_LENGTH];

  /*! drop current token and read next from input file */
  void gentoken(void);

  /*! eats a opening bracket '('.
   *
   * if \a force is
   * \arg \c false, the bracket is optional.
   * \arg \c true, a error is printed if there is no bracket
   *
   * \return true if there was a bracket
   */
  bool eatOpen(bool force);


  /*! eats a closing bracket ')'.
   *
   * if \a force is
   * \arg \c false, the bracket is optional.
   * \arg \c true, a error is printed if there is no bracket
   *
   * \return true if there was a bracket
   */
  bool eatClose(bool force);


  /*! eats a positive integer
   *
   * \return
   * \arg \c >=0 if there was a number
   * \arg \c -1 if there was no number
   */
  int  eatNumber(void);

  
  /*! eats a integer
   *
   * writes it into the argument
   *
   * \return
   * \arg \c true if there was a number
   * \arg \c false if there was no number
   */
  bool eatSNumber(int &);

private:
  int gchar(void);
  void push(void);
  int type(int);

  void skipwhite(void);

  /*! similar to vprintf() 
   *
   * replaces \c $$ by current token
   */
  void vprint(const char *args, va_list ap);

private:
  char *fname;
  FILE *f;
  int lineno, curch;
};

#endif
