bcc -c t.c
as86 -o ts.o ts.s
ld86 -d ts.o t.o mtxlib /usr/lib/bcc/libc.a

mount -o loop mtximage.bin fd0 
cp a.out fd0/boot/mtx
sleep 1
umount fd0

rm *.o a.out
echo done mtx
