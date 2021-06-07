# Ledger THORChain app
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![CodeFactor](https://www.codefactor.io/repository/github/thorchain/ledger-thorchain/badge)](https://www.codefactor.io/repository/github/thorchain/ledger-thorchain)

This project contains the THORChain app for Ledger Nano S and X.

- Ledger Nano S/X THORChain app
- Specs / Documentation
- C++ unit tests
- Zemu tests

## ATTENTION

Please:

- **Do not use in production**
- **Do not use a Ledger device with funds for development purposes.**
- **Have a separate and marked device that is used ONLY for development and testing**

## Download and install a prerelease

*Once the app is approved by Ledger, it will be available in their app store (Ledger Live).

User instructions in [docs/INSTRUCTIONS.md](docs/INSTRUCTIONS.md)

# Development

## Preconditions

- Be sure you checkout submodules:

    ```
    git submodule update --init --recursive
    ```

- We only officially support Ubuntu. Install the following packages:
   ```
   sudo apt update && apt-get -y install build-essential git wget cmake \
  libssl-dev libgmp-dev autoconf libtool
   ```

- Install Docker.
    ```bash
    docker pull zondax/builder-bolos:latest
    docker pull zondax/builder-zemu:latest
    ```

- (Optional: Manual builds) - Setup Ledger development environment using official docs:
    [https://ledger.readthedocs.io/en/latest/userspace/setup.html](https://ledger.readthedocs.io/en/latest/userspace/setup.html)


- You will need python 3 and then run
    - `make deps`

- This project requires **Ledger firmware 2.0**
    - The current repository keeps track of Ledger's SDK in `deps/` but it is possible to override it by changing the git submodule.

*Warning*: Some IDEs may not use the same python interpreter or virtual enviroment as the one you used when running `pip`.
If you see conan is not found, check that you installed the package in the same interpreter as the one that launches `cmake`.

## How to build ?

> We like clion or vscode but let's have some reproducible command line steps
>

- Building the app itself (Manual)

    If you installed the what is described above, just run:
    ```bash
    cd ledger-thorchain
    export PATH=/path/to/gcc-arm-none-eabi/bin:$PATH
    export BOLOS_SDK=/path/to/ledger-thorchain/deps/nanos-secure-sdk
    make
    make load
    ```

- Building with zondax docker images.
    The Makefile uses the zondas build system when `BOLOS_SDK` is undefined. Inspect `Makefile` for details.
    ```bash
    export BOLOS_SDK=
    make
    make load
    ```

## How to test with Zemu?

> What is Zemu??
> As part of the Cosmos project this forked from, Zondax made public a beta version of their internal testing+emulation framework for Ledger apps.
>
> Npm Package here: https://www.npmjs.com/package/@zondax/zemu
>
> Repo here: https://github.com/Zondax/zemu

Let's go! First install everything:
> At this moment, if you change the app you will need to run `make` before running the test again.

```bash
make zemu_install
make
make zemu
```

Then you can run JS tests:

```bash
make zemu_test
```

To run a single specific test:

> At the moment, the recommendation is to run from the IDE. Remember to run `make` if you change the app.

## How to debug a ledger app?

You can use vscode or clion to debug the app.

### Preconditions

If you are using CLion, you need to a configuration file in your home directory: `$HOME/.gdbinit` with the following content:

```
set auto-load local-gdbinit on
add-auto-load-safe-path /
```

### Warnings

There are a few things to take into account when enabling Ledger App debugging:

- Once you enable the local .gdbinit that is located in your project workspace. You **will break** local Rust debugging in your host. The reason is that debugging unit tests will use the same `.gdbinit` configuration that sets the environment to ARM. We are looking at some possible fixes. For now, if you want to debug unit tests instead of the ledger app, you need to comment out the lines in `.gdbinit`

### Debugging

1. Build your app

    ```bash
    make
    ```

2. Define your debug scenario

    Open `tests/zemu/tools/debug.mjs` and look for the line:

    ```bash
    /// TIP you can use zemu commands here to take the app ...
    ```

    You can adjust this code to get the emulator to trigger a breakpoint in your app:
    - send clicks
    - send APDUs, etc

3. Launch the emulator in debug mode

    > If you didnt install Zemu yet (previous section), then run `make zemu_install`

    ```bash
    make zemu_debug
    ```

    The emulator will launch and immediately stop. You should see a blank window

4. Configure Clion debugger

    Your configuration should look similar to this:

     ![image](docs/img/clion_debugging.png)

    Check that the path mappings are correct

5. Start CLion debugger

    You will hit a breakpoint in main.
    Add breakpoints in other places and continue.

    Enjoy :)

6. For VSCode, open `.vscode/launch.json` and change `gdbpath` to be correct for your `arm-none-eabi-gdb`.
    Whilst your `make zemu_debug` is running (waiting), press VSCode attach to gdbserver. Enjoy :)

## Using a real device

### How to prepare your DEVELOPMENT! device:

>  You can use an emulated device for development. This is only required if you are using a physical device
>
>    **Please do not use a Ledger device with funds for development purposes.**
>>
>    **Have a separate and marked device that is used ONLY for development and testing**

   There are a few additional steps that increase reproducibility and simplify development:

**1 - Ensure your device works in your OS**
- In Linux hosts it might be necessary to adjust udev rules, etc. See the Ledger manual dev environment setup (above) or https://support.ledger.com/hc/en-us/articles/115005165269-Fix-connection-issues

**2 - Set a test mnemonic**

Many of our integration tests expect the device to be configured with a known test mnemonic.

- Plug your device while pressing the right button

- Your device will show "Recovery" in the screen

- Double click

- Run `make dev_init`. This will take about 2 minutes. The device will be initialized to:

   ```
   PIN: 5555
   Mnemonic: equip will roof matter pink blind book anxiety banner elbow sun young
   ```

**3 - Add a development certificate**

- Plug your device while pressing the right button

- Your device will show "Recovery" in the screen

- Click both buttons at the same time

- Enter your pin if necessary

- Run `make dev_ca`. The device will receive a development certificate to avoid constant manual confirmations.


### Loading into your development device

The Makefile will build the firmware in a docker container and leave the binary in the correct directory.

- Build

   ```
   make                # Builds the app
   ```

- Upload to a device
   The following command will upload the application to the ledger. _Warning: The application will be deleted before uploading._
   ```
   make load          # Builds and loads the app to the device
   ```

## APDU Specifications

- [APDU Protocol](docs/APDUSPEC.md)
- [Transaction format](docs/TXSPEC.md)
