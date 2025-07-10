make format
make unload
make clean
bear -- make
make load

# sudo insmod build/ex_list.ko
# sudo insmod build/ex_queue.ko
# sudo insmod build/ex_bitmap.ko
# sudo insmod build/ex_rb_tree.ko
# sudo insmod build/ex_bin_search.ko

sudo python3 tests/test_list.py
sudo python3 tests/test_queue.py
sudo python3 tests/test_bitmap.py
sudo python3 tests/test_rb_tree.py

make unload
