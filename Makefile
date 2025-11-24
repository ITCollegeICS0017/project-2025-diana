
CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
TARGET = RailwayApp
SRCS = main.cpp ConsoleUI.cpp TicketService.cpp Repositories.cpp Employee.cpp IClock.cpp Ticket.cpp Types.cpp Util.cpp FileTicketRepository.cpp RefundPolicy.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
    $(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp
    $(CC) $(CFLAGS) -c $< -o $@

clean:
    rm -f $(OBJS) $(TARGET)

run: all
    ./$(TARGET)
