#!/bin/bash
git config --global user.email "peter@never.lan"
git config --global user.name "Peter Pan"
export HOST=i386-pc-linux
export TARGET=sslittle-na-sstrix

./configure --host=i386-pc-linux --target=sslittle-na-sstrix --with-gnu-as --with-gnu-ld --prefix=$HOME

sed -i -e "s/#if CLOCKS_PER_SEC <= 1000000/#define CLOCKS_PER_SEC_SUPPOSED ((clock)1000000)\n#if #CLOCKS_PER_SEC == #CLOCKS_PER_SEC_SUPPOSED\n#define CLOCKS_PER_SEC 1000000\n#endif\n#if CLOCKS_PER_SEC <= 1000000/g" libiberty/getruntime.c
sed -i -e "s/NEED_sys_errlist/NEED_sys_errPROTECTEDlist/g" libiberty/strerror.c
sed -i -e "s/sys_nerr/sys_nerr_2/g" libiberty/strerror.c
sed -i -e "s/sys_errlist/sys_errlist_2/g" libiberty/strerror.c
sed -i -e "s/NEED_sys_errPROTECTEDlist/NEED_sys_errlist/g" libiberty/strerror.c
sed -i -e "s/va_list ap = args;/va_list ap; va_copy(ap, args);/g" libiberty/vasprintf.c
sed -i -e "s/\/usr\/include\/sys\/errno.h/\/usr\/include\/x86_64-linux-gnu\/sys\/errno.h/g" gas/Makefile
sed -i -e "s/yy_current_buffer/YY_CURRENT_BUFFER/g" ld/ldlex.l
sed -i -e "s/varargs.h/stdarg.h/g" ld/ldmisc.c
sed -i -e "s/(va_alist)/()/g" ld/ldmisc.c
sed -i -e "s/ va_dcl/\/\/ va_dcl/g" ld/ldmisc.c
sed -i -e "s/ FILE \*fp;/\/\/ FILE \*fp;/g" ld/ldmisc.c
sed -i -e "s/ char \*fmt;/\/\/ char \*fmt;/g" ld/ldmisc.c
sed -i -e "s/vfinfo(fp, fmt, arg)/vfinfo(FILE \*fp, char \*fmt, va_list arg)/g" ld/ldmisc.c
sed -i -e "s/ va_list arg;/\/\/ va_list arg;/g" ld/ldmisc.c
sed -i -e "s/ va_start/\/\/ va_start/g" ld/ldmisc.c
sed -i -e "s/ file/\/\/ file/g" ld/ldmisc.c
sed -i -e "s/ fmt/\/\/ fmt/g" ld/ldmisc.c
sed -i -e "s/ vfinfo/\/\/ vfinfo/g" ld/ldmisc.c
sed -i -e "s/ va_end/\/\/ va_end/g" ld/ldmisc.c