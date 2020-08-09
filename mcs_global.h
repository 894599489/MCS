#ifndef MCS_GLOBAL_H
#define MCS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(MCS_LIBRARY)
#  define MCSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define MCSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // MCS_GLOBAL_H
