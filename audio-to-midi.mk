######################################
#
# mod-audio-to-midi
#
######################################

MOD_AUDIO_TO_MIDI_VERSION = main
MOD_AUDIO_TO_MIDI_SITE = $(call github,fuegovic,mod-audio-to-midi,$(MOD_AUDIO_TO_MIDI_VERSION))
MOD_AUDIO_TO_MIDI_BUNDLES = mod-audio-to-midi.lv2

MOD_AUDIO_TO_MIDI_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D)

define MOD_AUDIO_TO_MIDI_BUILD_CMDS
	$(MOD_AUDIO_TO_MIDI_TARGET_MAKE)
endef

define MOD_AUDIO_TO_MIDI_INSTALL_TARGET_CMDS
	$(MOD_AUDIO_TO_MIDI_TARGET_MAKE) install DESTDIR=$(TARGET_DIR)
	cp -rL $($(PKG)_PKGDIR)/mod-audio-to-midi.lv2/* $(TARGET_DIR)/usr/lib/lv2/mod-audio-to-midi.lv2/
endef

$(eval $(generic-package))