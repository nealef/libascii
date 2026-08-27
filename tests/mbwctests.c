#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

// --- Test Suites ---

static void
test_wcslen(void)
{
    fprintf(stdout, "[1/14] Testing wcslen...\n");

    assert(wcslen(L"") == 0);
    assert(wcslen(L"A") == 1);
    assert(wcslen(L"Hello, World!") == 13);

    fprintf(stdout, "       -> PASSED\n");
}

static void
test_wcsstr(void)
{
    fprintf(stdout, "[2/14] Testing wcsstr...\n");

    const wchar_t *haystack = L"The quick brown fox jumps over the lazy dog";

    // Standard searches
    assert(wcsstr(haystack, L"brown") == &haystack[10]);
    assert(wcsstr(haystack, L"The") == &haystack[0]);
    assert(wcsstr(haystack, L"dog") == &haystack[40]);

    // Edge cases
    assert(wcsstr(haystack, L"cat") == NULL);
    assert(wcsstr(haystack, L"") == haystack);

    fprintf(stdout, "       -> PASSED\n");
}

static void
test_wcstod(void)
{
    fprintf(stdout, "[3/14] Testing wcstod...\n");
    wchar_t *endPtr = NULL;

    // Standard conversions
    assert(fabs(wcstod(L"123.45", &endPtr) - 123.45) < 1e-6);
    assert(*endPtr == L'\0');

    // Negative + Exponent + Leading Whitespace
    assert(fabs(wcstod(L"   -1.25e2 Rest", &endPtr) - (-125.0)) < 1e-6);
    assert(wcscmp(endPtr, L" Rest") == 0);

    // Edge cases
    assert(wcstod(NULL, &endPtr) == 0.0);
    assert(endPtr == NULL);

    wcstod(L"NoNumber", &endPtr);
    assert(wcscmp(endPtr, L"NoNumber") == 0);

    fprintf(stdout, "       -> PASSED\n");
}

static void
test_mbsinit(void)
{
    fprintf(stdout, "[4/14] Testing mbsinit...\n");

    mbstate_t state;
    memset(&state, 0, sizeof(state));

    assert(mbsinit(NULL) != 0); // NULL is always in initial state
    assert(mbsinit(&state) != 0);

    fprintf(stdout, "       -> PASSED\n");
}

static void
test_mbrtowc_and_mbtowc(void)
{
    fprintf(stdout, "[5-6/14] Testing mbrtowc and mbtowc...\n");
    wchar_t wc = 0;
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    // Basic ASCII character
    assert(mbrtowc(&wc, "A", 1, NULL) == 1);
    assert(wc == L'A');

    // Null terminator returns 0
    assert(mbrtowc(&wc, "", 1, NULL) == 0);
    assert(wc == L'\0');

    // mbtowc wrapper check
    assert(mbtowc(&wc, "B", 1) == 1);
    assert(wc == L'B');

    // Boundary check (n = 0 should return -2 for incomplete sequence)
    int rc = mbrtowc(&wc, "A", 0, NULL);
    perror("mbrtowc");
    fprintf(stdout, "%d\n",rc);
    assert(mbrtowc(&wc, "A", 0, NULL) == (size_t)-2);

    fprintf(stdout, "         -> PASSED\n");
}

static void
test_wcrtomb_and_wctomb(void)
{
    fprintf(stdout, "[7-8/14] Testing wcrtomb and wctomb...\n");
    char buf[8] = {0};
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    // Single character conversion
    assert(wcrtomb(buf, L'Z', NULL) == 1);
    assert(buf[0] == 'Z');

    // Null character
    assert(wcrtomb(buf, L'\0', NULL) == 1);
    assert(buf[0] == '\0');

    // wctomb wrapper check
    assert(wctomb(buf, L'Y') == 1);
    assert(buf[0] == 'Y');

    // NULL target buffer returns 1 (stateless check)
    assert(wcrtomb(NULL, L'A', NULL) == 1);

    fprintf(stdout, "         -> PASSED\n");
}

static void
test_mblen_and_mbrlen(void)
{
    fprintf(stdout, "[9-10/14] Testing mblen and mbrlen...\n");
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    assert(mblen("Test", 4) == 1);
    assert(mblen("", 1) == 0);
    assert(mblen(NULL, 0) == 0);

    assert(mbrlen("Test", 4, NULL) == 1);
    assert(mbrlen("", 1, NULL) == 0);

    fprintf(stdout, "          -> PASSED\n");
}

