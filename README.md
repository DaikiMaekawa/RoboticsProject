RoboticsProject
==========

千葉工業大学　中島研究室　ロボティクスプロジェクト

--------------------------------------------------
                                                       ロボティクスプロジェクトとは？
                                                       
BeagleBoard上でSPPboardとXtionを操作するためのPythonAPIを提供することを目的としたプロジェクトである。
 
                                                       　　　提供する機能の一覧
 - DCモータ×2のPWM制御及び回転数のカウント
 - XBeeを用いたPCとのシリアル通信
 - LCDに任意の文字列を表示
 - AD変換によるセンサ値の取得×３
 - サーボモータの角度制御
 - IMU3000を用いた3軸の姿勢検出(I2Cインタフェースをサポート)
 - ユーザの姿勢検出
 - モーション認識
 - 日本語音声認識
 
                                                                  動作環境



#### ハードウェア

 - beagleboard-xm(ubuntu11.10 ARM Coretex-A8)

 - sppboard

 - Xtion PRO LIVE

#### ソフトウェア

 - ROS Groovy

 - julius-4.2.2

 - Qt4.8.1
 
 - boost1.46.1

 - OpenNI1.5.4.0

 - NiTE1.5.0.1

 - Sensor5.1.2.1

 - OpenCV2.3.1

 - ffmpeg0.8.10

 - gtk2.0

 - x264

 - gstreamer0.10

 - SDL1.2

 - v4l-0.8.5

 - SWIG2.0

 - Python2.7

 - CMake2.8.7

 - ICU4.8.1.1-3

 - PulseAudio1.1

 - zlib1.2.3.4

 - flex2.5.35

 - esd0.2.41

 - sndfile1.0.25-4

#### コンパイラ

 - gcc4.6.3

 - MPLAB C30 C Compiler v3.23

                                                               インストール手順

git clone https://github.com/DaikiMaekawa/RoboticsProject

cd RoboticsProject

catkin_make install

免責事項
=====

このプログラムによって発生した、いかなる障害・損害も作成者は一切責任を負わないものとする。
---------------------------------------------------
