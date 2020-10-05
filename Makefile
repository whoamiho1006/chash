CXX=g++
STRIP=strip
CXXFLAGS=-fPIC --shared

OBJS=src/chash/chash.o \
	src/chash/crc/crc16.o \
	src/chash/crc/crc32.o \
	src/chash/crc/crc64.o \
	src/chash/md/md5.o \
	src/chash/md/md4.o \
	src/chash/ripemd/ripemd128.o \
	src/chash/ripemd/ripemd160.o \
	src/chash/sha/sha256.o \
	src/chash/sha/sha384.o \
	src/chash/sha/sha512.o

TARGET=libchash.so

all: $(TARGET)

clean:
	rm -rf $(OBJS)
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I src -o $@

