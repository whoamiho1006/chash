# chash
Hash algorithm library in C++.

## examples
### CRC16/32/64
Warning: Currently WORK in PROGRESS.

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
