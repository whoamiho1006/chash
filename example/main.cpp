#include <stdio.h>

#ifdef _MSC_VER
#pragma comment(lib, "libchash.lib")
#endif

int md4();
int md5();
int ripemd160();
int sha256();
int crc16();
int crc32();
int crc64();
int hmac_md5();
int hmac_sha256();
int hmac_sha384();
int hmac_sha512();
int hmac_ripemd160();
int pbkdf2_hmac_sha256();

int main() {
	md4();
	md5();
	ripemd160();
	sha256();
	crc16();
	crc32();
	crc64();
	hmac_md5();
	hmac_sha256();
	hmac_sha384();
	hmac_sha512();
	hmac_ripemd160();
	pbkdf2_hmac_sha256();
}