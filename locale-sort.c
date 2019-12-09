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

	p(s1); p(s2); p(s3);

	if (_wsetlocale(LC_ALL, locale))
	{
	    /* int ordinary = */
	    /* 	cmp(s1, s3) < 0 && cmp(s3, s4) < 0 && cmp(s1, s4) < 0; */
	    /* verdict = ordinary ? "typical" : "unusual"; */
	    if (glocale)
		_free_locale(glocale);
	    glocale = NULL;
	    printf("%S: %d %d %d\n", locale, lt(s1, s2), lt(s2, s3), lt(s3, s1));
	    glocale = _wcreate_locale(LC_ALL, locale);
	    if (!glocale)
		puts("_wcreate_locale failed");
	    printf("%S: %d %d %d\n", locale, lt(s1, s2), lt(s2, s3), lt(s3, s1));
	}
	else
	    printf("%S: setlocale failed\n", locale);

	return TRUE;
}

int
main(int argc, char **argv)
{
	EnumSystemLocalesEx(cb, 0, (LPARAM) NULL, NULL);
	return 0;
}
