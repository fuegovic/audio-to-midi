######################################
#
# audio-to-midi
#
######################################

AUDIO_TO_MIDI_VERSION = master
AUDIO_TO_MIDI_SITE = $(call github,fuegovic,audio-to-midi,$(AUDIO_TO_MIDI_VERSION))
AUDIO_TO_MIDI_BUNDLES = audio-to-midi.lv2

AUDIO_TO_MIDI_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D)

define AUDIO_TO_MIDI_BUILD_CMDS
	$(AUDIO_TO_MIDI_TARGET_MAKE)
endef

define AUDIO_TO_MIDI_INSTALL_TARGET_CMDS
	$(AUDIO_TO_MIDI_TARGET_MAKE) install DESTDIR=$(TARGET_DIR)
	cp -rL $($(PKG)_PKGDIR)/audio-to-midi.lv2/* $(TARGET_DIR)/usr/lib/lv2/audio-to-midi.lv2/
endef

$(eval $(generic-package))