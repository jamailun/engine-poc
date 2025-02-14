
# Engine PoC

## Build

### Requirements

```bash
sudo apt-get install libsdl2-ttf-dev
# install SDL
```

### Create the build directories

```bash
meson setup build_debug --buildtype=debug -Db_sanitize=address,undefined
meson setup build_release --buildtype=release
```

### Compile

```bash
cd build_debug
ninja
```

### Run (debug)

```bash
cd build_debug
clear && ninja && LSAN_OPTIONS=suppressions=${PWD}/../asan_supp.txt ./app/engine-test -a ${PWD}/../assets
```
