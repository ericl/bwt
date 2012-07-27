// Copyright 2012 Eric Liang

#ifndef FASTA_H_
#define FASTA_H_

#define NEWLINE 0
#define LINE 1
#define COMMENT 2

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <fstream>

using std::string;

/**
 * Returns string with contents of fasta sequence
 */
string *read_fasta(char *filepath);

/**
 * Writes sequence and comment to output file
 */
void write_fasta(string filepath, string *seq, string comment);

#endif  // FASTA_H_
