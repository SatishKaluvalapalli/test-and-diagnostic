/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2015 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#include "ansc_platform.h"
#include "ssp_global.h"
#include "bbhm_common_internal_api.h"

extern ANSC_HANDLE bus_handle;

double
CalculateTimeDifference
    (
        USER_SYSTEM_TIME*  MeasurementStopTime,
        USER_SYSTEM_TIME*  MeasurementStartTime
    )
{
    long long TimeInMilliSeconds[2] = {0, 0};
    double TimeDifference = 0;
    int Year, Month, DayOfMonth, Hour, Minute, Second, MilliSecond;
    USER_SYSTEM_TIME* MeasurementTime[] = {MeasurementStopTime, MeasurementStartTime};
    int index;

    for (index = 0 ; index < 2 ; index++)
    {
        Year = MeasurementTime[index]->Year;
        Month = MeasurementTime[index]->Month;
        DayOfMonth = MeasurementTime[index]->DayOfMonth;
        Hour = MeasurementTime[index]->Hour;
        Minute = MeasurementTime[index]->Minute;
        Second = MeasurementTime[index]->Second;
        MilliSecond = MeasurementTime[index]->MilliSecond;

        long long totalSeconds = ((long long)(Year - 1900) * 31536000LL)
            + ((Month - 1) * 2592000LL) + (DayOfMonth * 86400LL)
            + (Hour * 3600LL) + (Minute * 60LL) + Second;

        TimeInMilliSeconds[index] = totalSeconds * 1000LL + MilliSecond;
    }

    TimeDifference = (double)(TimeInMilliSeconds[0] - TimeInMilliSeconds[1]) / 1000.0;

    CcspTraceInfo(("%s: The difference between the two times is %.3f seconds\n",
            __FUNCTION__, TimeDifference));

    return TimeDifference;
}

ANSC_STATUS Tad_GetParamValues(char *pchComponent, char *pchBus, char *pchParamName, ULONG *pulReturnVal)
{
    CcspTraceWarning(("Inside %s\n", __FUNCTION__));
    CCSP_MESSAGE_BUS_INFO  *pBusInfo = (CCSP_MESSAGE_BUS_INFO *)bus_handle;
    parameterValStruct_t   **ppRetVal;
    char                   *pchTmpParamName[1];
    int                    iRet = 0,iNVal;

    (void)pBusInfo;
    pchTmpParamName[0] = pchParamName;

    iRet = CcspBaseIf_getParameterValues(
                                    bus_handle,
                                    pchComponent,
                                    pchBus,
                                    pchTmpParamName,
                                    1,
                                    &iNVal,
                                    &ppRetVal);


    if (iRet != CCSP_SUCCESS)
    {
        return ANSC_STATUS_FAILURE;
    }
    else if (CCSP_SUCCESS == iRet)
    {
        if (ppRetVal && (NULL != ppRetVal[0]->parameterValue))
        {
            *pulReturnVal = strtoul(ppRetVal[0]->parameterValue, NULL, 10);

        }
        if (ppRetVal)
        {
            free_parameterValStruct_t(bus_handle, iNVal, ppRetVal);
        }
        return ANSC_STATUS_SUCCESS;
    }

    if (ppRetVal)
    {
       free_parameterValStruct_t(bus_handle, iNVal, ppRetVal);
    }
    return ANSC_STATUS_FAILURE;
}

/* Fetch a DM parameter as a string into pchBuf buffer. */
ANSC_STATUS
Tad_GetParamString
    (
        char  *pchComponent,
        char  *pchBus,
        char  *pchParamName,
        char  *pchBuf,
        ULONG  ulBufSize
    )
{
    parameterValStruct_t   **ppRetVal = NULL;
    char                   *pchTmpParamName[1];
    int                      iRet  = 0;
    int                      iNVal = 0;

    if (pchComponent == NULL || pchBus == NULL || pchParamName == NULL
            || pchBuf == NULL || ulBufSize == 0)
        return ANSC_STATUS_FAILURE;

    pchBuf[0] = '\0';
    pchTmpParamName[0] = pchParamName;

    iRet = CcspBaseIf_getParameterValues(
                                    bus_handle,
                                    pchComponent,
                                    pchBus,
                                    pchTmpParamName,
                                    1,
                                    &iNVal,
                                    &ppRetVal);

    if (iRet == CCSP_SUCCESS)
    {
        if (ppRetVal && ppRetVal[0] && (NULL != ppRetVal[0]->parameterValue))
        {
            snprintf(pchBuf, (size_t)ulBufSize, "%s", ppRetVal[0]->parameterValue);
        }
        if (ppRetVal)
        {
            free_parameterValStruct_t(bus_handle, iNVal, ppRetVal);
        }
        return ANSC_STATUS_SUCCESS;
    }

    if (ppRetVal)
    {
        free_parameterValStruct_t(bus_handle, iNVal, ppRetVal);
    }
    return ANSC_STATUS_FAILURE;
}
