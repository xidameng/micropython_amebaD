#include "flash_api.h"

extern const struct _mp_obj_type_t pyb_flash_type;

// FLASH BLOCK SIZE * FLASH_NUM_BLOCKS = RESRVED USED SIZE
#define FLASH_START_BASE    (0xB5000)
#define FLASH_BLOCK_SIZE    (512)
#define FLASH_START_BLOCK   (0)
#define FLASH_NUM_BLOCKS    (600)

void flash_init0(fs_user_mount_t *vfs);
