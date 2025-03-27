PREFIX ?= /usr/local
LV2_DESTDIR = $(DESTDIR)$(PREFIX)/lib/lv2

all:
	$(MAKE) -C src

install: all
	install -d $(LV2_DESTDIR)
	cp -r mod-audio-to-midi.lv2 $(LV2_DESTDIR)

clean:
	$(MAKE) clean -C src
	rm -f mod-audio-to-midi.lv2/*.so