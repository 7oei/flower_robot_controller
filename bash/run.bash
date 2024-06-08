#!/bin/bash
sudo chmod 666 /dev/ttyACM0
ypspur-coordinator -p /home/adachi/ros1_ws/src/flower_robot_controller/param/speego.param -d /dev/ttyACM0