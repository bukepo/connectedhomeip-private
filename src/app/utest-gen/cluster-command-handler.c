/**
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 *    @file
 *      This file provides message processing dispatch normally generated by the
 *      ZCL Advanced Platform (ZAP) generator for use in the CHIP ZCL
 *      Application Layer. These declarations are provided here for the
 *      purposes of unit testing the CHIP ZCL Application Layer
 *
 */

#include "cluster-command-handler.h"

static ChipZclStatus_t status(bool wasHandled, bool clusterExists, bool mfgSpecific)
{
    if (wasHandled)
    {
        return CHIP_ZCL_STATUS_SUCCESS;
    }
    else if (mfgSpecific)
    {
        return CHIP_ZCL_STATUS_UNSUP_MANUF_CLUSTER_COMMAND;
    }
    else if (clusterExists)
    {
        return CHIP_ZCL_STATUS_UNSUP_CLUSTER_COMMAND;
    }
    else
    {
        return CHIP_ZCL_STATUS_UNSUPPORTED_CLUSTER;
    }
}

// Main cluster specific command parsing controller.
ChipZclStatus_t chipZclClusterCommandParse(ChipZclCommandContext_t * context)
{
    ChipZclStatus_t result = status(false, false, context->mfgSpecific);
    if (context->direction == (uint8_t) ZCL_DIRECTION_CLIENT_TO_SERVER)
    {
        switch (context->clusterId)
        {
        case CHIP_ZCL_CLUSTER_ON_OFF:
            result = chipZclOnOffClusterServerCommandParse(context);
            break;
        default:
            // Unrecognized cluster ID, error status will apply.
            break;
        }
    }
    return result;
}

// Cluster: On/off, server
ChipZclStatus_t chipZclOnOffClusterServerCommandParse(ChipZclCommandContext_t * context)
{
    bool wasHandled = false;
    if (!context->mfgSpecific)
    {
        switch (context->commandId)
        {
        case ZCL_OFF_COMMAND_ID: {
            // Command is fixed length: 0
            ChipZclClusterOnOffServerCommandOffRequest_t request;
            request.dummy = true;
            chipZclClusterOnOffServerCommandOffRequestHandler(context, &request);
            wasHandled = true;
            break;
        }
        case ZCL_ON_COMMAND_ID: {
            // Command is fixed length: 0
            ChipZclClusterOnOffServerCommandOnRequest_t request;
            request.dummy = true;
            chipZclClusterOnOffServerCommandOnRequestHandler(context, &request);
            wasHandled = true;
            break;
        }
        case ZCL_TOGGLE_COMMAND_ID: {
            // Command is fixed length: 0
            ChipZclClusterOnOffServerCommandToggleRequest_t request;
            request.dummy = true;
            chipZclClusterOnOffServerCommandToggleRequestHandler(context, &request);
            wasHandled = true;
            break;
        }
        default: {
            // Unrecognized command ID, error status will apply.
            break;
        }
        }
    }
    return status(wasHandled, true, context->mfgSpecific);
}
