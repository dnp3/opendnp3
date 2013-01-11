#put all of boost specific defs here to make it easier to upgrade

URL=http://sourceforge.net/projects/boost/files/boost/1.46.1/boost_1_46_1.tar.bz2/download
BOOST_DIR=boost_1_46_1
TARBALL=$BOOST_DIR.tar.bz2

TEMP_DIR=$PWD/temp
DOWNLOAD=$TEMP_DIR/$TARBALL
INSTALL_DIR=$TOOLS_HOME/boostlib/$BOOST_DIR

echo "Temporary files will be written to $TEMP_DIR"
echo "Boost will be installed to $INSTALL_DIR"

if [ "`uname -a | grep CYGWIN_NT`" ]; then
  echo "detected cygwin..."
  PLATFORM=pc_cygwin
  CROSSTOOL=/opt/crosstool/gcc-3.3.4-glibc-2.3.2/arm-unknown-linux-gnu/bin/arm-unknown-linux-gnu-g++
else
  DO_LD_CONFIG=true
  PLATFORM=Linux_i686
  CROSSTOOL=/usr/local/opt/crosstool/arm-linux/gcc-3.3.4-glibc-2.3.2/bin/arm-linux-g++
fi

if [ ! $TOOLS_HOME ]; then
  echo "TOOLS_HOME environment variable not defined!"
  exit -1
else
  echo "TOOLS_HOME set to $TOOLS_HOME"
fi

if [ ! -e $DOWNLOAD ]; then
  echo "Downloading ${TARBALL} to ${TEMP_DIR}"
  mkdir $TEMP_DIR
  wget -P $TEMP_DIR $URL -O $DOWNLOAD
else
  echo "${TARBALL} already downloaded to ${TEMP_DIR}"
fi

cd $TEMP_DIR

if [ ! -d $BOOST_DIR ]; then
  echo "Extracting ${TARBALL} to ${BOOST_DIR}"
  tar -xvf $TARBALL
else
  echo "${TARBALL} already extracted to ${BOOST_DIR}"
fi

echo "Running Boost boostrap.sh script"
cd $BOOST_DIR
./bootstrap.sh
#echo "using gcc : arm : $CROSSTOOL ;" >> project-config.jam
#echo "using gcc : arm : $CROSSTOOL ;" >> project-config.jam

echo "Running bjam"
./bjam toolset=gcc cxxflags=-fPIC --with-program_options --with-system --with-date_time --with-thread --with-filesystem --with-test --layout=system stage

echo "Creating directory: ${INSTALL_DIR}/${PLATFORM}"
mkdir -p $INSTALL_DIR/$PLATFORM/

echo "Moving Boost libraries to ${INSTALL_DIR}/${PLATFORM}"
mv stage/lib/* $INSTALL_DIR/$PLATFORM/

#./bjam toolset=gcc-arm target-os=linux --with-program_options --with-system --with-date_time --with-thread --with-filesystem --with-test --layout=system stage
#mkdir -p $INSTALL_DIR/pc_linux_arm
#mv stage/lib/* $INSTALL_DIR/pc_linux_arm/

echo "Creating directory: ${INSTALL_DIR}/include"
mkdir -p $INSTALL_DIR/include

echo "Copying ${BOOST_DIR}/boost to ${INSTALL_DIR}/include"
cp -R boost $INSTALL_DIR/include

if [ $DO_LD_CONFIG ]
then
  echo "Running ldconfig"
  sudo ldconfig $INSTALL_DIR/$PLATFORM
else
  echo "Not running ldconfig"
fi
