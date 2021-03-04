#include "modsdfs.h"

#define TEST_SIZE   (512)

char WRBuf[TEST_SIZE];
char RDBuf[TEST_SIZE];
int sdioInitErr = FR_OK;

static void *m_fs = NULL;
static void *m_file = NULL;
static char logical_drv[4] = "0:/";

#if 0
logical_drv[0] = '0';
logical_drv[1] = ':';
logical_drv[2] = '/';
logical_drv[3] = '\0';
#endif

static int drv_num = -1;


///////////////////////////////
//                           //
//    Internal Functions     //
//                           //
///////////////////////////////


bool init_sd_fs(void){
	FRESULT ret = FR_OK;

    do {
        m_fs = (FATFS *) malloc (sizeof(FATFS));
        if (m_fs == NULL) {
            ret = FR_INT_ERR;
            break;
        }

        if(sdioInitErr == FR_DISK_ERR)
            break;
        
        // Uncomment the line below to select between flash and SD card
        // Note that disk specific macro has to be set&clear in platform_opts.h to enable fs
        // also SD card uses 512 for _MAX_SS, and flash 4096 in ffconf.h

        drv_num = FATFS_RegisterDiskDriver(&SD_disk_Driver);  // mount on SD card
        //drv_num = FATFS_RegisterDiskDriver(&FLASH_disk_Driver);   //mount on flash
        
        if (drv_num < 0) {
            printf("Rigester disk driver to FATFS fail.\n");
            ret = FR_DISK_ERR;
            break;
        }

        logical_drv[0] += drv_num;

        ret = f_mount((FATFS *)m_fs, logical_drv, 1);
        if (ret != FR_OK){
            printf("FATFS mount logical drive fail:%d\n", ret);
            break;
        }

    } while (0);

    if (ret != FR_OK) {
        drv_num = -1;
    }

    return (-(int)ret);
}


///////////////////////////////
//                           //
//    External Functions     //
//                           //
///////////////////////////////


char* getRootPath() {
    if (drv_num < 0) {
        return NULL;
    } else {
        return logical_drv;
    }
}


char* getCWD() {

	char absolute_path[128];

    if (drv_num < 0) {
        return NULL;
    } 
    
    f_getcwd(absolute_path, 128);

    return absolute_path;
}


void open(char* fileName) {
    FRESULT ret = FR_OK;
    char absolute_path[128];

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        m_file = (FIL *)malloc (sizeof(FIL));
        if (m_file == NULL) {
            ret = FR_INT_ERR;
            break;
        }

        sprintf(absolute_path, "%s%s", getRootPath(), fileName);

        ret = f_open((FIL *)m_file, absolute_path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

        if (ret != FR_OK) {
            printf("open file (%s) fail. (ret=%d)\n", absolute_path, ret);
            break;
        }
    } while (0);

    if (ret != FR_OK) {
        if (m_file != NULL) {
            free(m_file);
            m_file = NULL;
        }
    }

    //return m_file; // TBD, here should return an object of sdfs, not pointer
}



STATIC mp_obj_t listdir(void) {
    FRESULT ret = FR_OK;
    FILINFO fno;
    DIR dir;
    unsigned int bufsize = 512;
    char result_buf[bufsize];

    char *fn;
    unsigned int fnlen;
    int bufidx = 0;

#if _USE_LFN
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_opendir(&dir, getRootPath());
        if (ret != FR_OK) {
            break;
        }

        memset(result_buf, 0, bufsize);

        while (1) {
            ret = f_readdir(&dir, &fno);
            if ((ret != FR_OK) || (fno.fname[0] == 0)) {
                break;
            }

#if _USE_LFN
            if (*fno.lfname)
            {
                fn = fno.lfname;
                fnlen = fno.lfsize;
            }
            else
#endif
            {
                fn = fno.fname;
                fnlen = strlen(fn);
            }

            if ((bufidx + fnlen + 1) < bufsize) {
                bufidx += sprintf((result_buf + bufidx), "%s", fn);
                bufidx++;
            }
        }
    } while (0);

    return mp_obj_new_str(result_buf, bufidx);
}
MP_DEFINE_CONST_FUN_OBJ_0(listdir_obj, listdir);



STATIC mp_obj_t mkdir(mp_obj_t dirName) {
    FRESULT ret = FR_OK;
    char absolute_path[128];
    
    char* dir_path = mp_obj_str_get_str(dirName);
    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        sprintf(absolute_path, "%s%s", getRootPath(), dir_path);

        ret = f_mkdir(absolute_path);
        if (ret != FR_OK) {
            break;
        }
    } while (0);

    return mp_obj_new_int(ret);
}
MP_DEFINE_CONST_FUN_OBJ_1(mkdir_obj, mkdir);



// Note: Set _FS_RPATH to 2 in ffconf.h in order to enable chdir and getcwd
STATIC mp_obj_t chdir(mp_obj_t dirName) {
    FRESULT ret = FR_OK;
    char absolute_path[128];
    
    char* dir_path = mp_obj_str_get_str(dirName);
    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        sprintf(absolute_path, "%s%s", getRootPath(), dir_path);

        ret = f_chdir(absolute_path);
        if (ret != FR_OK) {
            break;
        }
    } while (0);

    return mp_obj_new_int(ret);
}
MP_DEFINE_CONST_FUN_OBJ_1(chdir_obj, chdir);




