#include <iostream>
#include <assert.h>
#include "fasta.h"
#include "bwt.h"

using namespace std;

int main(int argc, char **argv) {
    assert (argc > 1);
    string op = string(argv[1]);

    if (!op.compare("-test")) {
        string *input = read_fasta((char*)"sonnet1.fasta");
        string *input_bwt = bwt(input);
        string *input_ibwt = ibwt(input_bwt);
        string *output = read_fasta((char*)"sonnet1.bwt.fasta");
        assert (*input_bwt == *output);
        assert (*input_ibwt == *input);
        cout << "OK" << endl;
        exit(0);
    }

    assert (argc == 4);
    bool invert = false;

    if (!op.compare("-bwt")) {
        invert = false;
    } else if (!op.compare("-ibwt")) {
        invert = true;
    } else {
        assert (!"unknown op");
    }

    string *input = read_fasta(argv[2]);
    string output = string(argv[3]);

    if (invert) {
        write_fasta(output, ibwt(input), "inverse BWT of " + string(argv[2]));
    } else {
        write_fasta(output, bwt(input), "BWT of " + string(argv[2]));
    }
	return 0;
}
