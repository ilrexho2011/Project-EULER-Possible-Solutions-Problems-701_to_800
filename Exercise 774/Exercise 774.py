import functools

def div2(x):
  if x is None: return None,0
  return x//2,x&1

def modM(x,M):
  return x if M is None else x%M

@functools.lru_cache(maxsize=None)
def fibM(n,M):
  if n<2: return 1-n
  return modM(fibM(n-1,M)+fibM(n-2,M),M)

@functools.lru_cache(maxsize=None)
def ccons1(n,b,x,y,M=None):
  """Number of length n conjunctive sequences with terms < b and endpoints intersecting x, y"""
  if b==0: return 1 if n==0 and (x is None or y is None or x&y) else 0
  bp=b//2
  def cp(m,x,y):
    if m==0: return ccons1(0,0,x,y)
    xp,x1=div2(x)
    yp,y1=div2(y)
    o=modM(ccons1(m,bp,xp,yp,M)*fibM(m+3-x1-y1,M),M)
    if x1:
      o+=modM(ccons1(m,bp,None,yp,M)*fibM(m+1-y1,M),M)
    if y1:
      o+=modM(ccons1(m,bp,xp,None,M)*fibM(m+1-x1,M),M)
    if x1 and y1:
      o+=modM(ccons1(m,bp,None,None,M)*fibM(m-1,M),M)
    return modM(o,M)
  o=cp(n,x,y)
  # m = position of start of 11
  for m in range(1,n):
    o+=modM(cp(m,x,1)*ccons1(n-m,b,1,y,M),M)
  if b&1:
    # m = entries before b-1
    for m in range(n):
      o+=modM(cp(m,x,b-1)*ccons1(n-m-1,b,b-1,y,M),M)
  return modM(o,M)


def c2(n,b,M=None):
  b+=1
  for m in range(n): ccons1(m,b,None,None,M)
  return ccons1(n,b,None,None,M)

M=998244353
print(c2(3,4))
print(c2(10,6))
print(c2(100,200,M))
print(c2(123,123456789,M))