static void
test_mbstowcs_and_mbsrtowcs(void)
{
    fprintf(stdout, "[11-12/14] Testing mbstowcs and mbsrtowcs...\n");

    const char *srcStr = "Hello, C!";
    wchar_t dstBuf[32];

    // Standard string conversion
    size_t written = mbstowcs(dstBuf, srcStr, 32);
    assert(written == 9);
    assert(wcscmp(dstBuf, L"Hello, C!") == 0);

    // Buffer limit capping test
    memset(dstBuf, 0, sizeof(dstBuf));
    written = mbstowcs(dstBuf, srcStr, 5);
    assert(written == 5);
    assert(wcsncmp(dstBuf, L"Hello", 5) == 0);

    // Pointer update check for mbsrtowcs
    const char *srcPtr = srcStr;
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    written = mbsrtowcs(dstBuf, &srcPtr, 32, NULL);
    assert(written == 9);
    assert(srcPtr == NULL); // Standard requires setting *src to NULL on completion

    fprintf(stdout, "           -> PASSED\n");
}

static void
test_wcstombs_and_wcsrtombs(void)
{
    fprintf(stdout, "[13-14/14] Testing wcstombs and wcsrtombs...\n");

    const wchar_t *srcWstr = L"Wide String Test";
    char dstBuf[32];

    // Standard wide string to multibyte conversion
    size_t written = wcstombs(dstBuf, srcWstr, 32);
    assert(written == 16);
    assert(strcmp(dstBuf, "Wide String Test") == 0);

    // Buffer limit capping test
    memset(dstBuf, 0, sizeof(dstBuf));
    written = wcstombs(dstBuf, srcWstr, 4);
    assert(written == 4);
    assert(strncmp(dstBuf, "Wide", 4) == 0);

    // Pointer update check for wcsrtombs
    const wchar_t *srcPtr = srcWstr;
    mbstate_t state;
    memset(&state, 0, sizeof(state));

    written = wcsrtombs(dstBuf, &srcPtr, 32, NULL);
    assert(written == 16);
    assert(srcPtr == NULL); // Standard requires setting *src to NULL on completion

    fprintf(stdout, "           -> PASSED\n");
}

static void
test_wcstrtoul(void)
{
    wchar_t *endptr;

    // 1. Basic Decimal
    assert(wcstoul(L"12345", &endptr, 10) == 12345UL);
    assert(*endptr == L'\0');

    // 2. Hexadecimal auto-detect (base 0) and explicit base 16
    assert(wcstoul(L"0x1A3F", &endptr, 0) == 0x1A3FUL);
    assert(*endptr == L'\0');
    assert(wcstoul(L"1a3f", &endptr, 16) == 0x1A3FUL);

    // 3. Octal auto-detect
    assert(wcstoul(L"0755", &endptr, 0) == 0755UL);

    // 4. Base 36 (Alphanumeric maximum radix)
    assert(wcstoul(L"Z", &endptr, 36) == 35UL);

    // 5. Overflow handling
    errno = 0;
    assert(wcstoul(L"99999999999999999999999999999", &endptr, 10) == ULONG_MAX);
    assert(errno == ERANGE);

    // 6. Leading whitespace and sign handling
    assert(wcstoul(L"   -42 Rest", &endptr, 10) == (unsigned long)-42);
    assert(wcscmp(endptr, L" Rest") == 0);

    // 7. Invalid base check
    errno = 0;
    assert(wcstoul(L"123", &endptr, 37) == 0);
    assert(errno == EINVAL);

    fprintf(stdout, "All custom_wcstoul tests passed successfully!\n");
}

// --- Driver Main ---

int
main(void)
{
    fprintf(stdout, "====================================================\n");
    fprintf(stdout, "  Starting Custom Multibyte & Wide-Char Test Suite  \n");
    fprintf(stdout, "====================================================\n\n");

    test_wcslen();
    test_wcsstr();
    test_wcstod();
    test_mbsinit();
    test_mbrtowc_and_mbtowc();
    test_wcrtomb_and_wctomb();
    test_mblen_and_mbrlen();
    test_mbstowcs_and_mbsrtowcs();
    test_wcstombs_and_wcsrtombs();
    test_wcstrtoul();

    fprintf(stdout, "\n====================================================\n");
    fprintf(stdout, "  ALL 15 ROUTINE TESTS COMPLETED SUCCESSFULLY!      \n");
    fprintf(stdout, "====================================================\n");

    return 0;
}
