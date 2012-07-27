// Copyright 2012 Eric Liang

#include "fasta.h"

using std::ios;
using std::ofstream;
using std::endl;

string *read_fasta(char *filepath) {
    struct stat st;
    stat(filepath, &st);

    int fd = open(filepath, O_RDONLY);
    char *data = reinterpret_cast<char*>(
        mmap(NULL, st.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0));

    char *lim = data + st.st_size;
    char *readptr = data - 1;
    char *writeptr = data;
    int state = NEWLINE;

    while (readptr++ < lim) {
        switch (state) {
            case NEWLINE:
                switch (*readptr) {
                    case '\r': case '\n':
                        state = NEWLINE;
                        break;
                    case '>': case ';':
                        state = COMMENT;
                        break;
                    default:
                        *writeptr++ = *readptr;
                        state = LINE;
                }
                break;
            case LINE:
                switch (*readptr) {
                    case '\r': case '\n':
                        state = NEWLINE;
                        break;
                    default:
                        *writeptr++ = *readptr;
                }
                break;
            case COMMENT:
                switch (*readptr) {
                    case '\r': case '\n':
                        state = NEWLINE;
                }
                break;
        }
    }

    return new string(data, writeptr - data - 1);
}

void write_fasta(string filepath, string *seq, string comment) {
    ofstream of;
    of.open(filepath.c_str(), ios::out);
    of << ">" << comment << endl;
    for (size_t i = 0; i < seq->length(); i += 80) {
        of << seq->substr(i, 80) << endl;
    }
    of.close();
}
