make format
make unload
make clean
bear -- make
make load

sudo insmod build/ex_list.ko
sudo insmod build/ex_queue.ko

sudo python3 tests/test_list.py
sudo python3 tests/test_queue.py

make unload
