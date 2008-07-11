/*
 * $Id$
 */

/*
 * Harbour Project source code:
 *    Wrapper functions for minizip library
 *    Some higher level zip archive functions
 *
 * Copyright 2008 Mindaugas Kavaliauskas <dbtopas.at.dbtopas.lt>
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

#define INCL_DOSFILEMGR

#include "hbapi.h"
#include "hbapiitm.h"
#include "hbapierr.h"
#include "hbdate.h"
#include "hbset.h"
#include "zip.h"
#include "unzip.h"

#if defined( HB_OS_WIN_32 )
   #include "windows.h"
#elif defined( HB_OS_UNIX )
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <unistd.h>
   #include <time.h>
   #include <utime.h>
#elif defined( HB_OS_DOS )
   #if defined(__DJGPP__) || defined(__RSX32__) || defined(__GNUC__)
      #include "hb_io.h"
      #include <sys/param.h>
      #if defined( HB_OS_DOS )
         #include <time.h>
         #include <utime.h>
      #endif
   #endif
#endif


static HB_GARBAGE_FUNC( hb_zipfile_destructor )
{
   zipFile*  phZip = (zipFile*) Cargo;

   if( * phZip )
   {
      zipClose( * phZip, NULL );
      * phZip = NULL;
   }
}


static gzFile hb_zipfileParam( int iParam )
{
   zipFile*  phZip = ( zipFile* ) hb_parptrGC( hb_zipfile_destructor, iParam );

   if( phZip && * phZip )
      return * phZip;

   hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   return NULL;
}


static HB_GARBAGE_FUNC( hb_unzipfile_destructor )
{
   unzFile*  phUnzip = (unzFile*) Cargo;

   if( * phUnzip )
   {
      unzClose( * phUnzip );
      * phUnzip = NULL;
   }
}


static gzFile hb_unzipfileParam( int iParam )
{
   unzFile*  phUnzip = ( unzFile* ) hb_parptrGC( hb_unzipfile_destructor, iParam );

   if( phUnzip && * phUnzip )
      return * phUnzip;

   hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
   return NULL;
}



/* HB_ZipOpen( cFileName, [ iMode = HB_ZIP_CREATE ] ) --> hZip */
HB_FUNC( HB_ZIPOPEN )
{
   char* szFileName = hb_parc( 1 );

   if( szFileName )
   {
      zipFile hZip = zipOpen( szFileName, ISNUM( 2 ) ? hb_parni( 2 ) : APPEND_STATUS_CREATE );

      if( hZip )
      {
         zipFile* phZip = (zipFile*) hb_gcAlloc( sizeof( zipFile ), hb_zipfile_destructor );

         * phZip = hZip;
         hb_retptrGC( phZip );
      }
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}


/*  HB_ZipClose( hZip ) --> nError */
HB_FUNC( HB_ZIPCLOSE )
{
   zipFile*  phZip = ( zipFile* ) hb_parptrGC( hb_zipfile_destructor, 1 );

   if( phZip && * phZip )
   {
      zipFile  hZip = * phZip;

      * phZip = NULL;
      hb_retni( zipClose( hZip, NULL ) );
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}


/* HB_ZipFileCreate( hZip, cZipName, dDate, cTime, nInternalAttr, nExternalAttr,
                       [ nMethod = HB_ZLIB_METHOD_DEFLATE ], 
                       [ nLevel = HB_ZLIB_COMPRESSION_DEFAULT ], 
                       [ cPassword ] ) --> nError */
HB_FUNC( HB_ZIPFILECREATE )
{
   char* szZipName = hb_parc( 2 );

   if( szZipName )
   {
      zipFile hZip = hb_zipfileParam( 1 );

      if( hZip )
      {
         int iMethod = ISNUM( 7 ) ? hb_parni( 7 ) : Z_DEFLATED;
         int iLevel = ISNUM( 8 ) ? hb_parni( 8 ) : Z_DEFAULT_COMPRESSION;
         int iY, iM, iD;

         zip_fileinfo zfi;
    
         memset( &zfi, 0, sizeof( zfi ) );
    
         hb_dateDecode( hb_pardl( 3 ), &iY, &iM, &iD );
         zfi.tmz_date.tm_year = iY;
         zfi.tmz_date.tm_mon = iM - 1;
         zfi.tmz_date.tm_mday = iD;
    
         hb_timeStrGet( hb_parc( 4 ), &iY, &iM, &iD, NULL );
         zfi.tmz_date.tm_hour = iY;
         zfi.tmz_date.tm_min = iM;
         zfi.tmz_date.tm_sec = iD;

         hb_retni( zipOpenNewFileInZip3( hZip, szZipName, &zfi, 
                                         NULL, 0, NULL, 0, NULL, iMethod, iLevel, 0, 
                                         -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                         hb_parc( 9 ), 0 ) );
      }
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}


/*  HB_ZipFileWrite( hZip, cData [, nLen ] ) --> nError */
HB_FUNC( HB_ZIPFILEWRITE )
{
   char*  pData = hb_parc( 2 );

   if( pData )
   {
      zipFile hZip = hb_zipfileParam( 1 );
      ULONG   ulLen = hb_parclen( 2 );

      if( ISNUM( 3 ) && (ULONG) hb_parnl( 3 ) < ulLen )
         ulLen = (ULONG) hb_parnl( 3 );

      if( hZip )
         hb_retni( zipWriteInFileInZip( hZip, (void*) pData, ulLen ) );
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}


/*  HB_ZipFileClose( hZip ) --> nError */
HB_FUNC( HB_ZIPFILECLOSE )
{
   zipFile hZip = hb_zipfileParam( 1 );

   if( hZip )
      hb_retni( zipCloseFileInZip( hZip ) );
}


/* HB_UnzipOpen( cFileName ) --> hUnzip */
HB_FUNC( HB_UNZIPOPEN )
{
   char*  szFileName = hb_parc( 1 );

   if( szFileName )
   {
      unzFile hUnzip = unzOpen( szFileName );
      if( hUnzip )
      {
         unzFile*  phUnzip = (unzFile*) hb_gcAlloc( sizeof( unzFile ), hb_unzipfile_destructor );

         * phUnzip = hUnzip;
         hb_retptrGC( phUnzip );
      }
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}


/*  HB_UnzipClose( hUnzip ) --> nError */
HB_FUNC( HB_UNZIPCLOSE )
{
   unzFile*  phUnzip = ( unzFile* ) hb_parptrGC( hb_unzipfile_destructor, 1 );

   if( phUnzip && * phUnzip )
   {
      unzFile  hUnzip = * phUnzip;

      * phUnzip = NULL;
      hb_retni( unzClose( hUnzip ) );
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}


/*  HB_UnzipFileFirst( hUnzip ) --> nError */
HB_FUNC( HB_UNZIPFILEFIRST )
{
   unzFile  hUnzip = hb_unzipfileParam( 1 );

   if( hUnzip )
      hb_retni( unzGoToFirstFile( hUnzip ) );
}


/*  HB_UnzipFileNext( hUnzip ) --> nError */
HB_FUNC( HB_UNZIPFILENEXT )
{
   unzFile  hUnzip = hb_unzipfileParam( 1 );

   if( hUnzip )
      hb_retni( unzGoToNextFile( hUnzip ) );
}


/*  HB_UnzipFilePos( hUnzip ) --> nPosition */
HB_FUNC( HB_UNZIPFILEPOS )
{
   unzFile  hUnzip = hb_unzipfileParam( 1 );

   if( hUnzip )
      hb_retnint( unzGetOffset( hUnzip ) );
}


/*  HB_UnzipFileGoto( hUnzip, nPosition ) --> nError */
HB_FUNC( HB_UNZIPFILEGOTO )
{
   unzFile  hUnzip = hb_unzipfileParam( 1 );

   if( hUnzip )
      hb_retni( unzSetOffset( hUnzip, (ULONG) hb_parnint( 2 ) ) );
}


/*  HB_UnzipFileInfo( hUnzip, @cZipName, @dDate, @cTime, @nInternalAttr, @nExternalAttr,
                      @nMethod, @nSize, @nCompressedSize ) --> nError */
HB_FUNC( HB_UNZIPFILEINFO )
{
   unzFile hUnzip = hb_unzipfileParam( 1 );

   if( hUnzip )
   {
      char           szFileName[ _POSIX_PATH_MAX + 1 ];
      unz_file_info  ufi;
      int            iResult;
      PHB_ITEM       pItem;
      char           buf[ 16 ];

      iResult = unzGetCurrentFileInfo( hUnzip, &ufi, szFileName, _POSIX_PATH_MAX, 
                                       NULL, 0, NULL, 0 );
      hb_retni( iResult );

      if ( iResult == UNZ_OK )
      {
         hb_storc( szFileName, 2 );

         pItem = hb_itemPutD( NULL, ufi.tmu_date.tm_year, ufi.tmu_date.tm_mon + 1, 
                                    ufi.tmu_date.tm_mday );
         hb_itemParamStoreForward( 3, pItem );
         hb_itemRelease( pItem );


         snprintf( buf, sizeof( buf ), "%02d:%02d:%02d", ufi.tmu_date.tm_hour,  
                   ufi.tmu_date.tm_min, ufi.tmu_date.tm_sec );
         hb_storc( buf, 4 );
         hb_stornl( ufi.internal_fa, 5 );
         hb_stornl( ufi.external_fa, 6 );
         hb_stornl( ufi.compression_method, 7 );
         hb_stornl( ufi.uncompressed_size, 8 );
         hb_stornl( ufi.compressed_size, 9 );
      }
      else
      {
         hb_storc( NULL, 2 );

         pItem = hb_itemPutDL( NULL, 0 );
         hb_itemParamStoreForward( 3, pItem );
         hb_itemRelease( pItem );

         hb_storc( NULL, 4 ); 
         hb_stornl( 0, 5 );
         hb_stornl( 0, 6 );
         hb_stornl( 0, 7 );
         hb_stornl( 0, 8 );
         hb_stornl( 0, 9 );
      }
   }
}


/* HB_UnzipFileOpen( hUnzip ) --> nError */
HB_FUNC( HB_UNZIPFILEOPEN )
{
   unzFile  hUnzip = hb_unzipfileParam( 1 );

   if( hUnzip )
      hb_retni( unzOpenCurrentFilePassword( hUnzip, hb_parc( 2 ) ) );
}


/* HB_UnzipFileRead( hUnzip, @cBuf [, nLen ] ) --> nRead */
HB_FUNC( HB_UNZIPFILEREAD )
{
   PHB_ITEM  pBuffer = hb_param( 2, HB_IT_STRING );

   if( pBuffer && ISBYREF( 2 ) )
   {
      unzFile hUnzip = hb_unzipfileParam( 1 );

      if( hUnzip )
      {
         ULONG     ulRead;
         ULONG     ulSize = hb_parclen( 2 );
         int       iResult;

         if( ISNUM( 3 ) )
         {
            ulRead = (ULONG) hb_parnl( 3 );
            if( ulRead < ulSize )
               ulSize = ulRead;
         }

         pBuffer = hb_itemUnShareString( pBuffer );
         iResult = unzReadCurrentFile( hUnzip, hb_itemGetCPtr( pBuffer ), ulSize );
         hb_retnl( iResult );
      }
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}


/*  HB_UnzipFileClose( hUnzip ) --> nError */
HB_FUNC( HB_UNZIPFILECLOSE )
{
   unzFile hUnzip = hb_unzipfileParam( 1 );

   if( hUnzip )
      hb_retni( unzCloseCurrentFile( hUnzip ) );
}


/*
*
*  Higher level functions - not a wrappers of minizip code
*
*/

static int hb_zipStoreFile( zipFile hZip, char* szFileName, char* szName, char* szPassword )
{
   char          * szZipName, * pString;
   FHANDLE       hFile;
   ULONG         ulLen, ulExtAttr;
   zip_fileinfo  zfi;
   int           iResult;
   BOOL          fError;

   if( szName )
   {
      /* change path separators to '/' */
      szZipName = hb_strdup( szName );

      ulLen = strlen( szZipName );
      pString = szZipName;
      while( ulLen-- )
      {
         if( pString[ ulLen ] == '\\' )
            pString[ ulLen ] = '/';
      }
   }
   else 
   {
      /* get file name */
      szZipName = hb_strdup( szFileName );

      ulLen = strlen( szZipName );
      pString = szZipName;

      while( ulLen-- )
      {
         if( pString[ ulLen ] == '/' || pString[ ulLen ] == '\\' )
         {
            memmove( szZipName, &pString[ ulLen + 1 ], strlen( szZipName ) - ulLen );
            break;
         }
      }
   }

   memset( &zfi, 0, sizeof( zfi ) );
   fError = FALSE;

#if defined( HB_OS_WIN_32 )
   {
      ulExtAttr = GetFileAttributesA( szFileName );

      if( (LONG) ulExtAttr != -1 )
      {
         ulExtAttr = GetFileAttributesA( szFileName ) & 
                     ( FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN |
                       FILE_ATTRIBUTE_SYSTEM   | FILE_ATTRIBUTE_DIRECTORY |
                       FILE_ATTRIBUTE_ARCHIVE );
   
         if( ulExtAttr | FILE_ATTRIBUTE_READONLY )
            ulExtAttr |= 0x01240000;  /* r--r--r-- */
         else
            ulExtAttr |= 0x01B60000;  /* rw-rw-rw- */

         if( ulExtAttr & FILE_ATTRIBUTE_DIRECTORY )
            ulExtAttr |= 0x40000000;
         else
            ulExtAttr |= 0x80000000;
   
         ulLen = strlen( szZipName );
         if( ulLen > 4 )
         {
            pString = &szZipName[ ulLen - 4 ];
            if( hb_stricmp( pString, ".exe" ) == 0 || 
                hb_stricmp( pString, ".com" ) == 0 ||
                hb_stricmp( pString, ".bat" ) == 0 )
            {
               ulExtAttr |= 0x00490000; /* --x--x--x */
            }
         }
      }
      else
         fError = TRUE;
   }
#elif defined( HB_OS_UNIX )
   {
      struct stat statbuf;
      struct tm   st;

      ulExtAttr = 0;

      if( stat( szFileName, &statbuf ) == 0 )
      {
         if( S_ISDIR( statbuf.st_mode ) )
         {
            ulExtAttr |= 0x40000000;
            ulExtAttr |= 0x10; /* FILE_ATTRIBUTE_DIRECTORY */
         }
         else
         {
            ulExtAttr |= 0x80000000;
            ulExtAttr |= 0x20; /* FILE_ATTRIBUTE_ARCHIVE */
         }

         ulExtAttr |= ( statbuf.st_mode & S_IRWXO ) << 16;
         ulExtAttr |= ( statbuf.st_mode & S_IRWXG ) << 15;
         ulExtAttr |= ( statbuf.st_mode & S_IRWXU ) << 14;

#if defined( HB_OS_LINUX ) && !defined( __WATCOMC__ )
         localtime_r( &statbuf.st_mtime, &st );
#else
         memcpy( &st, localtime( &statbuf.st_mtime ), sizeof( st ) );
#endif

         zfi.tmz_date.tm_sec = st.tm_sec;
         zfi.tmz_date.tm_min = st.tm_min;
         zfi.tmz_date.tm_hour = st.tm_hour;
         zfi.tmz_date.tm_mday = st.tm_mday;
         zfi.tmz_date.tm_mon = st.tm_mon - 1;
         zfi.tmz_date.tm_year = st.tm_year;
      }
      else
         fError = TRUE;
   }
#elif defined( HB_OS_DOS )

#  if defined(__DJGPP__) || defined(__RSX32__) || defined(__GNUC__)
   {
      int iAttr;

      iAttr = _chmod( szFileName, 0, 0 );
      if( iAttr != -1 )
      {
         ulExtAttr = iAttr & ( HB_FA_READONLY | HB_FA_HIDDEN | HB_FA_SYSTEM | 
                               HB_FA_DIRECTORY | HB_FA_ARCHIVE );
     
         if( ulExtAttr | HB_FA_READONLY )
            ulExtAttr |= 0x01240000;  /* r--r--r-- */
         else
            ulExtAttr |= 0x01B60000;  /* rw-rw-rw- */

         if( ulExtAttr & HB_FA_DIRECTORY )
            ulExtAttr |= 0x40000000;
         else
            ulExtAttr |= 0x80000000;
   
         ulLen = strlen( szZipName );
         if( ulLen > 4 )
         {
            pString = &szZipName[ ulLen - 4 ];
            if( hb_stricmp( pString, ".exe" ) == 0 || 
                hb_stricmp( pString, ".com" ) == 0 ||
                hb_stricmp( pString, ".bat" ) == 0 )
            {
               ulExtAttr |= 0x00490000; /* --x--x--x */
            }
         }
      }
      else
         fError = TRUE;
   }
#  else
   {
      int TODO; /* To force warning */

      ulExtAttr = 0x81B60020;  /* FILE_ATTRIBUTE_ARCHIVE | rw-rw-rw- */
   }
#  endif

#elif defined( HB_OS_OS2 )
   {
      FILESTATUS3 fs3;
      APIRET ulrc;
      ULONG ulAttr;
    
      ulrc = DosQueryPathInfo( szName, FIL_STANDARD, &fs3, sizeof( fs3 ) );
      if( ulrc == NO_ERROR )
      {
         ulAttr = 0;
         if( fs3.attrFile & FILE_READONLY )
            ulAttr |= HB_FA_READONLY;
         if( fs3.attrFile & FILE_HIDDEN )
            ulAttr |= HB_FA_HIDDEN;
         if( fs3.attrFile & FILE_SYSTEM )
            ulAttr |= HB_FA_SYSTEM;
         if( fs3.attrFile & FILE_DIRECTORY )
            ulAttr |= HB_FA_DIRECTORY;
         if( fs3.attrFile & FILE_ARCHIVED )
            ulAttr |= HB_FA_ARCHIVE;

         ulExtAttr = ulAttr; 
         if( ulExtAttr | HB_FA_READONLY )
            ulExtAttr |= 0x01240000;  /* r--r--r-- */
         else
            ulExtAttr |= 0x01B60000;  /* rw-rw-rw- */

         if( ulExtAttr & HB_FA_DIRECTORY )
            ulExtAttr |= 0x40000000;
         else
            ulExtAttr |= 0x80000000;

#if 0 /* Please enable it if .exe, .bat and .com are executable files under OS2 */
         ulLen = strlen( szZipName );
         if( ulLen > 4 )
         {
            pString = &szZipName[ ulLen - 4 ];
            if( hb_stricmp( pString, ".exe" ) == 0 || 
                hb_stricmp( pString, ".com" ) == 0 ||
                hb_stricmp( pString, ".cmd" ) == 0 ||
                hb_stricmp( pString, ".bat" ) == 0 )
            {
               ulExtAttr |= 0x00490000; /* --x--x--x */
            }
         }
#endif

         zfi.tmz_date.tm_sec = fs3.ftimeLastWrite.twosecs * 2;
         zfi.tmz_date.tm_min = fs3.ftimeLastWrite.minutes;
         zfi.tmz_date.tm_hour = fs3.ftimeLastWrite.hours;
         zfi.tmz_date.tm_mday = fs3.fdateLastWrite.day;
         zfi.tmz_date.tm_mon = fs3.fdateLastWrite.month;
         zfi.tmz_date.tm_year = fs3.fdateLastWrite.year + 1980;
      }
      else
         fError = TRUE;
   }
#else
   {
      int TODO; /* To force warning */

      ulExtAttr = 0x81B60020;  /* FILE_ATTRIBUTE_ARCHIVE | rw-rw-rw- */
   }
#endif

   if( fError )
   {
      hb_xfree( szZipName );
      return -200;
   }

   zfi.external_fa = ulExtAttr;
   /* TODO: zip.exe test: 0 for binary file, 1 for text. Does not depend on 
      extension. We should analyse content of file to determine this??? */
   zfi.internal_fa = 0; 

   if( ulExtAttr & 0x40000000 )
   {
      iResult = zipOpenNewFileInZip3( hZip, szZipName, &zfi, NULL, 0, NULL, 0, NULL, 
                                      Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0, 
                                      -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                      szPassword, 0 );
      if( iResult == 0 )
         zipCloseFileInZip( hZip );
   }
   else
   {
      hFile = hb_fsOpen( (BYTE*) szFileName, FO_READ );

      if( hFile != FS_ERROR )
      {
#if defined( HB_OS_WIN_32 )
         {
            FILETIME    ftutc, ft;
            SYSTEMTIME  st;

            if( GetFileTime( ( HANDLE ) hb_fsGetOsHandle( hFile ), NULL, NULL, &ftutc ) &&
                FileTimeToLocalFileTime( &ftutc, &ft ) & 
                FileTimeToSystemTime( &ft, &st ) )
            {
               zfi.tmz_date.tm_sec = st.wSecond;
               zfi.tmz_date.tm_min = st.wMinute;
               zfi.tmz_date.tm_hour = st.wHour;
               zfi.tmz_date.tm_mday = st.wDay;
               zfi.tmz_date.tm_mon = st.wMonth - 1;
               zfi.tmz_date.tm_year = st.wYear;
            }
         }
#endif
         iResult = zipOpenNewFileInZip3( hZip, szZipName, &zfi, NULL, 0, NULL, 0, NULL, 
                                         Z_DEFLATED, Z_DEFAULT_COMPRESSION, 0,
                                         -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY,
                                         szPassword, 0 );
         if( iResult == 0 )
         {
            pString = (char*) hb_xgrab( 1024 * 16 );
            while ( ( ulLen = hb_fsReadLarge( hFile, (BYTE*) pString, 1024 * 16 ) ) > 0 )
            {
               zipWriteInFileInZip( hZip, pString, ulLen );
            }
            hb_xfree( pString );
   
            zipCloseFileInZip( hZip );
         }
         hb_fsClose( hFile );
      }
      else
      {
         iResult = -200 - hb_fsError();
      }
   }
   hb_xfree( szZipName );
   return iResult;
}


/*  HB_ZipStoreFile( hZip, cFileName, [ cZipName ], [ cPassword ] ) --> nError */
HB_FUNC( HB_ZIPSTOREFILE )
{
   char*  szFileName = hb_parc( 2 );

   if( szFileName )
   {
      zipFile hZip = hb_zipfileParam( 1 );

      if( hZip )
         hb_retni( hb_zipStoreFile( hZip, szFileName, hb_parc( 3 ), hb_parc( 4 ) ) );
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}


static int hb_unzipExtractCurrentFile( unzFile hUnzip, char* szFileName, char* szPassword )
{
   char           szName[ _POSIX_PATH_MAX + 1 ];
   ULONG          ulPos, ulLen;
   char           cSep, * pString;
   unz_file_info  ufi;
   int            iResult;
   FHANDLE        hFile;

   iResult = unzGetCurrentFileInfo( hUnzip, &ufi, szName, _POSIX_PATH_MAX, 
                                    NULL, 0, NULL, 0 );
   if( iResult != UNZ_OK )
      return iResult;

   iResult = unzOpenCurrentFilePassword( hUnzip, szPassword );

   if( iResult != UNZ_OK )
      return iResult;

   if( szFileName )
   {
      hb_strncpy( szName, szFileName, sizeof( szName ) - 1 );
   }

   ulLen = strlen( szName );

   /* Test shows that files in subfolders can be stored to zip file without 
      explicitly adding folder. So, let's create a requred path */

   ulPos = 1;
   while( ulPos < ulLen )
   {
      cSep = szName[ ulPos ];

      /* allow both path separators, ignore terminating path separator */
      if( ( cSep == '\\' || cSep == '/' ) && ulPos < ulLen - 1 )
      {
         szName[ ulPos ] = '\0'; 
         hb_fsMkDir( (BYTE*) szName );
         szName[ ulPos ] = cSep;
      }
      ulPos++;
   }

   if( ufi.external_fa & 0x40000000 ) /* DIRECTORY */
   {
      hb_fsMkDir( (BYTE*) szName );
      iResult = UNZ_OK;
   }
   else
   {
      hFile = hb_fsCreate( (BYTE*) szName, FC_NORMAL );

      if( hFile != FS_ERROR )
      {
         pString = (char*) hb_xgrab( 1024 * 16 );

         while ( ( iResult = unzReadCurrentFile( hUnzip, pString, 16 * 1024 ) ) > 0 )
         {
            hb_fsWriteLarge( hFile, (BYTE*) pString, (ULONG) iResult );
         }
         hb_xfree( pString );

#if defined( HB_OS_WIN_32 )
         {
            FILETIME    ftutc, ft;
            SYSTEMTIME  st;
            
            st.wSecond = ufi.tmu_date.tm_sec;
            st.wMinute = ufi.tmu_date.tm_min;
            st.wHour = ufi.tmu_date.tm_hour;
            st.wDay = ufi.tmu_date.tm_mday;
            st.wMonth = ufi.tmu_date.tm_mon + 1;
            st.wYear = ufi.tmu_date.tm_year;
            st.wMilliseconds = 0;

            if( SystemTimeToFileTime( &st, &ft ) &&
                LocalFileTimeToFileTime( &ft, &ftutc ) )
            {
               SetFileTime( ( HANDLE ) hb_fsGetOsHandle( hFile ), &ftutc, &ftutc, &ftutc );
            }
         }
#endif

         hb_fsClose( hFile );
      }
      else
      {
         iResult = -200 - hb_fsError();
      }
   }  
   unzCloseCurrentFile( hUnzip );


#if defined( HB_OS_WIN_32 )
   {
      SetFileAttributesA( szName, ufi.external_fa & 0xFF );
   }
#elif defined( HB_OS_UNIX ) || defined( __DJGPP__ )
   {
      struct utimbuf   utim;
      struct tm        st;

#  if defined( __DJGPP__ )
      _chmod( szName, 1, ufi.external_fa & 0xFF );
#  else
      chmod( szName, ( ufi.external_fa & 0x00070000 ) >> 16 |
                     ( ufi.external_fa & 0x00380000 ) >> 15 |
                     ( ufi.external_fa & 0x01C00000 ) >> 14 );
#  endif
      memset( &st, 0, sizeof( st ) );

      st.tm_sec = ufi.tmu_date.tm_sec;
      st.tm_min = ufi.tmu_date.tm_min;
      st.tm_hour = ufi.tmu_date.tm_hour;
      st.tm_mday = ufi.tmu_date.tm_mday;
      st.tm_mon = ufi.tmu_date.tm_mon + 1; 
      st.tm_year = ufi.tmu_date.tm_year;

      utim.actime = mktime( &st );
      utim.modtime = utim.actime;

      utime( szName, &utim );
   }
#elif defined( HB_OS_DOS )

#  if defined(__RSX32__) || defined(__GNUC__)
   {
      _chmod( szName, 1, ufi.external_fa & 0xFF );
   }
#  else
   {
      int TODO; /* To force warning */
   }
#  endif

#elif defined( HB_OS_OS2 )
   {
      FILESTATUS3 fs3;
      APIRET ulrc;
      ULONG ulAttr = FILE_NORMAL;
      int iAttr = ufi.external_fa & 0xFF;
    
      if( iAttr & HB_FA_READONLY )
         ulAttr |= FILE_READONLY;
      if( iAttr & HB_FA_HIDDEN )
         ulAttr |= FILE_HIDDEN;
      if( iAttr & HB_FA_SYSTEM )
         ulAttr |= FILE_SYSTEM;
      if( iAttr & HB_FA_ARCHIVE )
         ulAttr |= FILE_ARCHIVED;
    
      ulrc = DosQueryPathInfo( szName, FIL_STANDARD, &fs3, sizeof( fs3 ) );
      if( ulrc == NO_ERROR )
      {
         FDATE   fdate;
         FTIME   ftime;

         fdate.year = ufi.tmu_date.tm_year - 1980;
         fdate.month = ufi.tmu_date.tm_mon;
         fdate.day = ufi.tmu_date.tm_mday;
         ftime.hours = ufi.tmu_date.tm_hour;
         ftime.minutes = ufi.tmu_date.tm_min;
         ftime.twosecs = ufi.tmu_date.tm_sec / 2;

         fs3.attrFile = ulAttr;

         fs3.fdateCreation = fs3.fdateLastAccess = fs3.fdateLastWrite = fdate;
         fs3.ftimeCreation = fs3.ftimeLastAccess = fs3.ftimeLastWrite = ftime;
         ulrc = DosSetPathInfo( szName, FIL_STANDARD,
                                &fs3, sizeof( fs3 ), DSPI_WRTTHRU );
      }
   }
#else
   {
      int TODO; /* To force warning */
   }
#endif

   return iResult;
}


/*  HB_UnzipExtractCurrentFile( hZip, [ cFileName ], [ cPassword ] ) --> nError */
HB_FUNC( HB_UNZIPEXTRACTCURRENTFILE )
{
   unzFile hUnzip = hb_unzipfileParam( 1 );

   if( hUnzip )
      hb_retni( hb_unzipExtractCurrentFile( hUnzip, hb_parc( 2 ), hb_parc( 3 ) ) );
}
