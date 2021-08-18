echo Configuring GCC for Windows NT on Intel x86
rem This batch file assumes a unix-type "sed" program

copy config\i386\xm-winnt.h config.h
copy config\i386\xm-winnt.h hconfig.h
copy config\i386\xm-winnt.h tconfig.h
copy config\i386\winnt.h tm.h
copy config\i386\i386.md md
copy config\i386\i386.c aux-output.c

rem This batch file assumes a unix-type "sed" program

echo # Makefile generated by "config-nt.bat"> Makefile
echo all.nt: cccp.exe cc1.exe xgcc.exe ld-winnt.exe>> Makefile
sed -f config/i386/config-nt.sed Makefile.in >> Makefile

set LANG=

if not exist cp\make-lang.in goto no_cp
sed -f config/i386/config-nt.sed cp\make-lang.in >> Makefile
sed -f config/i386/config-nt.sed cp\makefile.in > cp\Makefile
set LANG=%LANG% c++.&	
:no_cp

echo lang.mostlyclean: %LANG% | sed "s/&/mostlyclean/g" >> Makefile
echo lang.clean: %LANG% | sed "s/&/clean/g" >> Makefile
echo lang.distclean: %LANG% | sed "s/&/distclean/g" >> Makefile
echo lang.realclean: %LANG% | sed "s/&/realclean/g" >> Makefile

echo #define MULTILIB_SELECT ". ;" > multilib.h1
copy multilib.h1 multilib.h
del multilib.h1
