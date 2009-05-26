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

#include "hbapi.h"
#include "hbqt.h"

/*----------------------------------------------------------------------*/
#if QT_VERSION >= 0x040500
/*----------------------------------------------------------------------*/


#include <QtGui/QStyle>


/*
 * QStyle ()
 * virtual ~QStyle ()
 */
HB_FUNC( QT_QSTYLE )
{

}

/*
 * int combinedLayoutSpacing ( QSizePolicy::ControlTypes controls1, QSizePolicy::ControlTypes controls2, Qt::Orientation orientation, QStyleOption * option = 0, QWidget * widget = 0 ) const
 */
HB_FUNC( QT_QSTYLE_COMBINEDLAYOUTSPACING )
{
   hb_retni( hbqt_par_QStyle( 1 )->combinedLayoutSpacing( ( QSizePolicy::ControlTypes ) hb_parni( 2 ), ( QSizePolicy::ControlTypes ) hb_parni( 3 ), ( Qt::Orientation ) hb_parni( 4 ), hbqt_par_QStyleOption( 5 ), hbqt_par_QWidget( 6 ) ) );
}

/*
 * virtual void drawComplexControl ( ComplexControl control, const QStyleOptionComplex * option, QPainter * painter, const QWidget * widget = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_DRAWCOMPLEXCONTROL )
{
   hbqt_par_QStyle( 1 )->drawComplexControl( ( QStyle::ComplexControl ) hb_parni( 2 ), hbqt_par_QStyleOptionComplex( 3 ), hbqt_par_QPainter( 4 ), hbqt_par_QWidget( 5 ) );
}

/*
 * virtual void drawControl ( ControlElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_DRAWCONTROL )
{
   hbqt_par_QStyle( 1 )->drawControl( ( QStyle::ControlElement ) hb_parni( 2 ), hbqt_par_QStyleOption( 3 ), hbqt_par_QPainter( 4 ), hbqt_par_QWidget( 5 ) );
}

/*
 * virtual void drawItemPixmap ( QPainter * painter, const QRect & rectangle, int alignment, const QPixmap & pixmap ) const
 */
HB_FUNC( QT_QSTYLE_DRAWITEMPIXMAP )
{
   hbqt_par_QStyle( 1 )->drawItemPixmap( hbqt_par_QPainter( 2 ), *hbqt_par_QRect( 3 ), hb_parni( 4 ), *hbqt_par_QPixmap( 5 ) );
}

/*
 * virtual void drawItemText ( QPainter * painter, const QRect & rectangle, int alignment, const QPalette & palette, bool enabled, const QString & text, QPalette::ColorRole textRole = QPalette::NoRole ) const
 */
HB_FUNC( QT_QSTYLE_DRAWITEMTEXT )
{
   hbqt_par_QStyle( 1 )->drawItemText( hbqt_par_QPainter( 2 ), *hbqt_par_QRect( 3 ), hb_parni( 4 ), *hbqt_par_QPalette( 5 ), hb_parl( 6 ), hbqt_par_QString( 7 ), ( HB_ISNUM( 8 ) ? ( QPalette::ColorRole ) hb_parni( 8 ) : ( QPalette::ColorRole ) QPalette::NoRole ) );
}

/*
 * virtual void drawPrimitive ( PrimitiveElement element, const QStyleOption * option, QPainter * painter, const QWidget * widget = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_DRAWPRIMITIVE )
{
   hbqt_par_QStyle( 1 )->drawPrimitive( ( QStyle::PrimitiveElement ) hb_parni( 2 ), hbqt_par_QStyleOption( 3 ), hbqt_par_QPainter( 4 ), hbqt_par_QWidget( 5 ) );
}

/*
 * virtual QPixmap generatedIconPixmap ( QIcon::Mode iconMode, const QPixmap & pixmap, const QStyleOption * option ) const = 0
 */
HB_FUNC( QT_QSTYLE_GENERATEDICONPIXMAP )
{
   hb_retptr( new QPixmap( hbqt_par_QStyle( 1 )->generatedIconPixmap( ( QIcon::Mode ) hb_parni( 2 ), *hbqt_par_QPixmap( 3 ), hbqt_par_QStyleOption( 4 ) ) ) );
}

/*
 * virtual SubControl hitTestComplexControl ( ComplexControl control, const QStyleOptionComplex * option, const QPoint & position, const QWidget * widget = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_HITTESTCOMPLEXCONTROL )
{
   hb_retni( ( QStyle::SubControl ) hbqt_par_QStyle( 1 )->hitTestComplexControl( ( QStyle::ComplexControl ) hb_parni( 2 ), hbqt_par_QStyleOptionComplex( 3 ), *hbqt_par_QPoint( 4 ), hbqt_par_QWidget( 5 ) ) );
}

/*
 * virtual QRect itemPixmapRect ( const QRect & rectangle, int alignment, const QPixmap & pixmap ) const
 */
HB_FUNC( QT_QSTYLE_ITEMPIXMAPRECT )
{
   hb_retptr( new QRect( hbqt_par_QStyle( 1 )->itemPixmapRect( *hbqt_par_QRect( 2 ), hb_parni( 3 ), *hbqt_par_QPixmap( 4 ) ) ) );
}

/*
 * virtual QRect itemTextRect ( const QFontMetrics & metrics, const QRect & rectangle, int alignment, bool enabled, const QString & text ) const
 */
