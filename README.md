# Hello World Raspberry Pi qemu
It pretends to show a png image using DispmanX API on the Raspberry Pi.  It is a fork from raspidmx. https://github.com/AndrewFromMelbourne/raspidmx 

### MIT License

## 1. clone repository
```bash
git clone https://github.com/brunoeee/hello-world-raspberry-pi-qemu.git

cd hello-world-raspberry-pi-qemu
```

## 2. download an image
I download an image from [here](https://www.kisspng.com/png-linux-logo-png-62718/) and copy to app/pngview with the name linux.png.

## 3. install qemu and unzip
sudo apt install qemu-system unzip

## 4. debian image and kernel
kernel from : https://github.com/dhruvvyas90/qemu-rpi-kernel
```bash
mkdir rpi

wget http://director.downloads.raspberrypi.org/raspbian_lite/images/raspbian_lite-2019-07-12/2019-07-10-raspbian-buster-lite.zip
unzip -d rpi 2019-07-10-raspbian-buster-lite.zip

wget -P rpi https://github.com/dhruvvyas90/qemu-rpi-kernel/raw/master/versatile-pb.dtb

wget -P rpi https://github.com/dhruvvyas90/qemu-rpi-kernel/raw/master/kernel-qemu-4.14.79-stretch

```

## 5. download dependencies
```bash
wget -P app/common -i requirements.txt

wget -P app/lib https://github.com/AndrewFromMelbourne/raspidmx/raw/master/lib/Makefile
```

## 6. running Raspberry Pi
```bash
qemu-system-arm \
   -kernel rpi/kernel-qemu-4.14.79-stretch \
   -dtb rpi/versatile-pb.dtb \
   -m 256 -M versatilepb -cpu arm1176 \
   -serial stdio \
   -append "rw console=ttyAMA0 root=/dev/sda2 rootfstype=ext4  loglevel=8 rootwait fsck.repair=yes memtest=1" \
   -drive file=rpi/2019-07-10-raspbian-buster-lite.img,format=raw \
   -net user,hostfwd=tcp::5022-:22,vlan=0 \
   -net nic,vlan=0 \
   -no-reboot
```

## 7. on Raspberry Pi terminal - enable ssh
```bash
raspberrypi login: pi
Password: raspberry 
sudo systemctl enable ssh
sudo systemctl start ssh
```

## 8. copy files to Raspberry Pi
```bash
scp -r -P 5022 ./app/* pi@localhost:/home/pi
```

## 9. on Raspberry Pi terminal - compile
```bash
cd /home/pi/lib
make
sudo mv libraspidmx.so.1 /usr/lib/libraspidmx.so.1

cd /home/pi/pngview
make
./pngview linux.png

```

## Result
```bash
*failed to open vchiq =/
```
see: [QEMU does not emulate the Videocore hardware](https://raspberrypi.stackexchange.com/questions/23127/failed-to-open-vchiq-instance-rpi-on-qemu-linux-kernel-mac)

