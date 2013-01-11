
if [ "`uname -a | grep CYGWIN_NT`" ]
then
  echo "detected cygwin..."
  if [ ! -e crosstool-cygwin-gcc-3.3.4-glibc-2.3.2.tar.bz2 ]; then
    wget ftp://ftp.embeddedarm.com/ts-arm-sbc/ts-7200-linux/cross-toolchains/crosstool-cygwin-gcc-3.3.4-glibc-2.3.2.tar.bz2
  fi
  tar -xjvf crosstool-cygwin-gcc-3.3.4-glibc-2.3.2.tar.bz2 -C /
else
  if [ ! -e crosstool-linux-gcc-3.3.4-glibc-2.3.2-0.28rc39.tar.bz2 ]; then
    wget ftp://ftp.embeddedarm.com/ts-arm-sbc/ts-7200-linux/cross-toolchains/crosstool-linux-gcc-3.3.4-glibc-2.3.2-0.28rc39.tar.bz2
  fi
  sudo tar -xjvf crosstool-linux-gcc-3.3.4-glibc-2.3.2-0.28rc39.tar.bz2 -C /
fi
