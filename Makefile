
all:
	g++ -O3 -std=c++17 -o shapow -pthread shapow.cpp

clean:
	rm -f shapow

update:
	wget -O picosha2.h https://raw.githubusercontent.com/okdshin/PicoSHA2/master/picosha2.h
