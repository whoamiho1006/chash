# chash
Hash algorithm library in C++.

![C/C++ CI](https://github.com/whoamiho1006/chash/workflows/C/C++%20CI/badge.svg)

## code reviews
1. https://codereview.stackexchange.com/questions/250149/simple-c-hash-function-library
2. https://codereview.stackexchange.com/questions/250172/simple-c-hash-library-and-its-abstraction-design-that-seems-out-of-focus

Thank you so much, G. Sliepen!

## examples
### PBKDF2 HMAC SHA-256
This implements PBKDF2 HMAC SHA-256.
```
pbkdf2_function pbkdf2(create_hmac(algorithm::SHA256));
digest_t gen_key;

pbkdf2.gen(gen_key,
	(uint8_t*)"1234", 4,
	(uint8_t*)"salt", 4,
	4, 32);

printf("PBKDF2 HMAC SHA256(1234, salt, 32): %s\n", to_hex(gen_key).c_str());

// output: d7da31cabfe509d02320eae72af619a2ee3a5dc19f02cfe53d8e200bd49f81ef
// php implementation: d7da31cabfe509d02320eae72af619a2ee3a5dc19f02cfe53d8e200bd49f81ef
```

Below is test script in PHP.
```
<?php
// https://www.php.net/manual/en/function.hash-hmac.php
// Here is an efficient PBDKF2 implementation:

function pbkdf2($algorithm, $password, $salt, $count, $key_length, $raw_output = false)
{
    $algorithm = strtolower($algorithm);
    if(!in_array($algorithm, hash_algos(), true))
		return false;
	
    if($count <= 0 || $key_length <= 0)
        return false;

    $hash_length = strlen(hash($algorithm, "", true));
    $block_count = ceil($key_length / $hash_length);

    $output = "";
    for($i = 1; $i <= $block_count; $i++) {
        // $i encoded as 4 bytes, big endian.
        $last = $salt . pack("N", $i);
        // first iteration
        $last = $xorsum = hash_hmac($algorithm, $last, $password, true);
        // perform the other $count - 1 iterations
        for ($j = 1; $j < $count; $j++) {
            $xorsum ^= ($last = hash_hmac($algorithm, $last, $password, true));
        }
        $output .= $xorsum;
    }

    if($raw_output)
        return substr($output, 0, $key_length);
    else
        return bin2hex(substr($output, 0, $key_length));
}

echo pbkdf2('sha256', '1234', 'salt', 4, 32);
```

### HMAC RipeMD160
This implements HMAC-RipeMD160.
```
if (auto hmac = create_hmac(algorithm::RipeMD160)) {
	digest_t Digest;

	if (hmac->init((uint8_t*)"1234", 4)) {
		hmac->update((uint8_t*)"abcd", 4);
		hmac->finalize(Digest);

		printf("HMAC RipeMD160(abcd, 1234): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: ef855c2fa4fc56e2b341a5ef0044d3c7a21f4508
// hash_hmac('ripemd160', 'abcd', '1234'): ef855c2fa4fc56e2b341a5ef0044d3c7a21f4508
```

### HMAC MD5
This implements HMAC-MD5.
```
if (auto hmac = create_hmac(algorithm::MD5)) {
	digest_t Digest;

	if (hmac->init((uint8_t*)"1234", 4)) {
		hmac->update((uint8_t*)"abcd", 4);
		hmac->finalize(Digest);

		printf("HMAC MD5(abcd, 1234): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: a825764f8c87475b652411748b011a11
// hash_hmac('md5', 'abcd', '1234'): a825764f8c87475b652411748b011a11
```

### HMAC MD4
This implements HMAC-MD4.
```
if (auto hmac = create_hmac(algorithm::MD4)) {
	digest_t Digest;

	if (hmac->init((uint8_t*)"1234", 4)) {
		hmac->update((uint8_t*)"abcd", 4);
		hmac->finalize(Digest);

		printf("HMAC MD4(abcd, 1234): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: 27c706484c8b1c80af3f7bffa061253c
// hash_hmac('md4', 'abcd', '1234'): 27c706484c8b1c80af3f7bffa061253c
```

### HMAC SHA256
This implements HMAC-SHA256.
```
if (auto hmac = create_hmac(algorithm::SHA256)) {
	digest_t Digest;

	if (hmac->init((uint8_t*)"1234", 4)) {
		hmac->update((uint8_t*)"abcd", 4);
		hmac->finalize(Digest);

		printf("HMAC SHA256(abcd, 1234): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: 30001ecb4a1a73fc66a1c23d62a6ca71009e3c879f07cd91444205f968a2ec60
// hash_hmac('sha256', 'abcd', '1234'): 30001ecb4a1a73fc66a1c23d62a6ca71009e3c879f07cd91444205f968a2ec60
```

### HMAC SHA384
This implements HMAC-SHA384.
```
if (auto hmac = create_hmac(algorithm::SHA384)) {
	digest_t Digest;

	if (hmac->init((uint8_t*)"1234", 4)) {
		hmac->update((uint8_t*)"abcd", 4);
		hmac->finalize(Digest);

		printf("HMAC SHA384(abcd, 1234): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: 0dd3df3e1eb9bbf561cd722089a86e234cc3cc5bdecf0ddc66976230b51bf964b790657c4831608aae99722cb6656d02
// hash_hmac('sha384', 'abcd', '1234'): 0dd3df3e1eb9bbf561cd722089a86e234cc3cc5bdecf0ddc66976230b51bf964b790657c4831608aae99722cb6656d02
```

### HMAC SHA512
This implements HMAC-SHA512.
```
if (auto hmac = create_hmac(algorithm::SHA512)) {
	digest_t Digest;

	if (hmac->init((uint8_t*)"1234", 4)) {
		hmac->update((uint8_t*)"abcd", 4);
		hmac->finalize(Digest);

		printf("HMAC SHA512(abcd, 1234): %s\n", to_hex(Digest).c_str());
		return 0;
	}
}

// output: 027139211cafddcf5034d816ff5a3491349d6c7a03e63662e5d47e2f39578c7a311935da78cf010da99d411a07f8f4dbcff8561a037faf03ef3703f4ad380b99
// hash_hmac('sha512', 'abcd', '1234'): 027139211cafddcf5034d816ff5a3491349d6c7a03e63662e5d47e2f39578c7a311935da78cf010da99d411a07f8f4dbcff8561a037faf03ef3703f4ad380b99
```

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
```

### CRC32
This implements CRC-32/BZIP2 (IEEE 802.3). (Init: 0xFFFFFFFF, Poly: 0x04C11DB7)
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
