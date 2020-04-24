make clean
make
sudo insmod bad_file.ko || true
sudo dmesg | grep "SAID::" | tail -n1
