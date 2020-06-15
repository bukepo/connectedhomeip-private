/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *    Copyright (c) 2019 Nest Labs, Inc.
 *    All rights reserved.
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
 *          Contains non-inline method definitions for the
 *          GenericThreadStackManagerImpl_FreeRTOS<> template.
 */

#ifndef GENERIC_THREAD_STACK_MANAGER_IMPL_FREERTOS_IPP
#define GENERIC_THREAD_STACK_MANAGER_IMPL_FREERTOS_IPP

#include <openthread/joiner.h>
#include <platform/FreeRTOS/GenericThreadStackManagerImpl_FreeRTOS.h>
#include <platform/ThreadStackManager.h>
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <support/CodeUtils.h>
#include <support/logging/CHIPLogging.h>

#include "setup_payload/QRCodeSetupPayloadGenerator.h"
#include "setup_payload/SetupPayload.h"

namespace chip {
namespace DeviceLayer {
namespace Internal {

// Fully instantiate the generic implementation class in whatever compilation unit includes this file.
template class GenericThreadStackManagerImpl_FreeRTOS<ThreadStackManagerImpl>;

template <class ImplClass>
CHIP_ERROR GenericThreadStackManagerImpl_FreeRTOS<ImplClass>::DoInit(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    mThreadStackLock = xSemaphoreCreateMutex();
    if (mThreadStackLock == NULL)
    {
        ChipLogError(DeviceLayer, "Failed to create Thread stack lock");
        ExitNow(err = CHIP_ERROR_NO_MEMORY);
    }

    mThreadTask = NULL;

exit:
    return err;
}

template <class ImplClass>
CHIP_ERROR GenericThreadStackManagerImpl_FreeRTOS<ImplClass>::_StartThreadTask(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    BaseType_t res;

    VerifyOrExit(mThreadTask == NULL, err = CHIP_ERROR_INCORRECT_STATE);

    res = xTaskCreate(ThreadTaskMain, CHIP_DEVICE_CONFIG_THREAD_TASK_NAME,
                      CHIP_DEVICE_CONFIG_THREAD_TASK_STACK_SIZE / sizeof(StackType_t), this,
                      CHIP_DEVICE_CONFIG_THREAD_TASK_PRIORITY, NULL);
    VerifyOrExit(res == pdPASS, err = CHIP_ERROR_NO_MEMORY);

exit:
    return err;
}

template <class ImplClass>
void GenericThreadStackManagerImpl_FreeRTOS<ImplClass>::_LockThreadStack(void)
{
    xSemaphoreTake(mThreadStackLock, portMAX_DELAY);
}

template <class ImplClass>
bool GenericThreadStackManagerImpl_FreeRTOS<ImplClass>::_TryLockThreadStack(void)
{
    return xSemaphoreTake(mThreadStackLock, 0) == pdTRUE;
}

template <class ImplClass>
void GenericThreadStackManagerImpl_FreeRTOS<ImplClass>::_UnlockThreadStack(void)
{
    xSemaphoreGive(mThreadStackLock);
}

template <class ImplClass>
void GenericThreadStackManagerImpl_FreeRTOS<ImplClass>::SignalThreadActivityPending()
{
    if (mThreadTask != NULL)
    {
        xTaskNotifyGive(mThreadTask);
    }
}

template <class ImplClass>
BaseType_t GenericThreadStackManagerImpl_FreeRTOS<ImplClass>::SignalThreadActivityPendingFromISR()
{
    BaseType_t yieldRequired = pdFALSE;

    if (mThreadTask != NULL)
    {
        vTaskNotifyGiveFromISR(mThreadTask, &yieldRequired);
    }

    return yieldRequired;
}

template <class ImplClass>
void GenericThreadStackManagerImpl_FreeRTOS<ImplClass>::ThreadTaskMain(void * arg)
{
    char pskd[27 / 5 + 1 + 1] = {};

    GenericThreadStackManagerImpl_FreeRTOS<ImplClass> * self =
        static_cast<GenericThreadStackManagerImpl_FreeRTOS<ImplClass> *>(arg);

    VerifyOrDie(self->mThreadTask == NULL);

    ChipLogDetail(DeviceLayer, "Thread task running");

    // Capture the Thread task handle.
    self->mThreadTask = xTaskGetCurrentTaskHandle();

    chip::SetupPayload payload;

    payload.version      = 1;
    payload.vendorID     = 1;
    payload.productID    = 1;
    payload.setUpPINCode = 0x202020;

    for (size_t i = 0; i < sizeof(pskd) - 1; ++i)
    {
        pskd[i] = "0123456789ABCDEFGHJKLMNPRSTUVWXY"[(payload.setUpPINCode >> (i * 5)) & ((1 << 5) - 1)];
    }

    ChipLogDetail(DeviceLayer, "PSKd: %s", pskd);

    QRCodeSetupPayloadGenerator generator(payload);

    std::string result;
    CHIP_ERROR err = generator.payloadBase41Representation(result);

    if (err == CHIP_NO_ERROR)
    {
        ChipLogDetail(DeviceLayer, "QRCode: %s", result.c_str());
    }
    else
    {
        ChipLogError(DeviceLayer, "Couldn't get payload string %d", err);
    }

    // start joinging on boot.
    self->mJoinPending = true;

    while (true)
    {
        // Lock the Thread stack.
        self->Impl()->LockThreadStack();

        if (self->mJoinPending)
        {
            self->mJoinPending = false;
            self->Impl()->_JoinerStart(pskd);
        }

        // Process any pending Thread activity.
        self->Impl()->ProcessThreadActivity();

        // Unlock the Thread stack.
        self->Impl()->UnlockThreadStack();

        // Wait for a signal that more activity is pending.
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip

#endif // GENERIC_THREAD_STACK_MANAGER_IMPL_FREERTOS_IPP
