
# Engine PoC

## Build

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
