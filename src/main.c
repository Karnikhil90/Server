#include <stdio.h>
#include "sd_card_module.h"

#define MOUNT_POINT "/sdcard"

void app_main(void) {
    // Initialize SD card

    // Mount SD card
    if (!mount_sd_card()) {
        printf("SD card mounting failed.\n");
        return;
    }
      
    // Get and print SD card volume information
    printf("Volume Name: %s\n", get_volume_name());
    printf("Volume Size: %llu MB\n", get_volume_size("MB"));
    printf("Volume Speed: %.2f KB/s\n", get_volume_speed());

    // Write data to a file
    const char *file_path = MOUNT_POINT "/test.txt";
    if (write_to_file(file_path, "Hello from nikhil esp32 first sd card read test I hope it working its best... \nHello, its a SDcard ", false)) {
        printf("File written successfully.\n");
    } else {
        printf("Failed to write to file.\n");
    }

    // Set up metadata and write it
    struct meta_data file_meta = {
        .create_date = "2024-10-31",
        .create_time = "14:30",
        .last_change_date = "2024-10-31",
        .last_change_time = "14:30",
        .description = "Test file for SD card module"
    };
    if (write_meta(file_path, file_meta)) {
        printf("Metadata written successfully.\n");
    } else {
        printf("Failed to write metadata.\n");
    }

    // Read the file content
    char *content = read_file(file_path);
    if (content != NULL) {
        printf("File Content: %s\n", content);
        free(content);  // Free the allocated memory for file content
    } else {
        printf("Failed to read file.\n");
    }

    // Unmount the SD card when done
    unmount_sd_card();
}
