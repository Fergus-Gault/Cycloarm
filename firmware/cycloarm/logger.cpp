#include "cycloarm.h"
void print_string(const char *message)
{
#ifdef DEBUG
    for (uint16_t i = 0; i < strlen(message); i++)
    {
        Serial.print(message[i]);
    }
    Serial.print("\n");
#endif
}

void print_int(int num)
{
#ifdef DEBUG
    Serial.println(num);
#endif
}