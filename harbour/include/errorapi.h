/*
 * $Id$
 */

/*
   Harbour Project source code

   This file contains the Harbour internal error handling definitions

   Copyright (C) 1999  Antonio Linares
   www - http://www.harbour-project.org

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   V 1.11   Victor Szel                 #include <x> changed to #include "x".
   V 1.0    ?                           Initial posting.
*/

#ifndef HB_ERRORAPI_H_
#define HB_ERRORAPI_H_

#include "extend.h"
#include "error.ch"

/* Error codes (returned from hb_errLaunch()) */

#define E_BREAK                         0xFFFF
#define E_RETRY                         1
#define E_DEFAULT                       0

/* Error flags */

#define EF_NONE                         0
#define EF_CANRETRY                     1
#define EF_CANSUBSTITUTE                2
#define EF_CANDEFAULT                   4

/* oError:Severity */

/* ... defined in extend.ch */

/* oError:SubSystem (commonly used) */

#define HB_ERR_SS_BASE                  "BASE"
#define HB_ERR_SS_TERMINAL              "TERM"
#define HB_ERR_SS_DBCMD                 "DBCMD"
#define HB_ERR_SS_TOOLS                 "TOOLS"

/* oError:GenCode */

/* ... defined in extend.ch */

/* Internal error numbers */

#define HB_ERR_IE_NOT_ENOUGH_MEM        1024
#define HB_ERR_IE_ERR_RECOV_FAIL        1025
#define HB_ERR_IE_UNREC_ERROR           1026
#define HB_ERR_IE_GENERIC               1027

/* Standard API */

extern char *   hb_errGetDescription    ( PHB_ITEM pError );
extern char *   hb_errGetFileName       ( PHB_ITEM pError );
extern USHORT   hb_errGetFlags          ( PHB_ITEM pError );
extern USHORT   hb_errGetGenCode        ( PHB_ITEM pError );
extern char *   hb_errGetOperation      ( PHB_ITEM pError );
extern USHORT   hb_errGetOsCode         ( PHB_ITEM pError );
extern USHORT   hb_errGetSeverity       ( PHB_ITEM pError );
extern USHORT   hb_errGetSubCode        ( PHB_ITEM pError );
extern char *   hb_errGetSubSystem      ( PHB_ITEM pError );
extern USHORT   hb_errGetTries          ( PHB_ITEM pError );
extern WORD     hb_errLaunch            ( PHB_ITEM pError );
extern PHB_ITEM hb_errNew               ( void );
extern PHB_ITEM hb_errPutDescription    ( PHB_ITEM pError, char * szDescription );
extern PHB_ITEM hb_errPutFileName       ( PHB_ITEM pError, char * szFileName );
extern PHB_ITEM hb_errPutFlags          ( PHB_ITEM pError, USHORT uiFlags );
extern PHB_ITEM hb_errPutGenCode        ( PHB_ITEM pError, USHORT uiGenCode );
extern PHB_ITEM hb_errPutOperation      ( PHB_ITEM pError, char * szOperation );
extern PHB_ITEM hb_errPutOsCode         ( PHB_ITEM pError, USHORT uiOsCode );
extern PHB_ITEM hb_errPutSeverity       ( PHB_ITEM pError, USHORT uiSeverity );
extern PHB_ITEM hb_errPutSubCode        ( PHB_ITEM pError, USHORT uiSubCode );
extern PHB_ITEM hb_errPutSubSystem      ( PHB_ITEM pError, char * szSubSystem );
extern PHB_ITEM hb_errPutTries          ( PHB_ITEM pError, USHORT uiTries );
extern void     hb_errRelease           ( PHB_ITEM pError );

/* Error launchers */

extern void     hb_errorInternal        ( ULONG ulIntCode, char * szText, char * szModul, WORD wLine, char * szPar1, char * szPar2, char * szPar3 );

extern void     hb_errorRT_BASE         ( ULONG ulGenCode, ULONG ulSubCode, char * szDescription, char * szOperation );
extern WORD     hb_errorRT_BASE_Ext1    ( ULONG ulGenCode, ULONG ulSubCode, char * szDescription, char * szOperation, USHORT uiOsCode, USHORT uiFlags );
extern void     hb_errorRT_TERMINAL     ( ULONG ulGenCode, ULONG ulSubCode, char * szDescription, char * szOperation );
extern void     hb_errorRT_DBCMD        ( ULONG ulGenCode, ULONG ulSubCode, char * szDescription, char * szOperation );
extern void     hb_errorRT_TOOLS        ( ULONG ulGenCode, ULONG ulSubCode, char * szDescription, char * szOperation );

extern char *   hb_errorNatDescription  ( ULONG ulGenCode ); /* Reads error description in national language */
extern char *   hb_errorNatInternal     ( ULONG ulIntCode ); /* Reads internal error description in national language */

#endif /* HB_ERRORAPI_H_ */
