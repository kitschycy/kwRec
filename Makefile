redis: redis.cpp redis.h
	g++ *.cpp -o redis -L/usr/local/lib/ -lhiredis

clean:
	rm redis.o redis
