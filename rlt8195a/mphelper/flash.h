#include "flash_api.h"

extern const struct _mp_obj_type_t pyb_flash_type;

// FLASH BLOCK SIZE * FLASH_NUM_BLOCKS = RESRVED USED SIZE
#define FLASH_START_BASE    (0xA9000)
#define FLASH_BLOCK_SIZE    (4096)
#define FLASH_START_BLOCK   (0)
#define FLASH_NUM_BLOCKS    (81)

void flash_init0(fs_user_mount_t *vfs);
