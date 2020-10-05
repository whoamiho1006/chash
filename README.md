# chash
Hash algorithm library in C++.

## examples
### CRC64
This implements CRC-64/ECMA182. (Init: 0x0000000000000000, Poly: 0x42f0e1eba9ea3693)
```
if (auto CRC64 = create(algorithm::CRC64)) {
	digest_t Digest;

	if (CRC64->init()) {
		CRC64->update((uint8_t*)"abcd", 4);
		CRC64->finalize(Digest);

		printf("CRC64(abcd): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: d5fbf74d5ddc3826
// https://crc64.online/ (ECMA 182, 'abcd'): d5fbf74d5ddc3826

### CRC32
This implements CRC-32/BZIP2. (Init: 0xFFFFFFFF, Poly: 0x04C11DB7)
```
if (auto CRC32 = create(algorithm::CRC32)) {
	digest_t Digest;

	if (CRC32->init()) {
		CRC32->update((uint8_t*)"abcd", 4);
		CRC32->finalize(Digest);

		printf("CRC32(abcd): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: 3D4C334B
// https://crccalc.com/ (CRC-16/BUYPASS, 'abcd'): 3D4C334B
```


### CRC16
This implements CRC-16/BUYPASS. (Init: 0x0000, Poly: 0x8000)
```
if (auto CRC16 = create(algorithm::CRC16)) {
	digest_t Digest;

	if (CRC16->init()) {
		CRC16->update((uint8_t*)"abcd", 4);
		CRC16->finalize(Digest);

		printf("CRC16(abcd): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: 58e7
// https://crccalc.com/ (CRC-16/BUYPASS, 'abcd'): 58e7
```

### SHA-512
```
if (auto SHA512 = chash::create(algorithm::SHA512)) {
	chash::digest_t Digest;

	if (SHA512->init()) {
		SHA512->update((uint8_t*)"abcd", 4);
		SHA512->finalize(Digest);

		printf("%s\n", toHex(Digest).c_str());
	}
}

// output: d8022f2060ad6efd297ab73dcc5355c9b214054b0d1776a136a669d26a7d3b14f73aa0d0ebff19ee333368f0164b6419a96da49e3e481753e7e96b716bdccb6f
// php's hash('sha512', 'abcd'): d8022f2060ad6efd297ab73dcc5355c9b214054b0d1776a136a669d26a7d3b14f73aa0d0ebff19ee333368f0164b6419a96da49e3e481753e7e96b716bdccb6f
```

### SHA-384
```
if (auto SHA384 = chash::create(algorithm::SHA384)) {
	chash::digest_t Digest;

	if (SHA384->init()) {
		SHA384->update((uint8_t*)"abcd", 4);
		SHA384->finalize(Digest);

		printf("%s\n", toHex(Digest).c_str());
	}
}

// output: 1165b3406ff0b52a3d24721f785462ca2276c9f454a116c2b2ba20171a7905ea5a026682eb659c4d5f115c363aa3c79b
// php's hash('sha384', 'abcd'): 1165b3406ff0b52a3d24721f785462ca2276c9f454a116c2b2ba20171a7905ea5a026682eb659c4d5f115c363aa3c79b
```

### SHA-256
```
if (auto SHA256 = chash::create(algorithm::SHA256)) {
	chash::digest_t Digest;

	if (SHA256->init()) {
		SHA256->update((uint8_t*)"abcd", 4);
		SHA256->finalize(Digest);
		
		printf("%s\n", toHex(Digest).c_str());
	}
}

// output: 88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589
// php's hash('sha256', 'abcd'): 88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589
```

### MD5
```
if (auto MD5 = chash::create(algorithm::MD5)) {
	chash::digest_t Digest;

	if (MD5->init()) {
		MD5->update((uint8_t*)"abcd", 4);
		MD5->finalize(Digest);
		
		printf("%s\n", toHex(Digest).c_str());
	}
}

// output: e2fc714c4727ee9395f324cd2e7f331f
// php's md5('abcd'): e2fc714c4727ee9395f324cd2e7f331f
```

### MD4
```
if (auto MD4 = chash::create(algorithm::MD4)) {
	chash::digest_t Digest;

	if (MD4->init()) {
		MD4->update((uint8_t*)"abcd", 4);
		MD4->finalize(Digest);
		
		printf("%s\n", toHex(Digest).c_str());
	}
}

// output: 41decd8f579255c5200f86a4bb3ba740
// php's md5('abcd'): 41decd8f579255c5200f86a4bb3ba740
```

### RipeMD128
```
if (auto RipeMD128 = chash::create(algorithm::RipeMD128)) {
	chash::digest_t Digest;

	if (RipeMD128->init()) {
		RipeMD128->update((uint8_t*)"abcd", 4);
		RipeMD128->finalize(Digest);
		
		printf("%s\n", toHex(Digest).c_str());
	}
}

// output: 47696d5295eabc4aa314c15fc17553ff
// php's hash('ripemd160', 'abcd'): 47696d5295eabc4aa314c15fc17553ff
```

### RipeMD160
```
if (auto RipeMD160 = chash::create(algorithm::RipeMD160)) {
	chash::digest_t Digest;

	if (RipeMD160->init()) {
		RipeMD160->update((uint8_t*)"abcd", 4);
		RipeMD160->finalize(Digest);
		
		printf("%s\n", toHex(Digest).c_str());
	}
}

// output: 2e7e536fd487deaa943fda5522d917bdb9011b7a
// php's hash('ripemd160', 'abcd'): 2e7e536fd487deaa943fda5522d917bdb9011b7a
```
