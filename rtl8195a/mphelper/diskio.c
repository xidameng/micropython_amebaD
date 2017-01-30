#include "py/mpconfig.h"
#include "py/runtime.h"
#include "py/obj.h"
#include "lib/oofatfs/ff.h"

#include "timeutils.h"
#include "rtc_api.h"




#if !_FS_READONLY && !_FS_NORTC
DWORD get_fattime (
    void
)
{
    timeutils_struct_time_t tm;
    //TODO(Chester)
    timeutils_seconds_since_2000_to_struct_time(rtc_read(), &tm);

    return ((tm.tm_year - 1980) << 25) | ((tm.tm_mon) << 21)  |
            ((tm.tm_mday) << 16)       | ((tm.tm_hour) << 11) |
            ((tm.tm_min) << 5)         | (tm.tm_sec >> 1);
}
#endif