STATIC mp_obj_t rm(mp_obj_t fileName) {
    FRESULT ret = FR_OK;
    char absolute_path[128];
    
    char* fn = mp_obj_str_get_str(fileName);
    
    sprintf(absolute_path, "%s%s", getRootPath(), fn);

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_unlink(absolute_path);
        if (ret != FR_OK) {
            break;
        }
    } while (0);

    return mp_obj_new_int(ret);
}
MP_DEFINE_CONST_FUN_OBJ_1(rm_obj, rm);




#if 0
STATIC mp_obj_t open(mp_obj_t fileName) {
    FRESULT ret = FR_OK;
    char absolute_path[128];

    char* fn = mp_obj_str_get_str(fileName);
    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        m_file = (FIL *)malloc (sizeof(FIL));
        if (m_file == NULL) {
            ret = FR_INT_ERR;
            break;
        }

        sprintf(absolute_path, "%s%s", getRootPath(), fn);

        ret = f_open((FIL *)m_file, absolute_path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

        if (ret != FR_OK) {
            printf("open file (%s) fail. (ret=%d)\n", absolute_path, ret);
            break;
        }
    } while (0);

    if (ret != FR_OK) {
        if (m_file != NULL) {
            free(m_file);
            m_file = NULL;
        }
    }

    return MP_OBJ_FROM_PTR(m_file); // TBD, here should return an object of sdfs, not pointer
}
MP_DEFINE_CONST_FUN_OBJ_1(open_obj, open);
#endif

STATIC mp_obj_t close() {
    FRESULT ret = FR_OK;

    ret = f_close((FIL *)m_file);
    free(m_file);
    m_file = NULL;

    return mp_obj_new_int(ret);
}
MP_DEFINE_CONST_FUN_OBJ_0(close_obj, close);




STATIC mp_obj_t read(mp_obj_t fileName) {
    FRESULT ret = FR_OK;
    char buf[128];
    unsigned int readsize = 0;
    unsigned char nbyte = 127;

    char* fn = mp_obj_str_get_str(fileName);
    open(fn);

    ret = f_read((FIL *)m_file, buf, nbyte, &readsize);

    if (ret != FR_OK) {
        printf("File function error. \r\n");
    } else {
        printf("%s", buf);
    }

    return mp_obj_new_int(readsize);
}
MP_DEFINE_CONST_FUN_OBJ_1(read_obj, read);




STATIC mp_obj_t write(mp_obj_t fileName, mp_obj_t buf_in) {
    FRESULT ret = FR_OK;
    unsigned int writesize = 0;

    char* fn = mp_obj_str_get_str(fileName);
    open(fn);

#if 0
    //Prepare the buffer to write from
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf_in, &bufinfo, MP_BUFFER_READ);

    ret = f_write((FIL *)m_file, (const void *)bufinfo.buf, bufinfo.len, &writesize);
#endif

    char* buf = mp_obj_str_get_str(buf_in);

    printf("Recv: %s, commencing f write\n", buf);

    ret = f_write((FIL *)m_file, (const void *)buf, sizeof(buf), &writesize);

    printf("test f write finish\n");
    if (ret != FR_OK) {
        printf("File function error. \r\n");
    }

    //return writesize;
}
MP_DEFINE_CONST_FUN_OBJ_2(write_obj, write);



/////////////////////////////////////////////////
//                                             //
//            MicroPython API                  //
//                                             //
/////////////////////////////////////////////////


STATIC const mp_map_elem_t sdfs_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR___name__),      MP_OBJ_NEW_QSTR(MP_QSTR_fs)   },
    { MP_OBJ_NEW_QSTR(MP_QSTR_listdir),       MP_OBJ_FROM_PTR(&listdir_obj) },
    { MP_OBJ_NEW_QSTR(MP_QSTR_mkdir),         MP_OBJ_FROM_PTR(&mkdir_obj)   },
    { MP_OBJ_NEW_QSTR(MP_QSTR_chdir),         MP_OBJ_FROM_PTR(&chdir_obj)   },
    { MP_OBJ_NEW_QSTR(MP_QSTR_rm),            MP_OBJ_FROM_PTR(&rm_obj)      },
    //{ MP_OBJ_NEW_QSTR(MP_QSTR_open),          MP_OBJ_FROM_PTR(&open_obj)    },
    { MP_OBJ_NEW_QSTR(MP_QSTR_close),         MP_OBJ_FROM_PTR(&close_obj)   },
    { MP_OBJ_NEW_QSTR(MP_QSTR_read),          MP_OBJ_FROM_PTR(&read_obj)    },
    { MP_OBJ_NEW_QSTR(MP_QSTR_write),         MP_OBJ_FROM_PTR(&write_obj)   },
};
STATIC MP_DEFINE_CONST_DICT(sdfs_module_globals, sdfs_module_globals_table);

const mp_obj_module_t mp_module_sdfs = {
    .base    = { &mp_type_module },
    .globals = (mp_obj_dict_t*)&sdfs_module_globals,
};
