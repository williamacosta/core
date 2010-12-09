/*
 * $Id: hblzf.c 15096 2010-12-08 14:03:07Z petr_ch $
 */

/*
 * Harbour Project source code:
 *    LZF functions wrapper
 *
 * Copyright 2010 Petr Chornyj <myorg63@mail.ru>
 * www - http://harbour-project.org
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

/* TOFIX: Some platforms don't have this. Build breaker. */
#include <errno.h>

#include "hbapi.h"
#include "hbapierr.h"
#include "hbapiitm.h"

#include "lzf.h"
#include "lzfP.h"

#if ULTRA_FAST
#  define HB_LZF_OPTIMIZED_FOR    1
#elif VERY_FAST
#  define HB_LZF_OPTIMIZED_FOR    0
#endif

#define LZF_BUFFSIZE  1024

/* TOFIX: Make it MT compatible */
HB_SIZE s_delta = 0;
HB_SIZE s_buffer_size = LZF_BUFFSIZE;

/**
   Return a LZF_VERSION, API version
*/

HB_FUNC( HB_LZF_VERSION )
{
   hb_retni( LZF_VERSION );
}

/**
   Return 1 if lzf was optimized for speed, 0 for compression
*/

HB_FUNC( HB_LZF_OPTIMIZED_FOR )
{
   hb_retni( HB_LZF_OPTIMIZED_FOR );
}

/**
*/

HB_FUNC( HB_LZF_DELTA )
{
   hb_retni( s_delta );
   if( hb_pcount() >= 1 )
      s_delta = ( HB_SIZE ) hb_parnidef( 1, 0 );
}

/**
*/

HB_FUNC( HB_LZF_BUFFERSIZE )
{
   hb_retni( s_buffer_size );
   if( hb_pcount() >= 1 )
      s_buffer_size = ( HB_SIZE ) hb_parnidef( 1, LZF_BUFFSIZE );
}

/**
   Return a string compressed with LZF
*/

HB_FUNC( LZF_COMPRESS )
{
   PHB_ITEM pArg = hb_param( 1, HB_IT_STRING );

   if( pArg != NULL )
   {
      const char * in_data = NULL;
      char * out_data;
      HB_SIZE in_len, out_len;
      unsigned int uiResult;

      in_data = hb_itemGetCPtr( pArg );
      in_len  = hb_itemGetCLen( pArg );
      out_len = in_len + ( ( s_delta ) ? s_delta : ( ( HB_SIZE ) ( in_len * 1.04 ) + 1 ) );

      out_data = ( char * ) hb_xgrab( out_len + 1 );

      uiResult = lzf_compress( in_data, in_len, out_data, out_len );
      if( uiResult == 0 )
      {
         hb_xfree( out_data );
         hb_retc_null();
      }
      else
         hb_retclen_buffer( out_data, uiResult );
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

/**
   Return a string decompressed with LZF
*/

HB_FUNC( LZF_DECOMPRESS )
{
   PHB_ITEM pArg = hb_param( 1, HB_IT_STRING );

   if( pArg != NULL )
   {
      const char * in_data = NULL;
      char * buffer;
      HB_SIZE in_len, buffer_size, i = 1;
      unsigned int uiResult;

      in_data = hb_itemGetCPtr( pArg );
      in_len = hb_itemGetCLen( pArg );
      buffer_size = s_buffer_size;

      buffer = hb_xgrab( buffer_size + 1 );

      do
      {
         buffer_size *= i++;
         buffer = hb_xrealloc( buffer, buffer_size );

         uiResult = lzf_decompress( in_data, in_len, buffer, buffer_size );
      } while( uiResult == 0 && errno == E2BIG );

      if( uiResult == 0 )
      {
         if( errno == EINVAL )
         {
            HB_TRACE( HB_TR_DEBUG, "LZF decompression failed, compressed data corrupted" );

            hb_storni( errno, 2 );
         }

         hb_xfree( buffer );
         hb_retc_null();
      }
      else
         hb_retclen_buffer( buffer, uiResult );
   }
   else
      hb_errRT_BASE_SubstR( EG_ARG, 3012, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}
