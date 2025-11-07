/*
System / Portability Check

Features:
- Detects OS at compile time (Windows / Linux / macOS / etc.)
- Detects compiler (MSVC, GCC, Clang)
- Prints notes about portability (e.g. when fopen_s is or isn’t available)
- Gives you a quick diagnostic banner at the start of your demo


*/

#include <stdio.h>

// System / Portability Check
static void checkSystemPortability(void) {
    printf("= System Portability Check =\n");

#if defined(_WIN32) || defined(_WIN64)
    printf("Operating System : Windows\n");
    printf("Note: fopen_s and printf_s are available (MSVC Safe Functions)\n");
#elif defined(__linux__)
    printf("Operating System : Linux\n");
    printf("Note: fopen_s not available — fallback to fopen instead.\n");
#elif defined(__APPLE__) && defined(__MACH__)
    printf("Operating System : macOS\n");
    printf("Note: fopen_s not available — using standard fopen.\n");
#elif defined(__unix__)
    printf("Operating System : Generic UNIX\n");
#elif defined(__posix__)
    printf("Operating System : POSIX-compliant system\n");
#else
    printf("Operating System : Unknown / Unsupported Platform\n");
#endif

    printf("Compiler: ");
#if defined(_MSC_VER)
    printf("MSVC (Microsoft Visual C++)\n");
#elif defined(__clang__)
    printf("Clang\n");
#elif defined(__GNUC__)
    printf("GCC\n");
#else
    printf("Unknown compiler\n");
#endif
    printf("\n");
}

int main(void) {
    checkSystemPortability();
    return 0;
}