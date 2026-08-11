#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

// --- Test Suite Execution ---

static void
runMblenTests(void)
{
    printf("Running mblen tests...\n");

    // Test 1: State query (should return 0 for stateless encodings)
    assert(mblen(NULL, 0) == 0);

    // Test 2: Null terminator
    assert(mblen("", 1) == 0);

    // Test 3: Standard ASCII character
    assert(mblen("A", 1) == 1);

    // Test 4: Extended ASCII character (e.g., 0xE9 for 'é')
    const char extendedChar[] = { (char)0xE9, '\0' };
    assert(mblen(extendedChar, 1) == 1);

    // Test 5: Error handling when maxBytesRead is 0
    errno = 0;
    assert(mblen("A", 0) == -1);
    assert(errno == EILSEQ);

    printf("  -> mblen tests passed!\n");
}

static void
runMbtowcTests(void) {
    printf("Running mbtowc tests...\n");
    wchar_t destinationWc = 0;

    // Test 1: State query
    assert(mbtowc(NULL, NULL, 0) == 0);

    // Test 2: Null terminator
    destinationWc = L'X';
    assert(mbtowc(&destinationWc, "", 1) == 0);
    assert(destinationWc == L'\0');

    // Test 3: Standard ASCII conversion validation
    destinationWc = 0;
    assert(mbtowc(&destinationWc, "B", 1) == 1);
    assert(destinationWc == L'B');

    // Test 4: Extended ASCII character check
    const char extendedChar[] = { (char)0xA4, '\0' }; // Currency sign ¤
    destinationWc = 0;
    assert(mbtowc(&destinationWc, extendedChar, 1) == 1);
    assert(destinationWc == (wchar_t)0x00A4);

    // Test 5: Target pointer is NULL (should still return character length)
    assert(mbtowc(NULL, "C", 1) == 1);

    printf("  -> mbtowc tests passed!\n");
}

static void
runMbstowcsTests(void)
{
    printf("Running mbstowcs tests...\n");
    wchar_t wideBuffer[16];
    const char extendedString[] = { 'C', 'a', 'f', (char)0xa9, '\0' }; // Café

    // Test 1: Query mode (destination buffer is NULL)
    size_t requiredLength = mbstowcs(NULL, extendedString, 0);
    assert(requiredLength == 4);

    // Test 2: Standard complete array conversion
    memset(wideBuffer, 0, sizeof(wideBuffer));
    size_t resultCount = mbstowcs(wideBuffer, extendedString, 16);
    assert(resultCount == 4);
    assert(wcscmp(wideBuffer, L"Café") == 0);

    // Test 3: Buffer limit capping (destination array bounds constraint)
    memset(wideBuffer, 0, sizeof(wideBuffer));
    resultCount = mbstowcs(wideBuffer, extendedString, 2);
    assert(resultCount == 2);
    assert(wideBuffer[0] == L'C' && wideBuffer[1] == L'a');
    assert(wideBuffer[2] == 0); // Ensure it didn't write outside constraints

    printf("  -> mbtowcs tests passed!\n");
}

static void
runWctombTests(void)
{
    printf("Running wctomb tests...\n");
    char outputBuffer[4];

    // Test 1: State query
    assert(wctomb(NULL, L'\0') == 0);

    // Test 2: Null wide character conversion
    outputBuffer[0] = 'X';
    assert(wctomb(outputBuffer, L'\0') == 0);
    assert(outputBuffer[0] == '\0');

    // Test 3: Regular ASCII conversion mapping
    assert(wctomb(outputBuffer, L'Z') == 1);
    assert(outputBuffer[0] == 'Z');

    // Test 4: Extended ASCII boundary verification
    assert(wctomb(outputBuffer, (wchar_t)0x00FF) == 1);
    assert((unsigned char)outputBuffer[0] == 0xFF);

    // Test 5: Out of bounds invalid sequence protection (> 0xFF)
    errno = 0;
    assert(wctomb(outputBuffer, (wchar_t)0x0100) == -1);
    assert(errno == EILSEQ);

    printf("  -> wctomb tests passed!\n");
}

static void
runWcstombsTests(void)
{
    printf("Running wcstombs tests...\n");
    char byteBuffer[16];
    const wchar_t validWideStr[] = L"Test\x00B1"; // Test±

    // Test 1: Length calculation query mode
    size_t requiredBytes = wcstombs(NULL, validWideStr, 0);
    assert(requiredBytes == 5);

    // Test 2: Successful block string copy translation
    memset(byteBuffer, 0, sizeof(byteBuffer));
    size_t writtenBytes = wcstombs(byteBuffer, validWideStr, 16);
    assert(writtenBytes == 5);
    assert(byteBuffer[0] == 'T' && (unsigned char)byteBuffer[4] == 0xB1);

    // Test 3: Buffer size clamping checks
    memset(byteBuffer, 0, sizeof(byteBuffer));
    writtenBytes = wcstombs(byteBuffer, validWideStr, 3);
    assert(writtenBytes == 3);
    assert(strncmp(byteBuffer, "Tes", 3) == 0);

    // Test 4: Encountering an illegal runtime conversion character
    const wchar_t invalidWideStr[] = { L'A', (wchar_t)0x20AC, L'\0' }; // Euro mark (U+20AC) out of bounds
    errno = 0;
    assert(wcstombs(byteBuffer, invalidWideStr, 16) == (size_t)-1);
    assert(errno == EILSEQ);

    printf("  -> wcstombs tests passed!\n");
}

int main(void) {
    printf("=== Starting Multibyte and Wide Character Conversion Test Suite ===\n\n");
    
    runMblenTests();
    runMbtowcTests();
    runMbstowcsTests();
    runWctombTests();
    runWcstombsTests();

    printf("\n=== All Test Framework Cases Executed Successfully! ===\n");
    return 0;
}
