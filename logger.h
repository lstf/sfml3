#ifndef _LOGGER_H
#define _LOGGER_H

#include <iomanip>
#include <iostream>
#include <string>

#include <string.h>

using namespace std;

////////////////////////////////////////////////
//
// Debugging on
//
////////////////////////////////////////////////
#ifdef DEBUG_GAME

//prefix
#define _LOG_M_PRE_F_BASE (strrchr("/" __FILE__, '/') + 1)

#define _LOG_M_PRE_F \
string(_LOG_M_PRE_F_BASE).substr(0, string(_LOG_M_PRE_F_BASE).length() - 4)

#define _LOG_M_PRE_F_W 12

#define _LOG_M_PRE \
"[ " << left << setw(_LOG_M_PRE_F_W) << (\
_LOG_M_PRE_F.length() > _LOG_M_PRE_F_W ?\
(_LOG_M_PRE_F.substr(0, _LOG_M_PRE_F_W - 1) + ".") :\
_LOG_M_PRE_F\
)

//suffix
#define _LOG_M_SUF_F string(__func__)

#define _LOG_M_SUF_F_W 12

#define _LOG_M_SUF(msg)\
right << setw(_LOG_M_SUF_F_W) << (\
_LOG_M_SUF_F.length() > _LOG_M_SUF_F_W ?\
(_LOG_M_SUF_F.substr(0, _LOG_M_SUF_F_W - 1) + ".") :\
_LOG_M_SUF_F\
) << " ]" << ": " << msg << endl

//log macros
#define log_dbg(msg) (cout << _LOG_M_PRE << " " << _LOG_M_SUF(msg))

#define log_err(msg) (cout << "[ ERR ]" << _LOG_M_PRE << " " << _LOG_M_SUF(msg))

#define log_tmp(msg) (cout << "[ TMP ]" << _LOG_M_PRE << " " << _LOG_M_SUF(msg))


////////////////////////////////////////////////
//
// Debugging off
//
////////////////////////////////////////////////
#else
#define log_dbg(msg) ((void)0)

#define log_err(msg) ((void)0)

#define log_tmp(msg) ((void)0)

#endif

#endif
