

#include "ace/config-win32.h"

#if COMPILER == COMPILER_MICROSOFT && _MSC_VER >= 1910 && _HAS_CXX17
#define ACE_LACKS_AUTO_PTR
#define ACE_LACKS_IF_NAMETOINDEX
#endif