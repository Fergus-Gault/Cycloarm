#include "cycloarm.h"
void print_debug(const char *message)
{
#ifdef DEBUG
    for (uint16_t i = 0; i < strlen(message); i++)
    {
        Serial.print(message[i]);
    }
    Serial.print("\n");
#endif
}