.PHONY:cloud_backup
cloud_backup:cloud_backup.cpp
	g++ -std=c++11 $^ -o $@ -lpthread -lz -lboost_filesystem -lboost_system
srv:srv.cpp
	g++ -std=c++11 $^ -o $@ -lpthread -lboost_system -lboost_filesystem
clean:
	rm -rf backupfile gzfile
