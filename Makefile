CXX=g++
CXXFLAGS=
OBJS=src/chash/chash.o \
	src/chash/crc/CCRC16.o \
	src/chash/crc/CCRC32.o \
	src/chash/md5/CMD5.o \
	src/chash/ripemd160/CRipeMD160.o \
	src/chash/sha256/CSHA256.o \
	src/chash/sha384/CSHA384.o \
	src/chash/sha512/CSHA512.o
TARGET=libchash.so

all: $(TARGET)

clean:
	rm -rf *.o
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) --shared -o $@ $(OBJS)

%.o: %.cpp
	$(CXX) -fPIC -c $< -I src -o $@

