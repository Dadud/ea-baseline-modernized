# Legacy Wide-Character Compatibility

The EA Renegade baseline was authored for Win32/MSVC. In that environment:

```text
WCHAR == 16-bit Windows wide character
wchar_t is also 16-bit under MSVC
```

On common Linux toolchains:

```text
wchar_t == 32-bit
```

This difference matters because some foundation code does more than display text: it serializes and deserializes wide strings through bit streams and chunk IO. Treating `WCHAR` as a casual host `wchar_t` alias everywhere can silently change binary layout and compatibility-sensitive behavior.

## Current scaffold policy

During the conservative EA-baseline scaffold phase:

1. Preserve original project/source membership.
2. Avoid broad platform/string rewrites.
3. Fix only the immediate compile boundary being exercised.
4. Document every compatibility bridge that might need a final architecture decision later.

## First applied fix: `wwbitpack`

`BitStreamClass` already had overloads for fixed-width Win32-style scalar types such as:

```text
BYTE
USHORT
UINT
ULONG
```

The non-Windows scaffold currently maps `WCHAR` to host `wchar_t` so the existing wide-string helper code can call standard C wide-string functions. That made `WCHAR` distinct from `USHORT`, which exposed this compile blocker:

```text
BitStreamClass::Get_Wide_Terminated_String(WCHAR*, ...)
Get(temp) cannot bind WCHAR/wchar_t to int&
```

The narrow fix adds non-Windows `BitStreamClass` overloads for `WCHAR` that serialize through `USHORT`:

```text
Add(WCHAR) -> Add as USHORT
Get(WCHAR&) -> Get USHORT, cast back to WCHAR
```

This preserves the Win32-era 16-bit stream width for the `wwbitpack` path while avoiding a global wide-string redesign.

## Verification

Command:

```bash
cmake --build build/cmake-scaffold --target wwbitpack -j4
```

Result:

```text
[100%] Built target wwbitpack
```

Remaining warnings are legacy MSVC-era syntax/template lookup warnings currently tolerated by the scaffold's temporary GNU/Clang `-fpermissive` setting.

## Follow-up design work

This is not the final wide-character architecture. Before broader product builds or binary compatibility claims, decide where the final type boundary lives:

- host UI/API text type
- serialized 16-bit wide text type
- conversion helpers between the two
- chunk/bitstream persistence rules

Likely future shape: a small platform/foundation compatibility layer with explicit 16-bit serialized text helpers, rather than scattered `WCHAR` assumptions in product or engine code.
