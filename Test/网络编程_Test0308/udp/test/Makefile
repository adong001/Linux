all:udp_src udp_cli	
udp_srv:udp_srv.c
	gcc $^ -o $@
udp_cli:udp_cli.cpp
	g++ $^ -o $@
clean:
	rm udp_cli udp_src
