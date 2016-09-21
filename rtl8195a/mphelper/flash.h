#include "flash_api.h"

extern const struct _mp_obj_type_t pyb_flash_type;
extern uint32_t __fatfs_start_address__;
extern uint32_t __fatfs_end_address__;
extern uint32_t __fatfs_num_blocks__;

// FLASH BLOCK SIZE * FLASH_NUM_BLOCKS = RESRVED USED SIZE
#define FLASH_START_BASE    (&__fatfs_start_address__)
#define FLASH_BLOCK_SIZE    (4096)
#define FLASH_START_BLOCK   (0)
#define FLASH_NUM_BLOCKS    (&__fatfs_num_blocks__)

void flash_init0(fs_user_mount_t *vfs);
