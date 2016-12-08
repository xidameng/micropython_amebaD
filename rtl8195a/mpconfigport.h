#include <stdint.h>

#ifdef MP_RTL8195A
#include "mpconfigports/rtl8195a_ports.h"
#elif MP_RTL8711AM
#include "mpconfigports/rtl8711am_ports.h"
#elif MP_RTL8711AF
#include "mpconfigports/rtl8711af_ports.h"
#elif MP_RTL8710AF
#include "mpconfigports/rtl8710af_ports.h"
#else
#warning "Invalid CHIP set"
#endif

#define mp_type_fileio fatfs_type_fileio
#define mp_type_textio fatfs_type_textio

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
