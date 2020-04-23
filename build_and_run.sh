make clean
make
sudo insmod bad_file.ko || true
sudo dmesg | tail -n10 | grep "SAID::"
