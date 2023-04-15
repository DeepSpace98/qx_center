#开发框架头文件路径
PUBINCL = -I/project/public

#开发框架cpp文件名
PUBCPP = /project/public/_public.cpp

#编译参数
CFLAGS = -g

all:crtsurfdata1 crtsurfdata2

crtsurfdata1:crtsurfdata1.cpp
	g++ $(CFLAGS) -o crtsurfdata1 crtsurfdata1.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata1 ../bin/.

crtsurfdata2:crtsurfdata2.cpp
	g++ $(CFLAGS) -o crtsurfdata2 crtsurfdata2.cpp $(PUBINCL) $(PUBCPP) -lm -lc
	cp crtsurfdata2 ../bin/.
clean:
	rm crtsurfdata1 crtsurfdata2