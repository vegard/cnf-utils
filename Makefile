CXXFLAGS := -Wall -g -O3

programs := \
	cnf-grep \
	cnf-shuffle-variables \
	cnf-shuffle-literals \
	cnf-shuffle-clauses \
	cnf-sort-literals \
	cnf-sort-clauses \
	cnf-propagate \
	cnf-stat

all: $(programs)

clean:
	rm -rf $(programs)
