#!/bin/sh

set -e

cmake --build build --parallel
sudo cmake --install build
systemctl --user restart plasma-krunner
