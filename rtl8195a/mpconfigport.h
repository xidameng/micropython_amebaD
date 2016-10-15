#include <stdint.h>

// options to control how Micro Python is built

#define MICROPY_QSTR_BYTES_IN_HASH              (1)
#define MICROPY_ALLOC_PATH_MAX                  (128)
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_EMIT_THUMB                      (0)
#define MICROPY_EMIT_INLINE_THUMB               (0)
#define MICROPY_COMP_MODULE_CONST               (1)
#define MICROPY_COMP_CONST                      (1)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN        (0)
#define MICROPY_COMP_TRIPLE_TUPLE_ASSIGN        (0)
#define MICROPY_ENABLE_COMPILER                 (1)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_ENABLE_FINALISER                (1)
#define MICROPY_HELPER_REPL                     (1)
#define MICROPY_ENABLE_FINALISER                (1)
#define MICROPY_ENABLE_SOURCE_LINE              (1)
#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_MPZ)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_FLOAT)
#define MICROPY_ENABLE_DOC_STRING               (1)
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_DETAILED)
#define MICROPY_REPL_AUTO_INDENT                (1)
#define MICROPY_OPT_COMPUTED_GOTO               (1)
#define MICROPY_MODULE_WEAK_LINKS               (1)
#define MICROPY_PY_BUILTINS_BYTEARRAY           (1)
#define MICROPY_PY_BUILTINS_MEMORYVIEW          (1)
#define MICROPY_PY_BUILTINS_ENUMERATE           (1)
#define MICROPY_PY_BUILTINS_FROZENSET           (1)
#define MICROPY_PY_BUILTINS_REVERSED            (1)
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
#define MICROPY_PY_IO                           (0)
#define MICROPY_PY_OS_DUPTERM                   (0)
#define MICROPY_PY_WEBSOCKET                    (1)
#define MICROPY_PY_IO_FILEIO                    (1)
#define MICROPY_PY_UCTYPES                      (1)
#define MICROPY_PY_UJSON                        (1)
#define MICROPY_PY_FRAMEBUF                     (1)
#define MICROPY_PY_UBINASCII                    (1)
#define MICROPY_PY_URE                          (1)
#define MICROPY_PY_STRUCT                       (1)
#define MICROPY_PY_SYS                          (1)
#define MICROPY_PY_MACHINE                      (1)
#define MICROPY_PY_UERRNO                       (1)
#define MICROPY_PY_SYS_EXIT                     (1)
#define MICROPY_PY_BUILTINS_FLOAT               (1)

#define MICROPY_PY_TERM_NUM                     (3)

#define MICROPY_FATFS_ENABLE_LFN                (1)
#define MICROPY_FATFS_LFN_CODE_PAGE             (437) /* 1=SFN/ANSI 437=LFN/U.S.(OEM) */
#define MICROPY_FSUSERMOUNT                     (1)
#define MICROPY_FATFS_VOLUMES                   (4)
#define MICROPY_FATFS_RPATH                     (2)
#define MICROPY_FATFS_MAX_SS                    (4096)
#define MICROPY_FATFS_USE_LABEL                 (1)


// type definitions for the specific machine

#define BYTES_PER_WORD (4)

#define MICROPY_MAKE_POINTER_CALLABLE(p) ((void*)((mp_uint_t)(p)))

#define UINT_FMT "%lu"
#define INT_FMT "%ld"

typedef int32_t mp_int_t; // must be pointer size
typedef uint32_t mp_uint_t; // must be pointer size
typedef void *machine_ptr_t; // must be of pointer size
typedef const void *machine_const_ptr_t; // must be of pointer size
typedef long mp_off_t;

#define MP_STATE_PORT MP_STATE_VM

extern const struct _mp_obj_fun_builtin_t mp_builtin_ftpd_obj;
//extern const struct _mp_obj_fun_builtin_t mp_builtin_mdns_obj;

// extra built in names to add to the global namespace
#define MICROPY_PORT_BUILTINS \
    { MP_OBJ_NEW_QSTR(MP_QSTR_open),        (mp_obj_t)&mp_builtin_open_obj },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_input),       (mp_obj_t)&mp_builtin_input_obj },  \
    { MP_OBJ_NEW_QSTR(MP_QSTR_help),        (mp_obj_t)&mp_builtin_help_obj },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_ftpd),        (mp_obj_t)&mp_builtin_ftpd_obj },   \

extern const struct _mp_obj_module_t mp_module_umachine;
extern const struct _mp_obj_module_t mp_module_uos;
extern const struct _mp_obj_module_t mp_module_uterminal;
extern const struct _mp_obj_module_t mp_module_utime;
//extern const struct _mp_obj_module_t mp_module_uwireless;

