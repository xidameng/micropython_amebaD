// options to control how Micro Python is built
#define MICROPY_ALLOC_GC_STACK_SIZE             (32)
#define MICROPY_QSTR_BYTES_IN_HASH              (1)
#define MICROPY_ALLOC_PATH_MAX                  (128)
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_COMP_MODULE_CONST               (0)
#define MICROPY_COMP_CONST                      (0)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN        (1)
#define MICROPY_COMP_TRIPLE_TUPLE_ASSIGN        (0)
#define MICROPY_CPYTHON_COMPAT                  (1)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_PY_MICROPYTHON_MEM_INFO         (MICROPY_ENABLE_GC)
#define MICROPY_ENABLE_COMPILER                 (1)
#define MICROPY_ENABLE_FINALISER                (1)
#define MICROPY_HELPER_REPL                     (1)
#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_ENABLE_DOC_STRING               (1)
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_NORMAL)
#define MICROPY_REPL_AUTO_INDENT                (1)
#define MICROPY_PY_BUILTINS_BYTEARRAY           (1)
#define MICROPY_PY_BUILTINS_MEMORYVIEW          (1)
#define MICROPY_PY_BUILTINS_ENUMERATE           (1)
#define MICROPY_PY_BUILTINS_FROZENSET           (0)
#define MICROPY_PY_BUILTINS_REVERSED            (0)
#define MICROPY_PY_BUILTINS_EXECFILE            (1)
#define MICROPY_PY_BUILTINS_SET                 (1)
#define MICROPY_PY_BUILTINS_SLICE               (1)
#define MICROPY_PY_BUILTINS_PROPERTY            (1)
#define MICROPY_PY_BUILTINS_TIMEOUTERROR        (1)
#define MICROPY_PY___FILE__                     (1)
#define MICROPY_PY_GC                           (1)
#define MICROPY_PY_ARRAY                        (1)
#define MICROPY_PY_ATTRTUPLE                    (1)
#define MICROPY_PY_COLLECTIONS                  (1)
#define MICROPY_PY_MATH                         (1)
#define MICROPY_PY_IO                           (1)
#define MICROPY_PY_IO_FILEIO                    (1)
#define MICROPY_PY_UJSON                        (1)
#define MICROPY_PY_UBINASCII                    (1)
#define MICROPY_PY_URE                          (1)
#define MICROPY_PY_STRUCT                       (1)
#define MICROPY_PY_SYS                          (1)
#define MICROPY_PY_MACHINE                      (1)
#define MICROPY_PY_UERRNO                       (1)
#define MICROPY_PY_SYS_EXIT                     (1)
#define MICROPY_PY_THREAD                       (0)
#define MICROPY_PY_LWIP                         (1)
#define MICROPY_PY_BUILTINS_FLOAT               (1)
#define MICROPY_MODULE_FROZEN_STR               (1)
#define MICROPY_MODULE_FROZEN_MPY               (1)
#define MICROPY_QSTR_EXTRA_POOL                 mp_qstr_frozen_const_pool

#define MICROPY_PY_TERM_NUM                     (3)

#define MICROPY_VFS_FAT                         (1)
#define MICROPY_READER_FATFS                    (MICROPY_VFS_FAT)
#define MICROPY_FATFS_ENABLE_LFN                (1)
#define MICROPY_FATFS_LFN_CODE_PAGE             (437) /* 1=SFN/ANSI 437=LFN/U.S.(OEM) */
#define MICROPY_FSUSERMOUNT                     (1)
#define MICROPY_FATFS_VOLUMES                   (4)
#define MICROPY_FATFS_RPATH                     (2)
#define MICROPY_FATFS_MAX_SS                    (4096)
#define MICROPY_FATFS_USE_LABEL                 (1)

// extra built in names to add to the global namespace
#define MICROPY_PORT_BUILTINS \
    { MP_OBJ_NEW_QSTR(MP_QSTR_help),  MP_OBJ_FROM_PTR(&mp_builtin_help_obj) }, \
    { MP_OBJ_NEW_QSTR(MP_QSTR_input), MP_OBJ_FROM_PTR(&mp_builtin_input_obj) },\
    { MP_OBJ_NEW_QSTR(MP_QSTR_open),  MP_OBJ_FROM_PTR(&mp_builtin_open_obj) }, \

extern const struct _mp_obj_module_t mp_module_umachine;
extern const struct _mp_obj_module_t mp_module_uos;
extern const struct _mp_obj_module_t mp_module_uterminal;
extern const struct _mp_obj_module_t mp_module_utime;
extern const struct _mp_obj_module_t mp_module_uwireless;

extern const struct _mp_obj_module_t mp_network_module;

extern const struct _mp_obj_module_t mp_module_lwip;

#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_OBJ_NEW_QSTR(MP_QSTR_uwireless),    MP_OBJ_FROM_PTR(&mp_module_uwireless) },  \
    { MP_OBJ_NEW_QSTR(MP_QSTR_network),      MP_OBJ_FROM_PTR(&mp_network_module) },    \
    { MP_OBJ_NEW_QSTR(MP_QSTR_usocket),      MP_OBJ_FROM_PTR(&mp_module_lwip) },    \
    { MP_OBJ_NEW_QSTR(MP_QSTR_umachine),     MP_OBJ_FROM_PTR(&mp_module_umachine) },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_uos),          MP_OBJ_FROM_PTR(&mp_module_uos) },        \
    { MP_OBJ_NEW_QSTR(MP_QSTR_utime),        MP_OBJ_FROM_PTR(&mp_module_utime) },      \
    { MP_OBJ_NEW_QSTR(MP_QSTR_uterm),        MP_OBJ_FROM_PTR(&mp_module_uterminal) },   \

#define MICROPY_PY_SYS_PLATFORM             "AmebaBoard"

#define MICROPY_HW_PORT_VERSION             "0.0.3"

#define MICROPY_HW_BOARD_NAME               MICROPY_PY_SYS_PLATFORM
#define MICROPY_HW_MCU_NAME                 "RTL8195A"

#define MICROPY_WLAN_AP_DEFAULT_SSID        "mpiot-ap"
#define MICROPY_WLAN_AP_DEFAULT_PASS        "password"

#define MICROPY_MAIN_TASK_STACK_SIZE        512
#define MICROPY_MAIN_TASK_PRIORITY          (1)

#define MICROPY_PORT_ROOT_POINTERS          \
    const char *readline_hist[8];           \
    mp_obj_list_t term_list_obj;            \
    mp_obj_list_t netif_list_obj;           \
    mp_obj_t dupterm_arr_obj;               \
    mp_obj_t mp_kbd_exception;              \
