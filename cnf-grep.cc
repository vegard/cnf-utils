/*  cnf-grep  Copyright (c) 2011, Vegard Nossum
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

extern "C" {
#include <stdio.h>
#include <stdlib.h>
}

#include <vector>

#include <boost/shared_ptr.hpp>

/* TODO:
 * options: -c count number of matches
 *          -C context (recursively match other clauses that contain literals of the matched clauses)
 *          -v invert match
 *
 * advanced pattterns -- AND, OR, NOT
 */

class cnf {
public:
	typedef int literal;

	class clause {
	public:
		typedef boost::shared_ptr<clause> ptr;
		typedef std::vector<literal> literal_vector;

		literal_vector literals;

		clause()
		{
		}

		void add(literal x)
		{
			literals.push_back(x);
		}
	};

	typedef std::vector<clause::ptr> clause_vector;

	clause_vector clauses;

	cnf()
	{
	}

	void add(clause::ptr c)
	{
		clauses.push_back(c);
	}
};

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s [OPTION]... PATTERN [FILE] ...\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int pattern = atoi(argv[1]);

	FILE *fp;
	if (argc > 2) {
		fp = fopen(argv[2], "r");
		assert(fp);
	} else {
		fp = stdin;
	}

	unsigned int nr_variables;
	unsigned int nr_clauses;
	if (fscanf(fp, "p cnf %u %u\n", &nr_variables, &nr_clauses) != 2) {
		fprintf(stderr, "error: expected: p cnf <variables> <clauses>\n");
		exit(EXIT_FAILURE);
	}

	cnf f;
	for (unsigned int i = 0; i < nr_clauses; ++i) {
		cnf::clause::ptr clause(new cnf::clause());
		bool match = false;

		while (1) {
			int literal;
			if (fscanf(fp, "%d", &literal) != 1) {
				fprintf(stderr, "error: expected literal\n");
				exit(EXIT_FAILURE);
			}

			if (!literal)
				break;

			clause->add(literal);

			if (literal == pattern || -literal == pattern)
				match = true;
		}

		if (match)
			f.add(clause);
	}

	printf("p cnf %u %u\n", nr_variables, f.clauses.size());
	for (cnf::clause_vector::iterator cit = f.clauses.begin(),
		cend = f.clauses.end(); cit != cend; ++cit)
	{
		cnf::clause::ptr c(*cit);

		for (cnf::clause::literal_vector::iterator lit = c->literals.begin(),
			lend = c->literals.end(); lit != lend; ++lit)
		{
			printf("%d ", *lit);
		}

		printf("0\n");
	}

	fclose(fp);
	return 0;
}
