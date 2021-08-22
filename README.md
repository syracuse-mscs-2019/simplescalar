### Branches
- Main: [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/syracuse-mscs-2019/simplescalar/master?urlpath=lab)
- Ifiht: [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/syracuse-mscs-2019/simplescalar/ifiht?urlpath=lab)
- shut: [![Binder](https://mybinder.org/badge_logo.svg)](https://mybinder.org/v2/gh/syracuse-mscs-2019/simplescalar/ifiht?urlpath=lab)

# simplescalar

To edit in GitHub directly, press "." while at this page.

https://github.com/sdenel/How-to-install-SimpleScalar-on-Ubuntu/blob/master/install_simple_scalar.sh

# Steps Taken 
### (plz update with any committed changes!!!)

#### Download & Extract
1. `wget http://www.simplescalar.com/agreement.php3?simplesim-3v0e.tgz`
2. `wget http://www.simplescalar.com/downloads/simpletools-2v0.tgz`
3. `wget http://www.simplescalar.com/downloads/simpleutils-2v0.tgz`
4. `tar -xzf simplesim-3v0e.tgz`
5. `tar -xzf simpletools-2v0.tgz`
6. `tar -xzf simpleutils-2v0.tgz`

#### Build GCC
1. cd gcc-2.6.3
2. ./configure --host=i386-pc-linux
3. make

#### Build SimpleScalar Utils
1. `cd binutils-2.5.2`
2. `./configure --host=i386-pc-linux --target=sslittle-na-sstrix --with-gnu-as --with-gnu-ld --prefix=$HOME`
3. `sed -i -e "s/#if CLOCKS_PER_SEC <= 1000000/#define CLOCKS_PER_SEC_SUPPOSED ((clock)1000000)\n#if #CLOCKS_PER_SEC == #CLOCKS_PER_SEC_SUPPOSED\n#define CLOCKS_PER_SEC 1000000\n#endif\n#if CLOCKS_PER_SEC <= 1000000/g" libiberty/getruntime.c`
4. `sed -i -e "s/NEED_sys_errlist/NEED_sys_errPROTECTEDlist/g" libiberty/strerror.c`
5. `sed -i -e "s/sys_nerr/sys_nerr_2/g" libiberty/strerror.c`
6. `sed -i -e "s/sys_errlist/sys_errlist_2/g" libiberty/strerror.c`
7. `sed -i -e "s/NEED_sys_errPROTECTEDlist/NEED_sys_errlist/g" libiberty/strerror.c`

#### Test
1. `cp -r sslittle-na-sstrix/* glibc-1.09/`
2. `./simplesim-3.0/sim-outorder tests/bin/test-math`
