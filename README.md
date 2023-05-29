# lx16a
Controls and communication library in C++ for Lewansoul LX16-a serial bus servos

Repo contains and uses serial submodule http://wjwwood.github.io/serial/
Pull with: git clone --recurse-submodules

Also https://github.com/google/googletest for unit/integration testing, specifically v1.12.1

```
$ cd lx16a/ext/googletest
$ git checkout release-1.12.1
```

linux command line shortcuts (shell scripts must be executable (chmod +x configure.sh _build.sh run.sh))

./configure.sh
./_build.sh
./run.sh

Install prereqs on new Ubuntu machine: 

```
# my fork of serial no longer needs python packages and catkin, which were depreciated for Ubuntu 20.04 Jammy
sudo apt install libboost-all-dev cmake
```
