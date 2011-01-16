CXXFLAGS := -Wall -g -O3

programs := \
	cnf-grep \
	cnf-shuffle-variables \
	cnf-shuffle-literals \
	cnf-shuffle-clauses

all: $(programs)

clean:
	rm -rf $(programs)
