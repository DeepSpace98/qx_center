#开发框架头文件路径
PUBINCL = -I/project/public

all:crtsurfdata1

crtsurfdata1:crtsurfdata1.cpp
	g++ -g -o crtsurfdata1 crtsurfdata1.cpp -I/project/public /project/public/_public.cpp -lm -lc
	cp crtsurfdata1 ../bin/.

clean:
	rm crtsurfdata1