#
#    Copyright (c) 2020 Project CHIP Authors
#    Copyright (c) 2014-2018 Nest Labs, Inc.
#    Copyright (c) 2019-2020 Google, LLC.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.
#

#
#    Description:
#      This file is the GNU automake template for the CHIP library.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

SUBDIRS                             =                         \
    support                                                   \
    core/tests                                                \
    $(NULL)

# Pull in the sources that comprise the CHIP library.

include ../ble/BleLayer.am
include ../controller/DeviceController.am
include ../inet/InetLayer.am
include ../system/SystemLayer.am
include core/CoreLayer.am
include support/SupportLayer.am

EXTRA_DIST                          =                         \
    $(CHIP_BUILD_CORE_LAYER_HEADER_FILES)                     \
    $(CHIP_BUILD_SUPPORT_LAYER_HEADER_FILES)                  \
    $(CHIP_BUILD_DEVICE_CONTROLLER_HEADER_FILES)

lib_LIBRARIES                       = libCHIP.a

libCHIP_a_CPPFLAGS                  =                         \
    -I$(top_srcdir)/src                                       \
    -I$(top_srcdir)/src/include                               \
    -I$(top_srcdir)/src/inet                                  \
    -I$(top_srcdir)/src/lib                                   \
    -I$(top_srcdir)/src/lib/core                              \
    -I$(top_srcdir)/src/lib/support                           \
    -I$(top_srcdir)/src/system                                \
    $(NLASSERT_CPPFLAGS)                                      \
    $(NLFAULTINJECTION_CPPFLAGS)                              \
    $(NLIO_CPPFLAGS)                                          \
    $(LWIP_CPPFLAGS)                                          \
    $(SOCKETS_CPPFLAGS)                                       \
    $(NULL)

libCHIP_a_SOURCES                   = $(CHIP_BUILD_SYSTEM_LAYER_SOURCE_FILES)
libCHIP_a_SOURCES                  += $(CHIP_BUILD_INET_LAYER_SOURCE_FILES)
libCHIP_a_SOURCES                  += $(CHIP_BUILD_CORE_LAYER_SOURCE_FILES)
libCHIP_a_SOURCES                  += $(CHIP_BUILD_SUPPORT_LAYER_SOURCE_FILES)
libCHIP_a_SOURCES                  += $(CHIP_BUILD_DEVICE_CONTROLLER_SOURCE_FILES)

if CONFIG_NETWORK_LAYER_BLE
libCHIP_a_SOURCES                  += $(CHIP_BUILD_BLE_LAYER_SOURCE_FILES)
endif # CONFIG_NETWORK_LAYER_BLE

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
