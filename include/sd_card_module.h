#ifndef SD_CARD_MODULE_H
#define SD_CARD_MODULE_H

#include <stdbool.h>
#include <stdint.h>

// Metadata structure
struct meta_data {
    char create_date[11];        // e.g., "YYYY-MM-DD"
    char create_time[6];         // e.g., "HH:MM"
    char last_change_date[11];
    char last_change_time[6];
    char description[50];        // File description
};

// SD card mounting and unmounting functions
bool mount_sd_card(void);
void unmount_sd_card(void);

// SD card information retrieval
char* get_volume_name(void);
uint64_t get_volume_size(const char *unit);
float get_volume_speed(void);    // Speed in KB/s

// File operations
bool write_to_file(const char *path, const char *data, bool append);  // Renamed function
bool write_meta(const char *path, struct meta_data meta);
char* read_file(const char *path);

#endif
