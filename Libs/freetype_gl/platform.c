#include <string.h>
#include "platform.h"

#if defined(_WIN32) || defined(_WIN64)

#include <math.h>

#if !defined(_MSC_VER) || _MSC_VER < 1800
double round (double v)
{
	return (v > 0.0) ? floor(v + 0.5) : ceil(v - 0.5);
}
#endif // _MSC_VER < 1800

// strndup() is not available on Windows
char *strndup( const char *s1, size_t n)
{
	char *copy= (char*)malloc( n+1 );
	memcpy( copy, s1, n );
	copy[n] = 0;
	return copy;
};
#endif


// strndup() was only added in OSX lion
#if defined(__APPLE__)
char *strndup( const char *s1, size_t n)
{
    char *copy = calloc( n+1, sizeof(char) );
    memcpy( copy, s1, n );
    return copy;
};
#endif
