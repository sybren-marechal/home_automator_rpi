
To obtain WiringPi using GIT:
$ cd
$ git clone git://git.drogon.net/wiringPi

To build/install there is a new simplified script:
$ cd ~/wiringPi
$ ./build

The new build script will compile and install it all for you – it does use the sudo command at one point, so you may wish to inspect the script before running it.


NOTE: To compile programs with wiringPi, you need to add:
    -lwiringPi
  to your compile line(s) To use the Gertboard, MaxDetect, etc.
  code (the devLib), you need to also add:
    -lwiringPiDev
  to your compile line(s).


run the gpio command to check the installation:

$ gpio -v
$ gpio readall
