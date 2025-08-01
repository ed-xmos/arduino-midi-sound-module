xcc -target=XCORE-AI-EXPLORER main.xc xmidi.cpp ../arduino-midi-sound-module/emscripten/avr/mocks.cpp \
-std=c++14 -report -Os -g \
-I ../arduino-midi-sound-module -I ../arduino-midi-sound-module/emscripten -I . -DPROGMEM="" \
-DF_CPU=100000000 -D__EMSCRIPTEN__

# ../arduino-midi-sound-module/emscripten/avr/mocks.cpp \
