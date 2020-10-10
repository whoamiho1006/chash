CXX=g++
STRIP=strip
CXXFLAGS=

OBJS=md5.o md4.o ripemd160.o sha256.o crc16.o crc32.o crc64.o \
	hmac_md5.o hmac_sha256.o hmac_sha384.o hmac_sha512.o \
	hmac_ripemd160.o pbkdf2.o main.o

TARGET=example

all: $(TARGET)

clean:
	rm -rf $(OBJS)
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -L../ -lchash

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I ../src -o $@

