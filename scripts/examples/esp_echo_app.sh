#!/bin/bash

source examples/wifi-echo/server/esp32/idf.sh
idf make -C examples/wifi-echo/server/esp32 defconfig
idf make -C examples/wifi-echo/server/esp32
