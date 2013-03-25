#! /bin/sh
echo Make Clean
make clean
echo Make
make
echo rmmod
sudo rmmod pcie.ko
echo insmod
sudo insmod pcie.ko

