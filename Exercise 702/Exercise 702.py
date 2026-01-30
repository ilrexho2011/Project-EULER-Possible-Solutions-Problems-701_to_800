import math
import sys

# Draw a picture of the situation for (N,m)
# where N is the side length and m is
# the number of divisions in the dot grid
# (some power of 2, when computing S(N)).
# For instance, the picture for (N=3,m=5) is:
#   *--*-+*--*+-*--*
#   |\   |\   |\   |\
#   | \  | \  | \  | \
#   *  * |* \*| *\ *  *
#   |   \|   \|   \|   \
#   +----+----+----+----+
#   *\ * |*  *|\*  *\ * |*
#   | \  | \  | \  | \  | \
#   |  \ |  \ |  \ |  \ |  \
#   *  *\|*  *| * \*  *\|*  *
#   +----+----+----+----+----+
#   |\   |\   |\   |\   |\   |\
#   * \* |*\ *| *  * \* |*\ *| *
#   |  \ |  \ |  \ |  \ |  \ |  \
#   |   \|   \|   \|   \|   \|   \
#   *--*-+*--*+-*--*--*-+*--*+-*--*
# and the picture for (N=3,m=8) is:
#   *--*--*-+*--*--*+-*--*--*
#   |\      |\      |\      |\
#   | \     | \     | \     | \
#   *  *  * |* \*  *| *\ *  *  *
#   |   \   |   \   |   \   |   \
#   |    \  |    \  |    \  |    \
#   *  *  * |*  * \*| *  *\ *  *  *
#   |      \|      \|      \|      \
#   +-------+-------+-------+-------+
#   *\ *  * |*  *  *|\*  *  *\ *  * |*
#   | \     | \     | \     | \     | \
#   |  \    |  \    |  \    |  \    |  \
#   *  *\ * |*  *  *| * \*  *  *\ * |*  *
#   |    \  |    \  |    \  |    \  |    \
#   |     \ |     \ |     \ |     \ |     \
#   *  *  *\|*  *  *| *  * \*  *  *\|*  *  *
#   +-------+-------+-------+-------+-------+
#   |\      |\      |\      |\      |\      |\
#   * \*  * |*\ *  *| *  *  * \*  * |*\ *  *| *
#   |  \    |  \    |  \    |  \    |  \    |  \
#   |   \   |   \   |   \   |   \   |   \   |   \
#   *  * \* |*  *\ *| *  *  *  * \* |*  *\ *| *  *
#   |     \ |     \ |     \ |     \ |     \ |     \
#   |      \|      \|      \|      \|      \|      \
#   *--*--*-+*--*--*+-*--*--*--*--*-+*--*--*+-*--*--*
def drawPicture(N,m):
  print("(N=%r, m=%r)" % (N,m))
  assert N >= 1
  assert m >= 1
  assert gcd(N,m) == 1
  for i in range(N*m+1):
    line = ''
    for j in range(N*m+1+i):
      if i % N == 0 and j % N == 0:
        line += '*'
      elif i % m == 0:
        if j % m == 0:
          line += '+'
        else:
          line += '-'
      else:
        if j % m == 0:
          line += '|'
        elif j % m == i % m:
          line += '\\'
        else:
          line += ' '
    print(line)

# From https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python#answer-57173975
# Given 0<a<b, returns the unique c such that 0<c<b and a*c == gcd(a,b) (mod b).
# In particular, if a,b are relatively prime, returns the inverse of a modulo b.
def invmod(a,b): return 0 if a==0 else 1 if b%a==0 else b - invmod(b%a,a)*b//a

def gcd(a,b):
  while b != 0: a,b = b,a%b
  return a

def triangled(n): return n * (n+1) // 2

# Return n such that triangled(n)==triangled_n.
def triroot(triangled_n):
  # CBB: could do binary search.  but this is used only for naive anyway
  for n in range(triangled_n+1):
    if triangled(n) == triangled_n:
      return n
  assert False

