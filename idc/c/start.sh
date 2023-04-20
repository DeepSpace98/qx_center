################################################
#启动数据中心后台服务程序的脚本
################################################

# 检查服务程序是否超时，配置在/etc/rc.local中由root用户执行
#project/tools/bin/procctl 30 /project/tools/bin/checkproc

# 压缩数据中心后台服务程序的备份日志
/project/tools/bin/procctl 300 /project/tools/bin/gzipfiles/ /log/idc "*.log.20*" 0.04

#生成用于测试的数据
/project/tools/bin/procctl 60 /project/idc/bin/crtsurfdata /project/idc/ini/stcode.ini /tmp/idc/surfdata /log/idc/crtsurfdata.log xml,json,csv

#清理目录和历史数据
/project/tools/bin/procctl 300 /project/tools/bin/deletefiles /tmp/idc/surfdata "*" 0.04

#采集全国气象站观测数据
/project/tools/bin/procctl 30 /project/tools/bin/ftpgetfiles /log/idc/ftpgetfiles_surfdata.log "<host>127.0.0.1:21</host><mode>1</mode><username>wucz</username><password>wuczpwd</password><localpath>/idcdata/surfdata</localpath><remotepath>/tmp/idc/surfdata</remotepath><matchname>SURF_ZH*.XML,SURF_ZH*.XML</matchname><listfilename>/idcdata/ftplist/ftpgetfiles_surfdata.list</listfilename><ptype>1</ptype><okfilename>/idcdata/ftplist/ftpgetfiles_surfdata.xml</okfilename><checkmtime>true</checkmtime><timeout>80</timeout><pname>ftpgetfiles_surfdata</pname>"
