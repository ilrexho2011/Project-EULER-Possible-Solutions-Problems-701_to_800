def gen_seq(x=290797, m=50515093):
  while True:
    yield x
    x = (x*x) % m


def compressed(num_timesteps, m=50515093):
  s = gen_seq()
  rects = []
  for _ in xrange(num_timesteps):
    x0, x1 = sorted([next(s), next(s)])
    y0, y1 = sorted([next(s), next(s)])
    rects.append((x0, x1+1, y0, y1+1))
  rects.append((0, m, 0, m))
  x_val = sorted(set(x for x0, x1, _, _ in rects for x in (x0, x1)))
  y_val = sorted(set(y for _, _, y0, y1 in rects for y in (y0, y1)))
  x_idx = {v: i for i, v in enumerate(x_val)}
  y_idx = {v: i for i, v in enumerate(y_val)}
  rects.pop()
  rects = [(x_idx[x0], x_idx[x1], y_idx[y0], y_idx[y1])
           for x0, x1, y0, y1 in rects]
  return x_val, y_val, rects


class SegTree:
  def __init__(self, weights):
    self.n = n = len(weights)
    self._w = weights
    self._lazy = [0 for _ in xrange(4*n)]
    self._hour = [[0]*12 for _ in xrange(4*n)]
    self._build_tree(0, 0, n)

  def _build_tree(self, u, a, b):
    if a + 1 < b:
      c = (a + b) // 2
      self._build_tree(2*u+1, a, c)
      self._build_tree(2*u+2, c, b)
    self._update_node(u, a, b)

  def _update_node(self, u, a, b):
    hour = self._hour[u]
    rot = self._lazy[u]
    if a + 1 < b:
      lhs = self._hour[2*u+1]
      rhs = self._hour[2*u+2]
      for i in xrange(12):
        hour[(i+rot) % 12] = lhs[i] + rhs[i]
    else:
      for i in xrange(12):
        hour[i] = 0
      hour[rot % 12] = self._w[a]

  def update(self, A, B, d):
    def _update(u, a, b):
      if b <= A or B <= a:
        return
      if A <= a and b <= B:
        self._lazy[u] += d
        self._update_node(u, a, b)
        return
      c = (a + b) // 2
      _update(2*u+1, a, c)
      _update(2*u+2, c, b)
      self._update_node(u, a, b)
    _update(0, 0, self.n)

  def total(self):
    return sum(i * self._hour[0][i % 12] for i in xrange(1, 13))


def solve(num_timesteps):
  x_val, y_val, rects = compressed(num_timesteps)
  scanline = defaultdict(list)
  for x0, x1, y0, y1 in rects:
    scanline[y0].append((+1, x0, x1))
    scanline[y1].append((-1, x0, x1))
  seg = SegTree([x_val[i+1]-x_val[i] for i in xrange(len(x_val)-1)])
  m, n = x_val[-1], len(x_val)
  y_prev = 0
  ans = 0
  for y in sorted(scanline):
    ans += (y_val[y] - y_prev) * seg.total()
    for d, x0, x1 in scanline[y]:
      seg.update(x0, x1, d)
    y_prev = y_val[y]
  if y_prev < m:
    ans += (m - y_prev) * seg.total()
  return ans