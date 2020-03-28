.PHONY:cloud_backup
cloud_backup:cloud_backup.cpp
	g++ -std=c++11 $^ -o $@ -lpthread -lz -lboost_filesystem -lboost_system
clean:
	rm -rf backupfile gzfile
