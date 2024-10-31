#include "SD_CARD_MODULE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

static const char *TAG = "SD_CARD_MODULE";
static sdmmc_card_t *card = NULL;  // Initialize to NULL

// SD card mounting function
bool mount_sd_card(void) {
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5
    };

    sdmmc_host_t host = SDMMC_HOST_DEFAULT();
    sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
    host.max_freq_khz = SDMMC_FREQ_DEFAULT;

    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount filesystem. Error: %s", esp_err_to_name(ret));
        return false;
    }
    ESP_LOGI(TAG, "SD card mounted successfully.");
    return true;
}

// SD card unmounting function
void unmount_sd_card(void) {
    if (card != NULL) {
        esp_vfs_fat_sdcard_unmount("/sdcard", card);
        ESP_LOGI(TAG, "SD card unmounted successfully.");
        card = NULL;  // Reset card pointer after unmounting
    } else {
        ESP_LOGW(TAG, "SD card is not mounted.");
    }
}

// Retrieve the SD card's volume name
char* get_volume_name(void) {
    if (card == NULL) {
        ESP_LOGE(TAG, "SD card is not mounted.");
        return NULL;
    }
    return (char *)card->cid.name;
}

// Retrieve the SD card's total size in the specified unit
uint64_t get_volume_size(const char *unit) {
    if (card == NULL) {
        ESP_LOGE(TAG, "SD card is not mounted.");
        return 0;
    }
    uint64_t size = (uint64_t)card->csd.capacity * card->csd.sector_size;
    if (strcmp(unit, "MB") == 0) {
        return size / (1024 * 1024);
    } else if (strcmp(unit, "GB") == 0) {
        return size / (1024 * 1024 * 1024);
    }
    return size; // Return in bytes if no unit specified
}

// Retrieve the SD card's speed in KB/s
float get_volume_speed(void) {
    if (card == NULL) {
        ESP_LOGE(TAG, "SD card is not mounted.");
        return 0.0;
    }
    return (float)card->csd.tr_speed / 1000.0;
}

// Write data to a file on the SD card
bool write_to_file(const char *path, const char *data, bool append) {
    const char *mode = append ? "a" : "w";
    FILE *file = fopen(path, mode);
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing.");
        return false;
    }

    if (fprintf(file, "%s", data) < 0) {
        ESP_LOGE(TAG, "Failed to write data to file.");
        fclose(file);
        return false;
    }

    fclose(file);
    ESP_LOGI(TAG, "Data written successfully.");
    return true;
}

// Write metadata to a file on the SD card
bool write_meta(const char *path, struct meta_data meta) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing metadata.");
        return false;
    }

    fprintf(file, "Create Date: %s\n", meta.create_date);
    fprintf(file, "Create Time: %s\n", meta.create_time);
    fprintf(file, "Last Change Date: %s\n", meta.last_change_date);
    fprintf(file, "Last Change Time: %s\n", meta.last_change_time);
    fprintf(file, "Description: %s\n", meta.description);

    fclose(file);
    ESP_LOGI(TAG, "Metadata written successfully.");
    return true;
}

// Read a file from the SD card and return its content as a string
char* read_file(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *buffer = (char *)malloc(file_size + 1);
    if (buffer == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for file content.");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    fclose(file);
    ESP_LOGI(TAG, "File read successfully.");
    return buffer;
}
