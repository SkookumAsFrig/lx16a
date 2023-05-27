# lx16a
Controls and communication library in C++ for Lewansoul LX16-a serial bus servos

Repo contains and uses serial submodule http://wjwwood.github.io/serial/
Pull with: git clone --recurse-submodules

Also https://github.com/google/googletest for unit/integration testing, specifically v1.8.2

linux command line shortcuts (shell scripts must be executable (chmod +x configure.sh _build.sh run.sh))

./configure.sh
./_build.sh
./run.sh

Install prereqs on new Ubuntu machine: 

```
$ sudo sh \
    -c 'echo "deb http://packages.ros.org/ros/ubuntu `lsb_release -sc` main" \
        > /etc/apt/sources.list.d/ros-latest.list'
$ wget http://packages.ros.org/ros.key -O - | sudo apt-key add -

$ sudo apt-get update
$ sudo apt-get install python3-catkin-tools

$ # Or for 20.04 and up:
$ pip3 install -U catkin_tools

$ sudo apt-get install cmake python-catkin-pkg python-empy python-nose libgtest-dev
$ sudo apt-get install ros-melodic-catkin

$ source /opt/ros/melodic/setup.bash
```

Add sourcing catkin to bashrc for automatic upon new terminal session, change melodic to ROS version of choice
