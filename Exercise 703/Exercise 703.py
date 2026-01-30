M = 1001001011


def f(x, n):
	b1 = x & 1
	b2 = (x >> 1) & 1
	b3 = (x >> 2) & 1
	return (x >> 1) + ((b1 & (b2 ^ b3)) << (n-1))


def S(n):
	# Undirected edges.
	global edges
	edges = [set() for _ in range(2**n)]
	for x in range(2**n):
		edges[x].add(f(x, n))
		edges[f(x, n)].add(x)

	result = 1
	unhandled = set(range(2**n))
	while unhandled:
		# Pick an arbitrary element and extract its connected component.
		v = next(iter(unhandled))
		connected_component = set()
		layer = set([v])
		while layer:
			connected_component.update(layer)
			layer = set(w for v in layer for w in edges[v]) - connected_component

		result = result * count_connected_component(connected_component, n) % M
		unhandled -= connected_component

	return result


def count_connected_component(connected_component, n):
	# Find the cycle.
	cycle = find_cycle(connected_component)

	# Build layers outwards from the cycle.
	layer = set(cycle)
	layers = []
	unhandled = set(connected_component)
	while layer:
		layers.append(layer)
		unhandled -= layer
		layer = set(w for v in layer for w in edges[v]) & unhandled

	# For the trees hanging off the vertices in the cycles, it's easy enough.
	counts_with = [0] * (2 ** n)
	counts_without = [0] * (2 ** n)
	processed = set()
	for layer in reversed(layers):
		for v in layer:
			counts_with[v] = 1
			counts_without[v] = 1
			for succ in edges[v] & processed:
				counts_with[v] *= counts_without[succ]
				counts_without[v] *= counts_with[succ] + counts_without[succ]
			counts_with[v] %= M
			counts_without[v] %= M

		processed.update(layer)

	# Special-case the cycle.
	# a, b count independent sets including the first element of the cycle and respectively
	# including or excluding the previously seen element of the cycle.
	# c, d are similar but excluding the first element of the cycle.
	a, b = counts_with[cycle[0]], 0
	c, d = 0, counts_without[cycle[0]]
	for v in cycle[1:]:
		a, b = b * counts_with[v] % M, (a + b) * counts_without[v] % M
		c, d = d * counts_with[v] % M, (c + d) * counts_without[v] % M

	# With the first element in the cycle, we must exclude the last one, so discard a.
	return b + c + d


def find_cycle(connected_component):
	# Pick an arbitrary element.
	v = next(iter(connected_component))

	# If we take one step for each edge in the component, we're guaranteed to be in the cycle.
	for _ in range(len(connected_component)):
		v = f(v, n)

	cycle = [v]
	v = f(v, n)
	while v != cycle[0]:
		cycle.append(v)
		v = f(v, n)

	return cycle


for n in [3, 4, 20]:
	print(n, S(n) % M)