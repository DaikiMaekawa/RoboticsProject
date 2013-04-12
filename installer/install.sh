#! /bin/sh

workspace=${PWD}

if test $# -ne 1 ; then
    echo "Error: One argument is required"
    exit 1
fi

tar xvf julius-4.2.2.tar.gz
cd julius-4.2.2
./configure
make
sudo make install

sudo aptitude install libboost1.46-all-dev swig2.0

cd ${workspace}
tar -xvf NITE-Bin-Linux-x86-v1.5.2.21.tar.bz2
tar -xvf OpenNI-Bin-Dev-Linux-x86-v1.5.4.0.tar.bz2
tar -xvf Sensor-Bin-Linux-x86-v5.1.2.1.tar.bz2
cd OpenNI-Bin-Dev-Linux-x86-v1.5.4.0
sudo ./install.sh
cd ../Sensor-Bin-Linux-x86-v5.1.2.1
sudo ./install.sh
cd ../NITE-Bin-Dev-Linux-x86-v1.5.2.21
sudo ./install.sh

cd ${workspace}
sudo apt-get remove ffmpeg x264 libx264-dev
sudo apt-get install build-essential libtool manpages-dev libtool-doc cmake  checkinstall cmake libfaac-dev libjack-jackd2-dev libmp3lame-dev libopencore-amrnb-dev libvdpau-dev libvorbis-dev libx11-dev libxfixes-dev libxvidcore-dev texi2html yasm zlib1g-dev 

sudo apt-get install libgstreamer0.10-0 libgstreamer0.10-dev gstreamer0.10-tools gstreamer0.10-plugins-base libgstreamer-plugins-base0.10-dev gstreamer0.10-plugins-good gstreamer0.10-plugins-ugly gstreamer0.10-plugins-bad gstreamer0.10-ffmpeg

git clone git://git.videolan.org/x264
cd x264
./configure -enable-static -enable-shared
make
sudo make install

cd ${workspace}
wget http://ffmpeg.org/releases/ffmpeg-0.8.10.tar.bz2
tar xvf ffmpeg-0.8.10.tar.bz2
cd ffmpeg-0.8.10
./configure --enable-gpl --enable-libfaac --enable-libmp3lame --enable-libopencore-amrnb --enable-libopencore-amrwb --enable-libtheora --enable-libvorbis --enable-libx264 --enable-libxvid --enable-nonfree --enable-postproc --enable-version3 --enable-x11grab --enable-shared
make
sudo make install

cd ${workspace}
sudo apt-get install libgtk2.0-0 libgtk2.0-dev libjpeg62 libjpeg62-dev
wget http://www.linuxtv.org/downloads/v4l-utils/v4l-utils-0.8.5.tar.bz2
tar xvf v4l-utils-0.8.5.tar.bz2
cd v4l-utils-0.8.5
make
sudo make install

cd ${workspace}
wget http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.3.1/OpenCV-2.3.1a.tar.bz2
tar xvf OpenCV-2.3.1a.tar.bz2
cd OpenCV-2.3.1
mkdir build
cd build
cmake ..
make
sudo make install

cd ${workspace}
export LD_LIBRARY_PATH=/usr/local/lib
sudo cp /etc/ld.so.conf.d/opencv.conf /etc/ld.so.conf.d/opencv.conf.backup
sudo cat /etc/ld.so.conf.d/opencv.conf.backup | sudo sed -e '$s/$/ \n\/usr\/local\/lib/g' > opencv.conf

PKG_CONFIG_PATH=$PKD_CONFIG_PATH:/usr/local/lib/pkgconfig
export PKG_CONFIG_PATH
export LD_LIBRARY_PATH=/usr/local/lib

sudo shutdown -r now

