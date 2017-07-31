CC=gcc
rely=$(shell ls *c *h)
httpd:$(rely)
	$(CC) -o $@ $^
.PHONY:clean
clean:
	rm -rf httpd
