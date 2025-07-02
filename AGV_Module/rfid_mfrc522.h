#ifndef RFID_MFRC522_H
#define RFID_MFRC522_H

#include <stdbool.h>
#include <stdint.h>

void rfid_init(void);
bool rfid_check_tag_present(void);

#endif // RFID_MFRC522_H
