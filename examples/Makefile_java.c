gcc -I. -I.. -I..  -I/usr/lib/j2se/1.6/include -I/usr/lib/j2se/1.6/include/linux -I/usr/local/nss/include -I/usr/local/include/nspr -I/usr/local/ImageMagick/include/ImageMagick -I/usr/local/include -std=gnu99 -shared -Wall -Wundef -Wunused -Wstrict-prototypes -Werror-implicit-function-declaration -Wno-pointer-sign -Wshadow -g -O2 -o libenroll_java.so enroll_java.c -L/usr/local/nss/mozilla/dist/Linux2.6_x86_glibc_PTH_DBG.OBJ/lib -L/usr/local/glib_228/lib -L/usr/local/ImageMagick/lib -L/usr/local/lib -L/usr/lib /usr/local/libfprint/lib/libfprint.so  -pthread -Wl,-rpath  -Wl,/usr/local/libfprint/lib

gcc -I. -I.. -I..  -I/usr/lib/j2se/1.6/include -I/usr/lib/j2se/1.6/include/linux -I/usr/local/nss/include -I/usr/local/include/nspr -I/usr/local/ImageMagick/include/ImageMagick -I/usr/local/include -std=gnu99 -shared -Wall -Wundef -Wunused -Wstrict-prototypes -Werror-implicit-function-declaration -Wno-pointer-sign -Wshadow -g -O2 -o libverify_java.so verify_java.c -L/usr/local/nss/mozilla/dist/Linux2.6_x86_glibc_PTH_DBG.OBJ/lib -L/usr/local/glib_228/lib -L/usr/local/ImageMagick/lib -L/usr/local/lib -L/usr/lib /usr/local/libfprint/lib/libfprint.so  -pthread -Wl,-rpath  -Wl,/usr/local/libfprint/lib
