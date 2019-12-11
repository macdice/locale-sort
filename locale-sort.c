#include <locale.h>
#include <windows.h>
#include <stdio.h>
#include <wchar.h>
#include <winnls.h>

_locale_t glocale;

void p(const wchar_t *bp)
{
    while (*bp)
	printf("%hx ", *bp++);
    puts("");
}

char lt(wchar_t *a, wchar_t *b)
{
    int result;
    errno = 0;
    result = glocale ? _wcscoll_l(a, b, glocale) : wcscoll(a, b);
    if (errno != 0)
	puts("wcscoll(_l) failed");
    return result;
}

BOOL CALLBACK
cb(LPWSTR locale, DWORD flags, LPARAM unused)
{
	wchar_t s1[] = { 0x11a7, 0x1188, 0xd7a2, 0x0 };
	wchar_t s2[] = { 0x11a7, 0xd7a2, 0x1188, 0x0 };
	wchar_t s3[] = { 0xd7a2, 0x11a7, 0x1188, 0x0 };

//	if (_wsetlocale(LC_ALL, locale))
	{
	    glocale = _wcreate_locale(LC_ALL, locale);
	    if (!glocale)
	    {
		puts("_wcreate_locale failed");
		return TRUE;
	    }
	    /*
	    if (lt(s1, s2) != 0 ||
		lt(s2, s3) != 0 ||
		lt(s3, s1) != -1)
		*/
	      printf("%S: %d %d %d\n", locale, lt(s1, s2), lt(s2, s3), lt(s1, s3));
	    _free_locale(glocale);
	}
//	else
//	    printf("%S: setlocale failed\n", locale);

	return TRUE;
}

int
main(int argc, char **argv)
{
	EnumSystemLocalesEx(cb, 0, (LPARAM) NULL, NULL);
	cb("Korean_100_CS_AS", 0, NULL);
	return 0;
}
