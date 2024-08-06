# Building the cross-compiler
I build the cross compiler using the resources in provided in the [OSDevWiki](https://wiki.osdev.org/GCC_Cross-Compiler). Important is that the version of GCC matches the version of GCC installed on your system.
For my run I used:
- GCC version 14.1.1 (host-system Fedora 41rc)
- GCC cross compiler 14.1
- Binutils cross compiler 2.42

Check the table on [OSDevWiki](https://wiki.osdev.org/Cross-Compiler_Successful_Builds) for matching versions of Binutils and GCC.

Before Build:
Update your system to prevent weird errors.

Unzip:
```
tar -xvf binutils*.tar.xz
tar -xvf gcc*.tar.xz
tar -xvf gdb*.tar.xz
```

GCC contrib:
```
cd gcc*
contrib/download_prerequisites
```

First compile Binutils:
```
mkdir aarch64-binutils
../binutils-*/configure --prefix=/usr/local/cross-compiler --target=aarch64-elf --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
```

Then compile GCC:
```
mkdir aarch64-gcc
../gcc-*/configure --prefix=/usr/local/cross-compiler --target=aarch64-elf --disable-nls --enable-languages=c,c++ --without-headers
make -j$(nproc) all-gcc
make -j$(nproc) all-target-libgcc
make install-gcc
make install-target-libgcc
```

# Problems
- Be very careful with matching versions and research for valid combinations of Binutils/GCC for the cross-compiler and watch out that you GCC version matches the GCC version you want to compile. If you encounter errors with the some libraies for example missing headers probably you have some issues with matching versions.
- Another error I encountered is libgcc not found. That error occurs when you use the precompiled `aarch64-elf-gcc` of Ubuntu without libgcc.
