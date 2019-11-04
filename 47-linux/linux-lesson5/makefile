src=$(wildcard *.c)
$(warning $(src))
obj=$(patsubst %.c, %.o, $(src))
$(warning $(obj))

bin=a
$(bin):$(obj)
	gcc $^ -o $@
%.o:%.c
	gcc -c $< -o $@
clean:
	rm $(obj) $(bin)
