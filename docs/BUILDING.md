## Build Documentation

The CHIP build system uses GNU autotools to build various platform images on
Linux or MacOS.

Tested on:

-   MacOS
-   Ubuntu 18.04

Build system features:

-   Package management: 'make dist' and 'make distcheck'
-   Cross-platform handling: (Linux, Darwin, iOS, Android, embedded arm, etc.)
-   Multiple compiler support: clang, GCC
-   Integrates automated testing framework: 'make check'
-   Code style enforcement: 'make pretty' and 'make pretty-check' integration
    with clang-format

Assuming you have all the required tools installed, the system will build the
code, build a distribution, tidy, format, run tests, and run code coverage on
the entire tree. Tests are built into the make system.

### Tool Prerequisites

To take advantage of all the current capabilities of the make system, you'll
want:

-   Bash 4.0 or greater
-   GNU make (versions prior to 4.1 may produce spurious warnings/errors)
-   automake, autoconf, libtool
-   C and C++ compilers
-   clang-format-9
-   gcov

#### How to install tool prerequisites on Linux

On Debian-based Linux distributions such as Ubuntu, these dependencies can be
satisfied with the following:

```
sudo apt-get install make autoconf automake libtool
sudo apt-get install clang-format-9
sudo apt-get install lcov
```

#### How to install tool prerequisites on macOS

On macOS, these dependencies can be installed and satisfied using
[Brew](https://brew.sh/):

```
brew install make autoconf automake libtool
brew install llvm@9
brew install lcov
```

### Autotools Build Preparation

Before running any other build command, the `./bootstrap` command must be run
from the top-level.

```
# Initial preparation
./bootstrap
```

### Build Standalone (Native Linux or MacOS)

This will build all sources, libraries, and tests for the given platform:

```
# From top of clean tree
./bootstrap

make -f Makefile-Standalone
```

The helper Makefile-<platform> will automatically run configure the using a
default set of parameters, and allow custom override parameters to be passed via
environment variables. An example of this follows:

```
TESTS=1 DEBUG=1 COVERAGE=1 make -f Makefile-Standalone
```

At any time after this if a Makefile.am is updated, `./bootstrap -w make` must
be run again for the changes to be picked up.

### Build Custom configuration

```
# From top of clean tree
./bootstrap

mkdir build/<CONFIG>
cd build/<CONFIG>
../../configure -C <CONFIG ARGUMENTS>
```

Where `<CONFIG>` is something that describes what configuration (as described by
`<CONFIG ARGUMENTS>`) of the tree you're planning to build, or simply `out` if
you're not feeling creative.

-   [x] **Build all source, libraries, and tests**

```
make
```

-   [x] **Build distribution**

```
make dist
```

-   [x] **Build and check distribution, running all functional and unit tests**

```
make distcheck
```

-   [x] **Run tests**

```
make check
```

-   [x] **Verify coding style conformance**

```
make pretty-check
```

-   [x] **Auto-enforce coding style**

```
make pretty
```

-   [x] **Build documentation distribution from code headers**

To build the documentation bundle using doxygen, run the following:

```
make docdist
```

The top of the browsable web documentation can be found in
`docs/html/index.html`.

-   [x] **Build just one module in a subdirectory**

Either enter the desired subdirectory directly and run `make` or pass the
desired subdirectory to `make -C`.

```
$ make -C src/system

Making all in tests
make[1]: Entering directory 'src/system/tests'
make[1]: Nothing to be done for 'all'.
make[1]: Leaving directory 'src/system/tests'
make[1]: Entering directory 'src/system'
  CXX      ../../src/system/libSystemLayer_a-SystemClock.o
  CXX      ../../src/system/libSystemLayer_a-SystemError.o
  CXX      ../../src/system/libSystemLayer_a-SystemLayer.o
  CXX      ../../src/system/libSystemLayer_a-SystemMutex.o
  CXX      ../../src/system/libSystemLayer_a-SystemObject.o
  CXX      ../../src/system/libSystemLayer_a-SystemTimer.o
  CXX      ../../src/system/libSystemLayer_a-SystemPacketBuffer.o
  CXX      ../../src/system/libSystemLayer_a-SystemStats.o
  CXX      ../../src/system/libSystemLayer_a-SystemFaultInjection.o
  AR       libSystemLayer.a
ar: `u' modifier ignored since `D' is the default (see `U')
make[1]: Leaving directory 'src/system'
```

-   [x] **Add new source folder**

To add a new source folder, say under `src/<new>`, follow these steps:

1. Create the `src/<new>` directory and add a new `Makefile.am` within it.
2. Update `src/Makefile.am` and add a reference to the `<new>` folder under
   `SUBDIRS`.
3. Update `AC_CONFIG_FILES` in `configure.ac` to reference the `<new>/Makefile`
   (not the `Makefile.am`).
4. Rerun bootstrap and configure.
5. Now build as needed.

-   [x] **Add new source files**

To add new source files to the project, say under `src/<module>`, follow these
steps:

1. Add the new files to `src/<module>`
2. Add references to the new files in `src/<module>/Makefile.am` under
   target_SOURCES or target_HEADERS as appropriate.
3. Run `./bootstrap -w make` at the top-level.
4. Now build as needed.

-   [x] **Clean out entire source tree**

This will clear out all build artifacts, including those created by
`./bootstrap`.

```
make distclean
```

If the source has been pulled using `git clone` the following command can also
be used to clear out all build artifacts:

```
# To clean all intermediate build files from the tree
git clean -fdx
```

### Build iOS

Install XCode and XQuarz.

```
make -f Makefile-iOS
```

### Build Android

Install Android Studio, Java, and NDK.

```
# Update these paths based on your environment and version of the tools (MacOS examples):
export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk-12.0.1.jdk/Contents/Home
export ANDROID_HOME=~/Library/Android/sdk
export ANDROID_NDK_HOME=~/Library/Android/sdk/ndk/21.0.6113669

make -f Makefile-Android
```

## Maintaining CHIP

If you want to maintain, enhance, extend, or otherwise modify CHIP, it is likely
you will need to change its build system, based on GNU autotools, in some
circumstances.

After any change to the CHIP build system, including any _Makefile.am_ files or
the _configure.ac_ file, you must run the `bootstrap` or `bootstrap-configure`
(which runs both `bootstrap` and `configure` in one shot) script to update the
build system.

### Dependencies

Due to its leverage of GNU autotools, if you want to modify or otherwise
maintain the CHIP build system, the following additional packages are required
and are invoked by `bootstrap`:

-   autoconf
-   automake
-   libtool

Instructions for installing these tools is in the Tool Prerequisites section
above.
