#!/usr/bin/env python
"""Reference implementation of KS algorithm for suffix arrays and BWT"""

from itertools import izip, chain, repeat

def grouper(n, iterable, padvalue=None):
    return list(izip(*[chain(iterable, repeat(padvalue, n-1))]*n))

def char_sort(l):
    return sorted(l) # TODO radix sort

def struct_sort(l):
    return sorted(l) # TODO radix sort

def ranks(string):
    sstring = char_sort(string)
    prev = None
    cur_rank = 1
    rank = {}
    for char in sstring:
        if char != prev:
            prev = char
            rank[char] = cur_rank
            cur_rank += 1
    return [rank[char] for char in string]

def naivesa(T):
    return inaivesa(ranks(T), None)
    
def inaivesa(T, terminator):
    if type(T) is list:
        T.append(terminator)
    return sorted(range(len(T)), key=lambda i: T[i:])

def dc3(T):
    return idc3(ranks(T))

def idc3(T, depth=1, terminator=False):

    if len(T) < 5:
        return inaivesa(T, terminator=False)

    def __(*args):
        pass
        print '|'*depth,
        for a in args:
            print a,
        print
    __("args", T)

    if type(T) is list:
        T.append(terminator)
    else:
        T += terminator
    n = len(T)

    # step 0: construct a sample
    B = {}
    for k in [0,1,2]:
        B[k] = [i for i in range(n) if i % 3 == k]
    C = B[1] + B[2]
    print n
    x = int((n+1)/3)
    y = int(n/3)
    print B[1]
    assert len(B[1]) == x, (len(B[1]), x)
    y = int(n/3)
    assert len(B[2]) == y, (len(B[2]), y)
    __("C", C)

    # step 1: sort sample suffixes
    R1 = grouper(3, T[1:], terminator)
    R2 = grouper(3, T[2:], terminator)
    Rr = ranks(R1 + R2)
    __("Rr", Rr)
    SAR = idc3(Rr, depth + 1, terminator=False)
    __("SAR", SAR)
    rank = [None]*(n+2)
    rank[n] = 0
    rank[n+1] = 0
    for r, _ in enumerate(SAR[1:], 1):
        rank[C[_]] = r
    __("S ranks", rank)

    # step 2: sort nonsample suffixes
    SB0 = struct_sort([(T[i], rank[i+1], i) for i in B[0]])

    # step 3: merge
    Sc = [C[_] for _ in SAR[1:]]
    __("Sc", Sc)
    __("SB0", SB0)
    SB0.reverse()
    Sc.reverse()
    buf = []
    if type(T) is list:
        T.append(terminator)
    else:
        T += terminator
    while SB0 and Sc:
        i = Sc.pop()
        tj, _, j = SB0.pop()
        if i % 3 == 1:
            if (T[i], rank[i+1]) <= (T[j], rank[j+1]):
                buf.append(i)
                SB0.append((tj,_,j))
            else:
                buf.append(j)
                Sc.append(i)
        elif i % 3 == 2:
            if (T[i], T[i+1], rank[i+2]) <= (T[j], T[j+1], rank[j+2]):
                buf.append(i)
                SB0.append((tj,_,j))
            else:
                buf.append(j)
                Sc.append(i)
        else:
            assert False
    while SB0:
        tj, _, j = SB0.pop()
        buf.append(j)
    while Sc:
        i = Sc.pop()
        buf.append(i)
    return buf

def bwt(T, SA):
    output = ''
    for i in SA[1:]:
        output += T[i-1]
    return output

def ibwt(L):
    F = ''.join(char_sort(L))
    M = [None]*256
    C = [0]*len(F)
    for i, _ in enumerate(F):
        k = ord(_)
        if M[k] is None:
            M[k] = i
    tmp = [0]*256
    for i, _ in enumerate(L):
        k = ord(_)
        C[i] = tmp[k]
        tmp[k] += 1
    buf = [None]*len(F)
    buf[-1] = F[0]
    ptr = len(F) - 2
    j = 0
    for k in range(1, len(F)):
        buf[ptr] = L[j]
        ptr -= 1
        j = M[ord(L[j])] + C[j]
    return ''.join(buf)


if __name__ == '__main__':
    import sys
    if sys.argv[1] == '-bwt':
        with open(sys.argv[2]) as f:
            f.readline()
            T = ''.join(f.read().split('\n'))
            with open(sys.argv[3], 'w') as out:
                print >>out, ">BWT of", sys.argv[2]
                res = bwt(T, dc3(T))
                while res:
                    first = res[:80]
                    print >>out, first
                    res = res[80:]
    elif sys.argv[1] == '-ibwt':
        with open(sys.argv[2]) as f:
            f.readline()
            T = ''.join(f.read().split('\n'))
            with open(sys.argv[3], 'w') as out:
                print >>out, ">inverse BWT of", sys.argv[2]
                res = ibwt(T)
                while res:
                    first = res[:80]
                    print >>out, first
                    res = res[80:]
    else:
        assert False
