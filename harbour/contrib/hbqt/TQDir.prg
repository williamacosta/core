/*
 * $Id$
 */

/*
 * Harbour Project source code:
 * QT wrapper main header
 *
 * Copyright 2009 Marcos Antonio Gambeta <marcosgambeta at gmail dot com>
 * Copyright 2009 Pritpal Bedi <pritpal@vouchcac.com>
 * www - http://www.harbour-project.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307 USA (or visit the web site http://www.gnu.org/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */
/*----------------------------------------------------------------------*/


#include "hbclass.ch"


CREATE CLASS QDir

   VAR     pParent
   VAR     pPtr

   METHOD  New()

   METHOD  absoluteFilePath( cFileName )       INLINE  Qt_QDir_absoluteFilePath( ::pPtr, cFileName )
   METHOD  absolutePath()                      INLINE  Qt_QDir_absolutePath( ::pPtr )
   METHOD  canonicalPath()                     INLINE  Qt_QDir_canonicalPath( ::pPtr )
   METHOD  cd( cDirName )                      INLINE  Qt_QDir_cd( ::pPtr, cDirName )
   METHOD  cdUp()                              INLINE  Qt_QDir_cdUp( ::pPtr )
   METHOD  count()                             INLINE  Qt_QDir_count( ::pPtr )
   METHOD  dirName()                           INLINE  Qt_QDir_dirName( ::pPtr )
   METHOD  entryInfoList( pNameFilters, nFilters, nSort )  INLINE  Qt_QDir_entryInfoList( ::pPtr, pNameFilters, nFilters, nSort )
   METHOD  entryInfoList_1( nFilters, nSort )  INLINE  Qt_QDir_entryInfoList_1( ::pPtr, nFilters, nSort )
   METHOD  entryList( pNameFilters, nFilters, nSort )  INLINE  Qt_QDir_entryList( ::pPtr, pNameFilters, nFilters, nSort )
   METHOD  entryList_1( nFilters, nSort )      INLINE  Qt_QDir_entryList_1( ::pPtr, nFilters, nSort )
   METHOD  exists( cName )                     INLINE  Qt_QDir_exists( ::pPtr, cName )
   METHOD  exists_1()                          INLINE  Qt_QDir_exists_1( ::pPtr )
   METHOD  filePath( cFileName )               INLINE  Qt_QDir_filePath( ::pPtr, cFileName )
   METHOD  filter()                            INLINE  Qt_QDir_filter( ::pPtr )
   METHOD  isAbsolute()                        INLINE  Qt_QDir_isAbsolute( ::pPtr )
   METHOD  isReadable()                        INLINE  Qt_QDir_isReadable( ::pPtr )
   METHOD  isRelative()                        INLINE  Qt_QDir_isRelative( ::pPtr )
   METHOD  isRoot()                            INLINE  Qt_QDir_isRoot( ::pPtr )
   METHOD  makeAbsolute()                      INLINE  Qt_QDir_makeAbsolute( ::pPtr )
   METHOD  mkdir( cDirName )                   INLINE  Qt_QDir_mkdir( ::pPtr, cDirName )
   METHOD  mkpath( cDirPath )                  INLINE  Qt_QDir_mkpath( ::pPtr, cDirPath )
   METHOD  nameFilters()                       INLINE  Qt_QDir_nameFilters( ::pPtr )
   METHOD  path()                              INLINE  Qt_QDir_path( ::pPtr )
   METHOD  refresh()                           INLINE  Qt_QDir_refresh( ::pPtr )
   METHOD  relativeFilePath( cFileName )       INLINE  Qt_QDir_relativeFilePath( ::pPtr, cFileName )
   METHOD  remove( cFileName )                 INLINE  Qt_QDir_remove( ::pPtr, cFileName )
   METHOD  rename( cOldName, cNewName )        INLINE  Qt_QDir_rename( ::pPtr, cOldName, cNewName )
   METHOD  rmdir( cDirName )                   INLINE  Qt_QDir_rmdir( ::pPtr, cDirName )
   METHOD  rmpath( cDirPath )                  INLINE  Qt_QDir_rmpath( ::pPtr, cDirPath )
   METHOD  setFilter( nFilters )               INLINE  Qt_QDir_setFilter( ::pPtr, nFilters )
   METHOD  setNameFilters( pNameFilters )      INLINE  Qt_QDir_setNameFilters( ::pPtr, pNameFilters )
   METHOD  setPath( cPath )                    INLINE  Qt_QDir_setPath( ::pPtr, cPath )
   METHOD  setSorting( nSort )                 INLINE  Qt_QDir_setSorting( ::pPtr, nSort )
   METHOD  sorting()                           INLINE  Qt_QDir_sorting( ::pPtr )

   ENDCLASS

/*----------------------------------------------------------------------*/

METHOD New( pParent ) CLASS QDir

   ::pParent := pParent

   ::pPtr := Qt_QDir( pParent )

   RETURN Self

/*----------------------------------------------------------------------*/

