
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
