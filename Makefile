CC = g++
CFLAGS = -g -Wall -std=c++11
vpath %.h src
vpath %.cpp src
OBJDIR = obj
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lmpg123
OBJECTS = $(addprefix $(OBJDIR)/, main.o WindowManager.o SoundManager.o GameManager.o PlayState.o)

default: beat-dodge

beat-dodge: $(OBJDIR) $(OBJECTS)
	$(CC) $(CFLAGS) -o beat-dodge $(OBJECTS) $(LIBS)

$(OBJDIR): 
	mkdir -p $(OBJDIR)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -r $(OBJDIR)/*.o