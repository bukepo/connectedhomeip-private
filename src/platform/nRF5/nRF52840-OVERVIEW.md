# Overview of CHIP nRF52840 Adaption

The following is a quick overview of the nRF5 adaptation of CHIP. Most of this
code will have parallels in any new adaptation.

(All file names are relative to `connectedhomeip/src/...`).

`include/platform/nRF5/PlatformManagerImpl.h`<br>`nRF5/PlatformManagerImpl.cpp`

-   Concrete implementation of PlatformManager interface
-   Provides initialization of the CHIP stack and core event loop for the chip
    task
-   Relies on GenericPlatformManagerImpl_FreeRTOS<> class to provide most of the
    implementation

`include/platform/nRF5/ConfigurationManagerImpl.h`<br>`nRF5/ConfigurationManagerImpl.cpp`

-   Concrete implementation of ConfigurationManager interface
-   Manages storage and retrieval of persistent configuration data
-   Relies on GenericConfigurationManagerImpl<> classes to implement most API
    functionality
-   Delegates low-level reading and writing of persistent values to NRF5Config
    class

`include/platform/nRF5/ConnectivityManagerImpl.h`<br>`nRF5/ConnectivityManagerImpl.cpp`

-   Concrete implementation of ConnectivityManager interface
-   Provides high-level APIs for managing device connectivity
-   Relies on various generic implementation classes to provide API
    functionality
-   Very much a work-in-progress in the nrf52840 branch

`include/platform/nRF5/ThreadStackManagerImpl.h`<br>`nRF5/ThreadStackManagerImpl.cpp`

-   Concrete implementation of ThreadStackManager interface
-   Supports Thread stack initialization and core event loop processing
-   Relies on GenericThreadStackManagerImpl_OpenThread/FreeRTOS/LwIP<> classes
    to implement most API functionaltiy
-   Work-in-progress [2019/04/01]

`include/platform/nRF5/BLEManagerImpl.h`<br>`nRF5/BLEManagerImpl.cpp`

-   Concrete implementation of the BLEManager interface
-   Maps CHIP's BLE interface abstractions (BleLayer, BlePlatformDelegate,
    BleApplicationDelegate) onto the platform's native BLE services
-   Implements chip-compatible BLE advertising.

`platform/nRF5/Entropy.cpp`

-   Implements interface to platform entropy source

`platform/nRF5/Logging.cpp`

-   Adaption of chip debug logging to platform logging facility.

`platform/nRF5/NRF5Config.cpp`

-   Implements low-level read/write of persistent configuration values
-   Class API specifically designed to work in conjunction with the
    GenericConfigurationManagerImpl<> class.
