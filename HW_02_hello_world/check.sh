sudo rmmod hello_mod
make clean
bear -- make
sudo insmod hello_mod.ko

cat /sys/module/hello_mod/parameters/my_str 

sudo sh -c 'echo 5 > /sys/module/hello_mod/parameters/idx'
sudo sh -c 'echo . > /sys/module/hello_mod/parameters/ch_val'
cat /sys/module/hello_mod/parameters/my_str 

sudo sh -c 'echo 0 > /sys/module/hello_mod/parameters/idx'
sudo sh -c 'echo h > /sys/module/hello_mod/parameters/ch_val'
cat /sys/module/hello_mod/parameters/my_str 

