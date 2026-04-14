# Wombat-CC C++ Library Template

Minimal C++ library template for Wombat-CC projects.

## Remote usage

```sh
zig fetch --save=wombat_cc_lib_cpp_template https://github.com/cdenihan/Wombat-CC-CPP-Library-Template/archive/refs/heads/master.tar.gz
zig build
```

## Structure

- `build.zig` exports artifact `lib` and named lazy path `include`
- `include/` + `src/` provide your C++ API and implementation

## Adapting for your library

1. Rename package in `build.zig.zon`
2. Rename `LibraryTemplate.hpp/.cpp` and replace API
3. Keep artifact name `lib` for root auto-linker compatibility

## Consumer examples

### C / C++

```cpp
#include <LibraryTemplate.hpp>

int main() {
    const int sum = LibraryTemplate::Add(2, 3);
    (void)sum;
    return 0;
}
```

### Zig

Zig cannot directly use C++ classes. Add a C ABI shim in your library (for example `library_template_c_api.h/.cpp`) and call it via `@cImport`:

```zig
const lib = @cImport(@cInclude("library_template_c_api.h"));

pub fn main() void {
    const sum = lib.library_template_add(2, 3);
    _ = sum;
}
```
