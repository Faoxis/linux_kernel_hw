#scp -P 2222 -r faoxis@localhost:/home/faoxis/linux-6.1.130 ./
docker rmi kernel-builder
docker build -t kernel-builder .
docker run --rm -ti -v $PWD/linux-6.1.130:/usr/src/linux -v $PWD/out:/out kernel-builder
scp -P 2222 ./out/*.deb faoxis@localhost:/home/faoxis/newkernel/
ssh -p 2222 -T faoxis@localhost '
  sudo dpkg -r linux-headers-6.1.130 || true
  sudo rm -rf /usr/src/linux-headers-6.1.130
  sudo dpkg -i /home/faoxis/newkernel/*.deb &&
  sudo update-grub &&
  sudo reboot
'