HB_FUNC( QT_QSTYLE_ITEMTEXTRECT )
{
   hb_retptr( new QRect( hbqt_par_QStyle( 1 )->itemTextRect( *hbqt_par_QFontMetrics( 2 ), *hbqt_par_QRect( 3 ), hb_parni( 4 ), hb_parl( 5 ), hbqt_par_QString( 6 ) ) ) );
}

/*
 * int layoutSpacing ( QSizePolicy::ControlType control1, QSizePolicy::ControlType control2, Qt::Orientation orientation, const QStyleOption * option = 0, const QWidget * widget = 0 ) const
 */
HB_FUNC( QT_QSTYLE_LAYOUTSPACING )
{
   hb_retni( hbqt_par_QStyle( 1 )->layoutSpacing( ( QSizePolicy::ControlType ) hb_parni( 2 ), ( QSizePolicy::ControlType ) hb_parni( 3 ), ( Qt::Orientation ) hb_parni( 4 ), hbqt_par_QStyleOption( 5 ), hbqt_par_QWidget( 6 ) ) );
}

/*
 * virtual int pixelMetric ( PixelMetric metric, const QStyleOption * option = 0, const QWidget * widget = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_PIXELMETRIC )
{
   hb_retni( hbqt_par_QStyle( 1 )->pixelMetric( ( QStyle::PixelMetric ) hb_parni( 2 ), hbqt_par_QStyleOption( 3 ), hbqt_par_QWidget( 4 ) ) );
}

/*
 * virtual void polish ( QWidget * widget )
 */
HB_FUNC( QT_QSTYLE_POLISH )
{
   hbqt_par_QStyle( 1 )->polish( hbqt_par_QWidget( 2 ) );
}

/*
 * virtual void polish ( QApplication * application )
 */
HB_FUNC( QT_QSTYLE_POLISH_1 )
{
   hbqt_par_QStyle( 1 )->polish( hbqt_par_QApplication( 2 ) );
}

/*
 * virtual void polish ( QPalette & palette )
 */
HB_FUNC( QT_QSTYLE_POLISH_2 )
{
   hbqt_par_QStyle( 1 )->polish( *hbqt_par_QPalette( 2 ) );
}

/*
 * virtual QSize sizeFromContents ( ContentsType type, const QStyleOption * option, const QSize & contentsSize, const QWidget * widget = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_SIZEFROMCONTENTS )
{
   hb_retptr( new QSize( hbqt_par_QStyle( 1 )->sizeFromContents( ( QStyle::ContentsType ) hb_parni( 2 ), hbqt_par_QStyleOption( 3 ), *hbqt_par_QSize( 4 ), hbqt_par_QWidget( 5 ) ) ) );
}

/*
 * QIcon standardIcon ( StandardPixmap standardIcon, const QStyleOption * option = 0, const QWidget * widget = 0 ) const
 */
HB_FUNC( QT_QSTYLE_STANDARDICON )
{
   hb_retptr( new QIcon( hbqt_par_QStyle( 1 )->standardIcon( ( QStyle::StandardPixmap ) hb_parni( 2 ), hbqt_par_QStyleOption( 3 ), hbqt_par_QWidget( 4 ) ) ) );
}

/*
 * virtual QPalette standardPalette () const
 */
HB_FUNC( QT_QSTYLE_STANDARDPALETTE )
{
   hb_retptr( new QPalette( hbqt_par_QStyle( 1 )->standardPalette() ) );
}

/*
 * virtual int styleHint ( StyleHint hint, const QStyleOption * option = 0, const QWidget * widget = 0, QStyleHintReturn * returnData = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_STYLEHINT )
{
   hb_retni( hbqt_par_QStyle( 1 )->styleHint( ( QStyle::StyleHint ) hb_parni( 2 ), hbqt_par_QStyleOption( 3 ), hbqt_par_QWidget( 4 ), hbqt_par_QStyleHintReturn( 5 ) ) );
}

/*
 * virtual QRect subControlRect ( ComplexControl control, const QStyleOptionComplex * option, SubControl subControl, const QWidget * widget = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_SUBCONTROLRECT )
{
   hb_retptr( new QRect( hbqt_par_QStyle( 1 )->subControlRect( ( QStyle::ComplexControl ) hb_parni( 2 ), hbqt_par_QStyleOptionComplex( 3 ), ( QStyle::SubControl ) hb_parni( 4 ), hbqt_par_QWidget( 5 ) ) ) );
}

/*
 * virtual QRect subElementRect ( SubElement element, const QStyleOption * option, const QWidget * widget = 0 ) const = 0
 */
HB_FUNC( QT_QSTYLE_SUBELEMENTRECT )
{
   hb_retptr( new QRect( hbqt_par_QStyle( 1 )->subElementRect( ( QStyle::SubElement ) hb_parni( 2 ), hbqt_par_QStyleOption( 3 ), hbqt_par_QWidget( 4 ) ) ) );
}

/*
 * virtual void unpolish ( QWidget * widget )
 */
HB_FUNC( QT_QSTYLE_UNPOLISH )
{
   hbqt_par_QStyle( 1 )->unpolish( hbqt_par_QWidget( 2 ) );
}

/*
 * virtual void unpolish ( QApplication * application )
 */
HB_FUNC( QT_QSTYLE_UNPOLISH_1 )
{
   hbqt_par_QStyle( 1 )->unpolish( hbqt_par_QApplication( 2 ) );
}


/*----------------------------------------------------------------------*/
#endif             /* #if QT_VERSION >= 0x040500 */
/*----------------------------------------------------------------------*/

