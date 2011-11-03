/****************************************************************************
** FenetrePlanVille meta object code from reading C++ file 'FenetrePlanVille.h'
**
** Created: Fri Oct 14 12:32:19 2011
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.4   edited Jan 21 18:14 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "FenetrePlanVille.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FenetrePlanVille::className() const
{
    return "FenetrePlanVille";
}

QMetaObject *FenetrePlanVille::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FenetrePlanVille( "FenetrePlanVille", &FenetrePlanVille::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FenetrePlanVille::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FenetrePlanVille", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FenetrePlanVille::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FenetrePlanVille", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FenetrePlanVille::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"Terminer", 0, 0 };
    static const QUMethod slot_1 = {"Selection", 0, 0 };
    static const QUMethod slot_2 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Terminer()", &slot_0, QMetaData::Public },
	{ "Selection()", &slot_1, QMetaData::Public },
	{ "languageChange()", &slot_2, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"FenetrePlanVille", parentObject,
	slot_tbl, 3,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FenetrePlanVille.setMetaObject( metaObj );
    return metaObj;
}

void* FenetrePlanVille::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FenetrePlanVille" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool FenetrePlanVille::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Terminer(); break;
    case 1: Selection(); break;
    case 2: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool FenetrePlanVille::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool FenetrePlanVille::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool FenetrePlanVille::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