# Return a dict where answer[k] is the number of upward triangles
# containing triangled(k) dots.  For instance:
#   Qhistogram(N=3,m=5)  returns {0:6, 1:9}
#   Qhistogram(N=3,m=8)  returns {1:6, 2:9}.
#   Qhistogram(N=3,m=11) returns {2:6, 3:9}.
#   Qhistogram(N=3,m=14) returns {3:6, 4:9}.
# Simple slow implementation.
def QhistogramNaive(N, m):
  verboseLevel = 0
  if verboseLevel >= 1: print("            in QhistogramNaive(N=%r m=%r)" % (N,m))
  position2count = {}
  for I in range(N):
    for J in range(2*N-I):
      position2count[(I,J)] = 0
  for i in range(m+1):
    for j in range(2*m+1-i):
      ii = i*N%m
      jj = j*N%m
      if ii > 0 and jj > 0 and ii+jj < m:
        I = i*N//m
        J = j*N//m
        position2count[(I,J)] += 1
  answerTriangled = {}
  for I,J in position2count:
    count = position2count[(I,J)]
    if count not in answerTriangled: answerTriangled[count] = 0
    answerTriangled[count] += 1
  answer = {}
  for tri in answerTriangled:
    answer[triroot(tri)] = answerTriangled[tri]
  if verboseLevel >= 1: print("            out QhistogramNaive(N=%r m=%r), returning %r" % (N,m,answer))
  return answer

# Return a dict where answer[k] is the number of upward triangles
# containing triangled(k) dots. For instance:
#   Qhistogram(N=3,m=5)  returns {0:6, 1:9}
#   Qhistogram(N=3,m=8)  returns {1:6, 2:9}.
#   Qhistogram(N=3,m=11) returns {2:6, 3:9}.
#   Qhistogram(N=3,m=14) returns {3:6, 4:9}.
cache = {}
def Qhistogram(N, m):
  verboseLevel = 0
  if verboseLevel >= 1: print("            in Qhistogram(N=%r m=%r)" % (N,m))
  key = (N,m)
  if key in cache: return cache[key]

  assert N >= 1
  assert m >= 1
  assert gcd(N,m) == 1
  answer = {}
  if m == 1:
    # all upward triangles (in fact all triangles) are empty.
    # so answer[0] is the number of upward triangles.
    answer[0] = 2*triangled(N) + triangled(N-1)
  elif N == 1:
    # 2 upward triangles, each containing triangled(m-2) dots
    assert m >= 2
    answer[m-2] = 2
  elif N > m:
    # (N,m) has same number of total dots in upward triangles
    # as (N%m,m), but spread out into at most one dot
    # per triangle.
    if verboseLevel >= 1: print("              it's the easy spread-out case, at most 1 dot per tri")
    nOnes = Q(N%m, m)
    # Number of empty triangles is total number of triangles
    # minus number of nonempty ones.
    nZeros = 2*triangled(N)+triangled(N-1) - nOnes
    if nOnes != 0: answer[1] = nOnes
    if nZeros != 0: answer[0] = nZeros
  elif 2*N < m:
    # It's the 2*N<m case.
    # Reduce to the N<m<2*N case:
    # similar picture, with uniformly fewer dots per triangle.
    if verboseLevel >= 1: print("              it's the 2*N<m case that can be reduced")
    m0 = N + m%N
    subanswer = Qhistogram(N, m0)
    assert len(subanswer) in [2,3]
    # the keys are [0,1] or [0,1,2]
    for i in range(len(subanswer)): assert i in subanswer
    offset = (m-m0)//N
    for k in subanswer:
      answer[k+offset] = subanswer[k]
  else:
    # It's the N<m<2*N case.  This is the tricky one.
    if verboseLevel >= 1: print("              it's the tricky N<m<2*N case")
    assert N < m and m < 2*N

    # We can get nTwos (but not nOnes nor nZeros)
    # by similarity with (N,m-N) (similar to the 2*N<m case above).
    # That is, nTwos is (N,m-N)'s nOnes.
    subanswer = Qhistogram(N, m-N)
    nTwos = 0 if 1 not in subanswer else subanswer[1]

    # How do we figure out nOnes and nZeros?
    # We can enlist smaller case Helper(m-N,m) to help,
    # using the fact that sum of the total number of dots
    # in both cases is a simple function of m:
    #      Helper(N,m) + Helper(m-N,m) == 3*triangled(m-2).
    # i.e. Helper(N,m) = 3*triangled(m-2) - Helper(m-N,m).
    totalNumDots = 3*triangled(m-2) - Q(m-N,m)

    # All remaining triangles have 1 or 0 dots in them.
    # The number of 1's is the number of remaining dots...
    nOnes = totalNumDots - nTwos*triangled(2)

    # Number of empty triangles is total number of triangles
    # minus number of nonempty ones.
    nZeros = 2*triangled(N)+triangled(N-1) - (nTwos + nOnes)
    if nTwos != 0: answer[2] = nTwos
    assert nOnes != 0; answer[1] = nOnes
    assert nZeros != 0; answer[0] = nZeros

  cache[key] = answer
  if verboseLevel >= 1: print("            out Qhistogram(N=%r m=%r), returning %r" % (N,m,answer))
  return answer

