#ifndef  _BBHM_COMMON_INTERNAL_API_
#define  _BBHM_COMMON_INTERNAL_API_

#include "user_time.h"
#include "ssp_global.h"

#define  PAM_COMPONENT_NAME             "eRT.com.cisco.spvtg.ccsp.pam"
#define  PAM_DBUS_PATH                  "/com/cisco/spvtg/ccsp/pam"
#define  INTERFACE_STATS_Tx_BYTES       "Device.IP.Interface.1.Stats.BytesSent"
#define  INTERFACE_STATS_Rx_BYTES       "Device.IP.Interface.1.Stats.BytesReceived"

double
CalculateTimeDifference
    (
        USER_SYSTEM_TIME*  start_time,
        USER_SYSTEM_TIME*  stop_time
    );

ANSC_STATUS
Tad_GetParamValues
    (
        char  *pchComponent,
        char  *pchBus,
        char  *pchParamName,
        ULONG *pulReturnVal
    );

ANSC_STATUS
Tad_GetParamString
    (
        char  *pchComponent,
        char  *pchBus,
        char  *pchParamName,
        char  *pchBuf,
        ULONG  ulBufSize
    );

#endif
