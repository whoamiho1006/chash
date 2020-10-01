# chash
Hash algorithm library in C++.

##usage
### SHA-256

```
if (IAlgorithm* SHA256 = createAlgorithm(EAlgorithm::SHA256)) {
	IDigest* Digest = SHA256->create();

	if (SHA256->init()) {
		SHA256->update((uint8_t*)"abcd", 4);
		SHA256->finalize(Digest);

		printf("%s\n", Digest->toHex().c_str());
	}
}

// output: 88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589
// php's hash('sha256', 'abcd'): 88d4266fd4e6338d13b845fcf289579d209c897823b9217da3e161936f031589
```

### MD5
```
if (IAlgorithm* MD5 = createAlgorithm(EAlgorithm::MD5)) {
	IDigest* Digest = MD5->create();

	if (MD5->init()) {
		MD5->update((uint8_t*)"abcd", 4);
		MD5->finalize(Digest);

		printf("%s\n", Digest->toHex().c_str());
	}
}

// output: e2fc714c4727ee9395f324cd2e7f331f
// php's md5('abcd'): e2fc714c4727ee9395f324cd2e7f331f
```

### RipeMD160
```
if (IAlgorithm* RipeMD160 = createAlgorithm(EAlgorithm::RipeMD160)) {
	IDigest* Digest = RipeMD160->create();

	if (RipeMD160->init()) {
		RipeMD160->update((uint8_t*)"abcd", 4);
		RipeMD160->finalize(Digest);

		printf("%s\n", Digest->toHex().c_str());
	}
}

// output: 2e7e536fd487deaa943fda5522d917bdb9011b7a
// php's hash('ripemd160', 'abcd'): 2e7e536fd487deaa943fda5522d917bdb9011b7a
```
