CXX=g++
STRIP=strip
CXXFLAGS=

OBJS=md5.o md4.o ripemd160.o sha256.o crc16.o main.o

TARGET=example

all: $(TARGET)

clean:
	rm -rf $(OBJS)
	rm -f $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -L../ -lchash

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -I ../src -o $@

