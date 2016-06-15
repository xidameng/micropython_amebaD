#include <stdint.h>

// options to control how Micro Python is built

#define MICROPY_QSTR_BYTES_IN_HASH              (1)
#define MICROPY_ALLOC_PATH_MAX                  (128)
#define MICROPY_PERSISTENT_CODE_LOAD            (1)
#define MICROPY_EMIT_THUMB                      (1)
#define MICROPY_EMIT_INLINE_THUMB               (1)
#define MICROPY_COMP_MODULE_CONST               (1)
#define MICROPY_COMP_CONST                      (1)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN        (0)
#define MICROPY_COMP_TRIPLE_TUPLE_ASSIGN        (0)
#define MICROPY_MEM_STATS                       (0)
#define MICROPY_ENABLE_COMPILER                 (1)
#define MICROPY_ENABLE_GC                       (1)
#define MICROPY_ENABLE_FINALISER                (1)
#define MICROPY_HELPER_REPL                     (1)
#define MICROPY_ENABLE_FINALISER                (1)
#define MICROPY_ENABLE_SOURCE_LINE              (1)
#define MICROPY_LONGINT_IMPL                    (MICROPY_LONGINT_IMPL_NONE)
#define MICROPY_FLOAT_IMPL                      (MICROPY_FLOAT_IMPL_NONE)
#define MICROPY_ENABLE_DOC_STRING               (1)
#define MICROPY_ERROR_REPORTING                 (MICROPY_ERROR_REPORTING_DETAILED)
#define MICROPY_REPL_AUTO_INDENT                (1)
#define MICROPY_OPT_COMPUTED_GOTO               (1)
#define MICROPY_MODULE_WEAK_LINKS               (1)
#define MICROPY_PY_BUILTINS_BYTEARRAY           (1)
#define MICROPY_PY_BUILTINS_MEMORYVIEW          (0)
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
#define MICROPY_PY_LWIP                         (1)
#define MICROPY_PY_ARRAY                        (1)
#define MICROPY_PY_ATTRTUPLE                    (1)
#define MICROPY_PY_COLLECTIONS                  (1)
#define MICROPY_PY_MATH                         (1)
#define MICROPY_PY_CMATH                        (0)
#define MICROPY_PY_IO                           (0)
#define MICROPY_PY_IO_FILEIO                    (1)
#define MICROPY_PY_UZLIB                        (1)
#define MICROPY_PY_UHASHLIB                     (1)
#define MICROPY_PY_UJSON                        (1)
#define MICROPY_PY_URANDOM                      (1)
#define MICROPY_PY_URE                          (1)
#define MICROPY_PY_STRUCT                       (1)
#define MICROPY_PY_SYS                          (1)
#define MICROPY_PY_SYS_EXIT                     (1)
#define MICROPY_PY_OS_DUPTERM                   (0)
#define MICROPY_PY_MACHINE_PULSE                (0)
#define MICROPY_PY_MACHINE                      (0)
#define MICROPY_PY_BUILTINS_FLOAT               (1)

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

// dummy print
#define MP_PLAT_PRINT_STRN(str, len) mp_hal_stdout_tx_strn_cooked(str, len)

// extra built in names to add to the global namespace
extern const struct _mp_obj_fun_builtin_t mp_builtin_open_obj;
#define MICROPY_PORT_BUILTINS \
    { MP_OBJ_NEW_QSTR(MP_QSTR_open), (mp_obj_t)&mp_builtin_open_obj },

extern const struct _mp_obj_module_t mp_hardware_module;
extern const struct _mp_obj_module_t mp_wireless_module;
extern const struct _mp_obj_module_t mp_network_module;
extern const struct _mp_obj_module_t mp_time_module;
extern const struct _mp_obj_module_t mp_uos_module;
extern const struct _mp_obj_module_t mp_module_lwip;

#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_OBJ_NEW_QSTR(MP_QSTR_hardware),  (mp_obj_t)&mp_hardware_module },  \
    { MP_OBJ_NEW_QSTR(MP_QSTR_wireless),  (mp_obj_t)&mp_wireless_module },      \
    { MP_OBJ_NEW_QSTR(MP_QSTR_network),   (mp_obj_t)&mp_network_module },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_time),      (mp_obj_t)&mp_time_module },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_os),        (mp_obj_t)&mp_uos_module },   \
    { MP_OBJ_NEW_QSTR(MP_QSTR_lwip),      (mp_obj_t)&mp_module_lwip },   \

// There is no classical C heap in bare-metal ports, only Python
// garbage-collected heap. For completeness, emulate C heap via
// GC heap. Note that MicroPython core never uses malloc() and friends,
// so these defines are mostly to help extension module writers.
#define malloc  gc_alloc
#define free    gc_free
#define realloc gc_realloc

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

#define MICROPY_HW_BOARD_NAME "Ameba Board"
#define MICROPY_HW_MCU_NAME "RT8195A"

#define MICROPY_PORT_ROOT_POINTERS      \
    const char *readline_hist[8];       \
    mp_obj_t mp_const_user_interrupt;   \

#include "FreeRTOS.h"
#include <semphr.h>
#include "errno.h"

#define ENOTSUP 524
