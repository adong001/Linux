#.PHONY:cloud_backup
Backup:Backup.cpp
	g++ -std=c++11 $^ -o $@ -lpthread -lz -lboost_filesystem -lboost_system -g
srv:srv.cpp
	g++ -std=c++11 $^ -o $@ -lpthread -lboost_system -lboost_filesystem
clean:
	rm -rf backupfile gzfile