//extern const struct _mp_obj_module_t mp_module_usocket;
//extern const struct _mp_obj_module_t mp_module_ussl;
//
//extern const struct _mp_obj_module_t mp_network_module;
//extern const struct _mp_obj_module_t mp_crypto_module;
//extern const struct _mp_obj_module_t mp_uvc_module;
//
#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_OBJ_NEW_QSTR(MP_QSTR_umachine),     (mp_obj_t)&mp_module_umachine },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_uos),          (mp_obj_t)&mp_module_uos },        \
    { MP_OBJ_NEW_QSTR(MP_QSTR_uterm),         (mp_obj_t)&mp_module_uterminal },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_utime),      (mp_obj_t)&mp_module_utime },      \
    //{ MP_OBJ_NEW_QSTR(MP_QSTR_uwireless),  (mp_obj_t)&mp_module_uwireless },  \

    //{ MP_OBJ_NEW_QSTR(MP_QSTR_usocket),    (mp_obj_t)&mp_module_usocket },    \
    //{ MP_OBJ_NEW_QSTR(MP_QSTR_ussl),       (mp_obj_t)&mp_module_ussl },       \
    //{ MP_OBJ_NEW_QSTR(MP_QSTR_network),    (mp_obj_t)&mp_network_module },    \
    //{ MP_OBJ_NEW_QSTR(MP_QSTR_uvc),        (mp_obj_t)&mp_uvc_module },        \
    //{ MP_OBJ_NEW_QSTR(MP_QSTR_crypto),     (mp_obj_t)&mp_crypto_module },     \

#define MICROPY_PORT_BUILTIN_MODULE_WEAK_LINKS \
    { MP_OBJ_NEW_QSTR(MP_QSTR_binascii),  (mp_obj_t)&mp_module_ubinascii },  \
    { MP_OBJ_NEW_QSTR(MP_QSTR_re),        (mp_obj_t)&mp_module_ure },        \
    { MP_OBJ_NEW_QSTR(MP_QSTR_json),      (mp_obj_t)&mp_module_ujson },      \
    { MP_OBJ_NEW_QSTR(MP_QSTR_machine),   (mp_obj_t)&mp_module_umachine },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_os),        (mp_obj_t)&mp_module_uos },        \
    { MP_OBJ_NEW_QSTR(MP_QSTR_term),      (mp_obj_t)&mp_module_uterminal },        \
    { MP_OBJ_NEW_QSTR(MP_QSTR_time),      (mp_obj_t)&mp_module_utime },      \
    //{ MP_OBJ_NEW_QSTR(MP_QSTR_wireless),  (mp_obj_t)&mp_module_uwireless },  \

    //{ MP_OBJ_NEW_QSTR(MP_QSTR_socket),    (mp_obj_t)&mp_module_usocket },    \
    //{ MP_OBJ_NEW_QSTR(MP_QSTR_ssl),       (mp_obj_t)&mp_module_ussl },       \

// There is no classical C heap in bare-metal ports, only Python
// garbage-collected heap. For completeness, emulate C heap via
// GC heap. Note that MicroPython core never uses malloc() and friends,
// so these defines are mostly to help extension module writers.
#define malloc  gc_alloc
#define free    gc_free
#define realloc gc_realloc

// We need to provide a declaration/definition of alloca()
#include <alloca.h>
#include "cmsis_os.h"

#define MICROPY_PY_SYS_PLATFORM             "AmebaBoard"

#define MICROPY_HW_PORT_VERSION             "0.0.1"

#define MICROPY_HW_BOARD_NAME               MICROPY_PY_SYS_PLATFORM
#define MICROPY_HW_MCU_NAME                 "RTL8195A"

#define MICROPY_WLAN_AP_DEFAULT_SSID        "mpiot-ap"
#define MICROPY_WLAN_AP_DEFAULT_PASS        "1234567890"

#define MICROPY_FTPD_STACK_SIZE             512
#define MICROPY_FTPD_TASK_PRIORITY          osPriorityBelowNormal

#define MICROPY_MAIN_TASK_STACK_SIZE        1024 * 20
#define MICROPY_MAIN_TASK_PRIORITY          osPriorityHigh

#define MICROPY_PORT_ROOT_POINTERS          \
    const char *readline_hist[8];           \
    mp_obj_list_t term_list_obj;            \
    mp_obj_t dupterm_arr_obj;               \
    mp_obj_t mp_kbd_exception;              \
    struct _pyb_uart_obj_t *pyb_stdio_uart; \

#define ENOTSUP 524
