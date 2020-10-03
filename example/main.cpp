#include <stdio.h>

#ifdef _MSC_VER
#pragma comment(lib, "libchash.lib")
#endif

int md4();
int md5();
int ripemd160();
int sha256();

int main() {
	md4();
	md5();
	ripemd160();
	sha256();
}