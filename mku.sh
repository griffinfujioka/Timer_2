#Usage : mku filename

#A=/root/dosemu/a

bcc -c $1.c
as86 -o u.o u.s
ld86 u.o $1.o /usr/lib/bcc/libc.a

#mount /dev/fd0 /fd0
#cp a.out /fd0/$1
#cp a.out /fd0/bin/$1
#umount /dev/fd0

mount -o loop mtximage.bin fd0
cp a.out fd0/$1
cp a.out fd0/bin/$1
sleep 1 	# Sleep for a second so fd0 will not be busy and is able to umount 
sudo umount fd0

rm *.o a.out

echo done $1
