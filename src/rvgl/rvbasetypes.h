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
 * $Id: rvbasetypes.h,v 1.2 2000/05/03 09:16:11 ali Exp ali $
 *
 * $Log: rvbasetypes.h,v $
 * Revision 1.2  2000/05/03 09:16:11  ali
 * ces sync
 *
 * Revision 1.1  2000/05/03 08:49:54  ali
 * Initial revision
 */
#ifndef __RVBASETYPES_H
#define __RVBASETYPES_H

#ifdef WIN32  // ces 2000-04-07
#pragma warning(disable: 4056)
#pragma warning(disable: 4244)
#pragma warning(disable: 4305)
#pragma warning(disable: 4756)
#endif

/*! 32-bit floating point value */
typedef float                rvfloat;

/*! 16-bit signed integer */
typedef          short int   rvshort;


/*! 16-bit unsigned integer */
typedef unsigned short int   rvushort;


/*! 32-bit signed integer */
typedef                int   rvlong;


/*! 32-bit unsigned integer */
typedef unsigned       int   rvulong;


/*!
 * Just a placeholder to mark member function that are intended to
 * be overwritten by subclasses and serve as a "hook".
 */
#define hook


/*!
 * square function, implemented as f*f to be more accurate than pow()
 */
#define SQR(f) ((f)*(f))

/*!
 * Very small number used for FEQUAL
 */
#define EPSILON (0.001) // vag 2001-05-11: 0.0001 proved too small!

/*!
 * macro to do comparison of floats without worrying about
 * numerical errors.
 */
#define FEQUAL(a,b)   ( fabs((a)-(b))<EPSILON )

#endif
