#! /bin/sh
echo ">>Clear Log"
cat /dev/null > /var/log/syslog
echo ">>Make Clean"
make clean
echo ">>Make"
make
echo ">>rmmod"
sudo rmmod pcie.ko
echo ">>insmod"
sudo insmod pcie.ko

