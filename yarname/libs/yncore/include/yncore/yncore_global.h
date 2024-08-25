#ifndef YNCORE_GLOBAL_H
#define YNCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(YNCORE_LIBRARY)
#  define YNCORE_EXPORT Q_DECL_EXPORT
#else
#  define YNCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // YNCORE_GLOBAL_H
