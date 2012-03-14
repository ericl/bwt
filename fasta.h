#ifndef FASTA_H
#define FASTA_H

#define NEWLINE 0
#define LINE 1
#define COMMENT 2

#include <fstream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

/**
 * Returns string with contents of fasta sequence
 */
string *read_fasta(char *filepath);

/**
 * Writes sequence and comment to output file
 */
void write_fasta(string filepath, string *seq, string comment);

#endif
