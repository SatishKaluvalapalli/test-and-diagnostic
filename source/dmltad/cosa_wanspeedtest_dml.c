/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2022 Deutsche Telekom AG
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

/**************************************************************************

    module: cosa_wanspeedtest_dml.c

        For COSA Data Model Library Development

    -------------------------------------------------------------------

    description:

        This file implements back-end apis for the COSA Data Model Library

    -------------------------------------------------------------------

    environment:

        platform independent

**************************************************************************/

#include "ansc_platform.h"
#include "cosa_diagnostic_apis.h"
#include "plugin_main_apis.h"
#include "cosa_wanspeedtest_dml.h"
#include "diag.h"
#include "ansc_string_util.h"
#include "ccsp_base_api.h"

BOOL gbSpeedTest_TestUsable = TRUE;

/***********************************************************************
 IMPORTANT NOTE:

 According to TR69 spec:
 On successful receipt of a SetParameterValues RPC, the CPE MUST apply
 the changes to all of the specified Parameters atomically. That is, either
 all of the value changes are applied together, or none of the changes are
 applied at all. In the latter case, the CPE MUST return a fault response
 indicating the reason for the failure to apply the changes.

 The CPE MUST NOT apply any of the specified changes without applying all
 of them.

 In order to set parameter values correctly, the back-end is required to
 hold the updated values until "Validate" and "Commit" are called. Only after
 all the "Validate" passed in different objects, the "Commit" will be called.
 Otherwise, "Rollback" will be called instead.

 The sequence in COSA Data Model will be:

 SetParamBoolValue/SetParamIntValue/SetParamUlongValue/SetParamStringValue
 -- Backup the updated values;

 if( Validate_XXX())
 {
     Commit_XXX();    -- Commit the update all together in the same object
 }
 else
 {
     Rollback_XXX();  -- Remove the update at backup;
 }

***********************************************************************/
/***********************************************************************

   APIs for Object:

       Device.X_T-ONLINE-DE_Speedtest.

***********************************************************************/
/***********************************************************************

   APIs for Object:

       Device.X_T-ONLINE-DE_Speedtest.

        *  WANSpeedtest_GetParamBoolValue
        *  WANSpeedtest_SetParamBoolValue
        *  WANSpeedtest_Validate
        *  WANSpeedtest_Commit
        *  WANSpeedtest_Rollback

***********************************************************************/
/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WANSpeedtest_GetParamBoolValue
           (
               ANSC_HANDLE                 hInsContext,
               char*                       ParamName,
               BOOL*                       pBool
           );

    description:
        This function is called to retrieve BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool,
                The buffer of returned BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
WANSpeedtest_GetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL*                       pBool
    )
{
    /* check the parameter name and return the corresponding value */
    if (AnscEqualString(ParamName, "TestUsable", TRUE))
    {
        /* collect value */
        *pBool    =  gbSpeedTest_TestUsable;

        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WANSpeedtest_SetParamBoolValue
           (
               ANSC_HANDLE                 hInsContext,
               char*                       ParamName,
               BOOL                        bValue
           );

    description:
        This function is called to retrieve BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue,
                The buffer of returned BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL
WANSpeedtest_SetParamBoolValue
    (
        ANSC_HANDLE                 hInsContext,
        char*                       ParamName,
        BOOL                        bValue
    )
{
    if (AnscEqualString(ParamName, "TestUsable", TRUE))
    {
        gbSpeedTest_TestUsable = bValue;
        return TRUE;
    }

    /* AnscTraceWarning(("Unsupported parameter '%s'\n", ParamName)); */
    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WANSpeedtest_Validate
           (
                ANSC_HANDLE                 hInsContext,
                char*                       pReturnParamName,
                ULONG*                      puLength
           );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       pReturnParamName,
                The buffer (128 bytes) of parameter name if there's a validation.

                ULONG*                      puLength
                The output length of the param name.

    return:     TRUE if there's no validation.

**********************************************************************/

BOOL
WANSpeedtest_Validate
    (
        ANSC_HANDLE                 hInsContext,
        char*                       pReturnParamName,
        ULONG*                      puLength
    )
{
    if ((gbSpeedTest_TestUsable == TRUE) || (gbSpeedTest_TestUsable == FALSE))
    {
        AnscCopyString(pReturnParamName, "TestUsable");
        return TRUE;
    }

    return FALSE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WANSpeedtest_Commit
           (
               ANSC_HANDLE                 hInsContext
           );

    description:

        This function is called to finally commit all the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

BOOL
WANSpeedtest_Commit
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL
        WANSpeedtest_Rollback
           (
                ANSC_HANDLE                 hInsContext
           );

    description:

        This function is called to roll back the update whenever there's a
        validation found.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

BOOL
WANSpeedtest_Rollback
    (
        ANSC_HANDLE                 hInsContext
    )
{
    return TRUE;
}
