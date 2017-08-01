CC=gcc
rely=$(shell ls *c *h)
httpd:$(rely)
	$(CC) -o $@ $^ -g
.PHONY:clean
clean:
	rm -rf httpd
