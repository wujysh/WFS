#include <cstdlib>

void clearScreen(void) {
#ifdef __linux__
    system("clear");
#else
    system("cls");
#endif
}
