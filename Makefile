
OBJS=obj/main.o obj/Channel.o obj/Mixer.o obj/Fader.o obj/Thread.o -lpthread
CC=g++ -O4 -march=i686

bin/mixer: $(OBJS)
		$(CC) -o bin/mixer $(OBJS)
		strip bin/mixer

obj/main.o: src/main.cc src/Mixer.h src/Channel.h
		$(CC) -c src/main.cc -o obj/main.o

obj/Channel.o: src/Channel.cc src/Channel.h
		$(CC) -c src/Channel.cc -o obj/Channel.o

obj/Mixer.o: src/Mixer.cc src/Mixer.h src/Channel.h src/Fader.h
		$(CC) -c src/Mixer.cc -o obj/Mixer.o

obj/Fader.o: src/Fader.cc src/Fader.h src/Thread.h src/Channel.h
		$(CC) -c src/Fader.cc -o obj/Fader.o

obj/Thread.o: src/Thread.cc src/Thread.h
		$(CC) -c src/Thread.cc -o obj/Thread.o

clean:
		rm -rf obj/*.o bin/mixer src/*~

