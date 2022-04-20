CXX=g++
RM=rm -f
CPPFLAGS=-g --std=c++20 -Wall -Wextra -O3

SRCS=contact_plan.cpp contact.cpp main.cpp read.cpp route.cpp utilities.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: cgr

cgr: $(OBJS)
		$(CXX) $(LDFLAGS) -o cgr $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
		$(RM) ./.depend
		$(CXX) $(CPPFLAGS) -MM $^>>./.depend;

clean:
		$(RM) $(OBJS)

distclean: clean
		$(RM) *~ .depend

include .depend