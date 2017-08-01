CC=gcc
rely=$(shell ls *c *h)
httpd:$(rely)
	$(CC) -o $@ $^ -g -lpthread
.PHONY:clean
clean:
	rm -rf httpd