# Given triangle subdivison N and dots subdivision m,
# how many of the dots are inside upward triangles?
def Q(N, m):
  histogram = Qhistogram(N, m)
  return sum(histogram[k] * triangled(k) for k in histogram)

def sortedDictRepr(d):
  return '{' + ', '.join('%r:%r' % (k,d[k]) for k in sorted(d)) + '}'

# Make sure Qhistogram() agrees with the naive implementation.
def Qtest(upto):
  for m in range(1,upto+1):
    for N in range(1,upto+1):
      if gcd(N,m) != 1: continue
      print("      N=%r m=%r" % (N, m))
      histSmart = Qhistogram(N, m)
      histNaive = QhistogramNaive(N, m)
      print("          histSmart = %s" % (sortedDictRepr(histSmart),))
      print("          histNaive = %s" % (sortedDictRepr(histNaive),))
      assert histSmart == histNaive

      # Verify fun facts about Q
      if m != 1:
        assert Q(N, m) == Q(N%m, m)
      if N < m:
        assert Q(N,m) + Q(m-N,m) == 3 * triangled(m-2)
        assert Q(N,m) == Q(invmod(N,m),m)

def S(N):
  verboseLevel = 1
  if verboseLevel >= 1: print("        in S(N=%r)" % (N,))

  # Works only if N is positive and odd.
  assert N >= 1
  assert N%2 == 1

  # cumulative[k] will be the number of triangles reached in <=k moves.
  cumulative = []

  # The k's of interest are up to floor(log2(N))+2.
  for k in range(int(math.floor(math.log(N,2)))+2 + 1):
    histogram = Qhistogram(N, 2**k)
    if verboseLevel >= 1: print("          Qhistogram(N=%r, 2^%r) = %r" % (N, k, histogram))
    # We want the number of triangles with nonzero number of dots,
    # without regard to number of dots inside (as long as nonzero).
    # I.e. the sum of the values corresponding to the nonzero keys
    # in the histogram.
    cumulative.append(sum(histogram[i] for i in histogram if i != 0))
  if verboseLevel >= 1: print("          cumulative = %r" % (cumulative,))

  # exclusive[k] will be the number of triangles reached in exactly k moves.
  exclusive = [cumulative[i] - (0 if i==0 else cumulative[i-1]) for i in range(len(cumulative))]
  if verboseLevel >= 1: print("          exclusive = %r" % (exclusive,))

  answer = sum(exclusive[i] * i for i in range(len(exclusive)))
  if verboseLevel >= 1: print("        out S(N=%r), returning %r" % (N,answer))
  return answer

def main(argv):
  print("in main")
  if len(argv) == 4 and argv[1] == 'picture':
    N = int(argv[2])
    m = int(argv[3])
    drawPicture(N, m)
  elif len(argv) == 3 and argv[1] == 'Qtest':
    upto = int(argv[2])
    Qtest(upto)
  elif len(argv) in [2,3]:
    N0 = int(argv[1])
    N1 = int(argv[-1])
    for N in range(N0,N1+1):
      if N%2 != 1: continue  # works only for odd
      s = S(N)
      print("      S(N=%r) = %r" % (N, s))
  else:
    exit("Usage: runme.py picture <N> <m>")
    exit("Usage: runme.py Qtest <upto>")
    exit("Usage: runme.py <N>")
    exit("Usage: runme.py <N0> <N1>")

  print("out main")

main(sys.argv)