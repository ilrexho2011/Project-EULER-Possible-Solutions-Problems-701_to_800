from time import perf_counter
import sys
import itertools as it
from math import isqrt
from collections import deque

#------------------------------------------------------------------------------

def p763(N=10**4, mod=10**9):
    if mod == 0:
        sys.set_int_max_str_digits(100000)
    D = [0]*(N+1)
    D[0] = D[1] = 1
    Cs = [[[1], [1]], [[3,3]]]
    counts = Cs[-1][0]
    min_moves = [1]
    for w in it.count(2):
        min_moves.append(moves := w*(w+1)//2)
        if 2*moves > N:
            break
        counts = counts+[3+2*counts[-1]]
        counts[-2] += 3
        if mod:
            counts[-2] %= mod
            counts[-1] %= mod
        Cs.append([counts])
        if Verbose > 1: print(f"Cs[{w}][{min_moves[-1]}] = {counts}")
    min_moves.append((w+1)*(w+2)//2)
    for m in range(2, N+1):
        prev_m = m-1
        for w in it.count(0):
            if w > 0:
                prev_m, m = m, m+w+1
                if m+min_moves[w-1] > N:
                    break

            # move from (w-1, m-w-1)
            if w == 0:
                pass
            elif w == 1:
                counts = Cs[0][prev_m]
                count = 3*counts[0]
                counts = [count, count]
            else:
                m_idx = prev_m-min_moves[w-1]
                counts = Cs[w-1][m_idx]
                counts = counts+[counts[0]+2*counts[-1]]
                counts[-2] += counts[0]

            # move from (w, m-w-1)
            if w == 0:
                counts = [3*Cs[0][m-1][0]]
            elif w == 1:
                if prev_m >= 3:
                    prev_counts = Cs[1][prev_m-3]
                    pc = 4*prev_counts[0]
                    counts[0] += pc
                    counts[1] += pc
            else:
                m_idx = prev_m-min_moves[w]
                if m_idx >= 0:
                    prev_counts = Cs[w][m_idx]
                    for i in range(w):
                        counts[i] += prev_counts[i]+prev_counts[i+1]
                    counts[0] += prev_counts[0]
                    counts[-2] += prev_counts[0]
                    counts[-1] += prev_counts[0]+3*prev_counts[-1]

            # move from (w+1, m-w-1)
            m_idx = prev_m-min_moves[w+1]
            if m_idx >= 0:
                prev_counts = Cs[w+1][m_idx]
                Cs[w+1][m_idx] = None   # release old lists no longer needed
                if w == 0:
                    counts[0] += prev_counts[0]
                else:
                    counts[0] += prev_counts[0]+prev_counts[1]
                    for i in range(1, w+1):
                        counts[i] += prev_counts[i+1]

            if mod:
                for i in range(w+1):
                    counts[i] %= mod
            # assert w == 0 or sum(counts[:-1])%mod == counts[-1], counts
            Cs[w].append(counts)
            if w == 0:
                D[m] = counts[0]

            if Verbose > 1: print(f"Cs[{w}][{m}] = {counts}")
    if Verbose:
        for n in range(N+1):
            print(f"D({n}) = {D[n]}")
    return D[N]

#------------------------------------------------------------------------------

def p763_v1(N=10**4, mod=10**9):
    def update_state(width, c1, c2, c2sum):
        #assert sum(c2)%mod == c2sum%mod
        new_nmoves = nmoves+width+1
        if width == 2:
            Hprime[new_nmoves] += c1+c2sum
            if Verbose: print("===", iterations, (width, new_nmoves), c1, c2, c2sum)
        moves_reqd = new_nmoves+width*(width+1)//2
        if moves_reqd <= N:
            if new_nmoves <= N:
                counts[width][new_nmoves] += 1
            key = (width, new_nmoves)
            data = new_states.get(key, None)
            if data is None:
                if Verbose > 1: print(">>>", iterations, (width, new_nmoves, moves_reqd), c1, c2, c2sum)
                if mod:
                    c1 %= mod
                    c2sum %= mod
                    for i in range(len(c2)):
                        c2[i] %= mod
                new_states[key] = (c1, c2, c2sum)
            else:
                if Verbose > 1: print("+++", iterations, (width, new_nmoves, moves_reqd), c1, c2, c2sum)
                oc1, oc2, oc2sum = data
                oc1 += c1
                oc2sum += c2sum
                if mod:
                    oc1 %= mod
                    oc2sum %= mod
                    for i in range(len(c2)):
                        oc2[i] = (oc2[i]+c2[i])%mod
                else:
                    for i in range(len(c2)):
                        oc2[i] += c2[i]
                new_states[key] = (oc1, oc2, oc2sum)

    states = {(2,3) : (3,[6],6)}
    Hprime = [0]*(N+1)
    counts = [[0]*(N+1) for _ in range(isqrt(N)+1)]
    counts[2][3] = 1
    Hprime[3] = 9
    iterations = 1
    while states:
        new_states = {}
        for (width, nmoves), (c1, c2, c2sum) in sorted(states.items()):
            assert width >= 2

            # Handle u/v pair (1,1), adding a new u-label and v-label
            nw = width+1
            nc1 = c1
            nc2 = c2+[2*c1+c2sum]
            nc2sum = 2*(c2sum+c1)
            update_state(nw, nc1, nc2, nc2sum)

            # Handle u/v pair (0,0) removing the oldest u- and v-labels
            if width > 2:
                nw = width-1
                t2 = c2[0]
                nc1 = c1+t2
                nc2 = c2[1:]
                nc2sum = c2sum-t2
                update_state(nw, nc1, nc2, nc2sum)

            # Handle u/v pair of (0,1) removing oldest v-label and adding new one,
            # and (1,0) removing oldest u-label and adding new one
            t2 = c2[0]
            nc2 = c2[:]
            for i in range(len(nc2)-1):
                nc2[i] += nc2[i+1]
            nc2[-1] += 2*c1+c2sum
            nc2sum = 3*c2sum+2*c1-t2
            nc1 = 2*c1+t2
            update_state(width, nc1, nc2, nc2sum)

        states = new_states
        print(iterations, len(states), perf_counter()-start_time)
        iterations += 1

    P1 = [0]*(N+1)
    c = 1
    for i in range(1, N+1):
        P1[i] = c
        c *= 3
        if mod: c %= mod
    P2 = [0]*(N+1)
    c = 3
    for i in range(2, N+1, 2):
        P2[i] = c
        c *= 4
        if mod: c %= mod
    P1P2 = poly_mul(P1, P2, N, mod)
    one_minus_P1P2 = poly_sub([1], P1P2, mod)
    G2 = poly_div(P1, one_minus_P1P2, N, mod)
    G2[0] += 1
    V2 = poly_div(P2, one_minus_P1P2, N, mod)
    U2 = poly_mul(P1, V2, N, mod)
    inv9 = pow(9, -1, mod)
    for i in range(N+1):
        Hprime[i] = Hprime[i]*inv9%mod
    one_minus_V2H = poly_sub([1], poly_mul(V2, Hprime, N, mod), mod)
    U2sq = poly_mul(U2, U2, N, mod)
    Gpart = poly_div(poly_mul(U2sq, Hprime, N, mod), one_minus_V2H, N, mod)
    G = poly_add(G2, Gpart, mod)
    return G[N]

def poly_mul(poly1, poly2, N, mod=0):
    ''' Multiply the two polynomials, ignoring all terms of degree > N '''
    deg1, deg2 = len(poly1)-1, len(poly2)-1
    res_deg = min(deg1+deg2, N)
    res = [0]*(res_deg+1)
    for i, t1 in enumerate(poly1):
        if t1 == 0:
            continue
        for j, t2 in enumerate(poly2):
            if t2 == 0:
                continue
            if i+j > N:
                break
            term = res[i+j]+t1*t2
            if mod: term %= mod
            res[i+j] = term
    return res

def poly_div(poly1, poly2, N, mod=0):
    ''' Divide poly1 by poly2, assuming the result will be an infinite series,
        but ignoring all terms of degree > N.  Requires the constant term (x^0
        coefficient) of poly2 to be 1. '''
    assert poly2[0] == 1
    deg1, deg2 = len(poly1)-1, len(poly2)-1
    q = [0]*(N+1)
    r = poly1.copy()
    if deg1 < N:
        r += [0]*(N-deg1)
    for cur_deg in range(N+1):
        a = q[cur_deg] = r[cur_deg]
        i, j = 1, cur_deg+1
        for i in range(1, deg2+1):
            if j > N: break
            term = r[j]-a*poly2[i]
            if mod: term %= mod
            r[j] = term
            j += 1
    return q

def poly_add(poly1, poly2, mod=0):
    if len(poly1) < len(poly2):
        poly1, poly2 = poly2, poly1
    res = poly1.copy()
    for i, c2 in enumerate(poly2):
        term = poly1[i]+c2
        if mod: term %= mod
        res[i] = term
    return res

def poly_sub(poly1, poly2, mod=0):
    res = poly1.copy()
    if len(poly1) < len(poly2):
        res += [0]*(len(poly2)-len(poly1))
    for i, c2 in enumerate(poly2):
        term = poly1[i]-c2 if i < len(poly1) else -c2
        if mod: term %= mod
        res[i] = term
    return res

#------------------------------------------------------------------------------

def p763_test(*target):
    """ Find the instance count of states whose level-split counts tuple
        matches the list of integers on the cmdline, which must start and end
        with 1, never drop below 1, and each value is -1, 0, or +1 added to
        the previous one. """
    assert len(target) >= 1, "Counts missing"
    assert target[0] == target[-1] == 1, "Counts must start and end with 1"
    for i in range(1, len(target)):
        assert target[i] >= 1, "Counts must be positive"
        assert abs(target[i]-target[i-1]) <= 1, "Counts can differ by 0 or 1 from previous count"
    count = count_voidance_instances(target, False)
    print(count, target)

def count_voidance_instances(void_counts, optimize=True):
    """ Given a sequence of the counts of cells split on each level, find the
        count of states that can occur. """

    def count_no_singletons(seq):
        """ Count instances for void counts sequence that starts and ends with 2
            with no internal 1s. """
        subres = 0
        uv_vecs, mult = get_uv_vectors(seq, optimize)
        for u, v in uv_vecs:
            t2 = count_polyominoid_instances_improved(u, v)
            print("###", (t2, mult), u, v)
            subres += t2
        return mult*subres

    def count_polyominoid_instances_improved(u, v):
        assert len(u) == len(v) > 1
        assert u[0] == v[0] == 1
        assert u[-1] == v[-1] == 0
        w = 0
        c1 = 1
        for i in range(len(u)-1):
            match u[i],v[i]:
              case (1,1):
                if w == 0:
                    w = 1
                    c1 *= 6
                elif w == 1:
                    w = 2
                    c2 = deque([c1])
                    c3 = deque([c1])
                    c2sum = c3sum = c1
                else:
                    w += 1
                    c2.append(c1+c2sum)
                    c3.append(c1+c3sum)
                    c2sum += c2[-1]
                    c3sum += c3[-1]
              case (0,0):
                assert w > 0
                if w == 1:
                    w = 0
                else:
                    w -= 1
                    t2 = c2.popleft()
                    t3 = c3.popleft()
                    c1 += t2+t3
                    c2sum -= t2
                    c3sum -= t3
              case (0,1):
                if w == 0:
                    c1 *= 3
                elif w == 1:
                    c1 *= 2
                else:
                    t2 = c2.popleft()
                    c2.append(c1+c2sum)
                    c2sum += c1+c2sum-t2
                    c1 += t2
              case (1,0):
                assert w > 0
                if w == 1:
                    c1 *= 2
                else:
                    t3 = c3.popleft()
                    c3.append(c1+c3sum)
                    c3sum += c1+c3sum-t3
                    c1 += t3
        assert c1%6 == 0
        return c1//6 # caller wants to multiply by 3, remove double-counting

    def count_polyominoid_instances(u, v):
        """ Count the instances for a specific u/v vector pair.  Run all the
            u labels through possible values from [xyz], and see how many
            choices are available for each v label. """

        choice_counts = (0,1,1,2,1,2,2,3)

        def recurse(v_choices, u_idx):
            if u_idx >= len(conflicts):
                res = 1
                for i in range(1, len(v_choices)):
                    choices = choice_counts[v_choices[i]]
                    res *= choices
            else:
                low, high = conflicts[u_idx]
                if low == 0:
                    u_complements = (0b101, 0b011)          # y or z
                    low = 1
                else:
                    u_complements = (0b110, 0b101, 0b011)   # x, y, or z
                res = 0
                for complement in u_complements:
                    new_v_choices = v_choices.copy()
                    for i in range(low, high+1):
                        val = new_v_choices[i]
                        val &= complement
                        if val == 0:
                            break
                        new_v_choices[i] = val
                    else:
                        res += recurse(new_v_choices, u_idx+1)
            return res

        if sum(u) > sum(v):
            u, v = v, u
        conflicts = []  # range of conflicting v labels per u label
        v_label = 0
        for u_val in u:
            if u_val == 0:
                v_label += 1
            else:
                conflicts.append([v_label,-1])
        v_label, u_idx = -1, 0
        for v_val in v:
            if v_val != 0:
                v_label += 1
            else:
                conflicts[u_idx][1] = v_label
                u_idx += 1
        num_v_labels = v_label+1
        v_choices = [0b111]*num_v_labels
        v_choices[0] = 0b001        # v1 = x
        for i in range(1, conflicts[0][1]+1):
            v_choices[i] = 0b101   # u1 = y, conflicts with earliest v labels
        res = recurse(v_choices, 1)
        return res

    # assert void_counts[0] == void_counts[-1] == 1
    res = 1
    if not optimize:
        compressed = void_counts
    else:
        compressed = [1]
        prev = 1
        for i in range(1, len(void_counts)):
            count = void_counts[i]
            if prev == count == 2:
                res *= 4
            else:
                compressed.append(count)
                prev = count
    start = 0
    for i in range(1, len(compressed)):
        if compressed[i] == 1:
            if i == start+1:
                res *= 3
            else:
                res *= count_no_singletons(compressed[start+1:i])
            start = i
    return res

def get_uv_vectors(seq, use_symmetry=True):
    """ Convert the void counts sequence into one or more pairs of u/v
        pairs, as per the Eriksson paper.  In the vectors, 1 means this
        edge is labeled, 0 means no label.  Also returns a multiplier to
        use after summing the instance counts over u/v pairs, to account
        for the symmetries we'll use. """
    res = [([1], [1])]
    mult = 3
    for i in range(len(seq)-1):
        delta = seq[i+1]-seq[i]
        if delta < 0:
            for j in range(len(res)):
                u, v = res[j]
                u.append(0)
                v.append(0)
        elif delta > 0:
            for j in range(len(res)):
                u, v = res[j]
                u.append(1)
                v.append(1)
        elif use_symmetry and mult == 3:
            assert len(res) == 1
            mult = 6
            u, v = res[0]
            u.append(0)
            v.append(1)
        else:
            assert mult == 6 or not use_symmetry
            for j in range(len(res)):
                u, v = res[j]
                u1 = u.copy()
                v1 = v.copy()
                u.append(0)
                v.append(1)
                u1.append(1)
                v1.append(0)
                res.append((u1, v1))
    for u, v in res:
        u.append(0)
        v.append(0)
    return res, mult

#------------------------------------------------------------------------------

Verbose = 0
DoCounts = False
Counts = [0] * 100

if __name__ == "__main__":
    def main():
        global Verbose, DoCounts, start_time
        entrypoint = p763
        while len(sys.argv) > 1 and sys.argv[1][0] == '-':
            opt = sys.argv[1].lower()
            if opt in ('-v', '-vv', '-vvv', '-vvvv'):
                Verbose += len(opt)-1
            elif opt in ('-c', '-count', '-counts'):
                DoCounts = True
            elif opt in ('-t', '-t1'):
                entrypoint = p763_test
            elif opt in ('-v1',):
                entrypoint = p763_v1
            else:
                print(f"Unknown option {sys.argv[1]}")
                sys.exit(1)
            sys.argv.pop(1)
        start_time = perf_counter()
        try:
            result = entrypoint(*map(int, sys.argv[1:]))
        except KeyboardInterrupt:
            result = None
            print("\nInterrupted")
        end_time = perf_counter()
        if DoCounts:
            while Counts and not Counts[-1]: Counts.pop()
            if Counts: print(Counts)
        if result is not None: print(result)
        print(f"Time: {end_time-start_time} secs")

    if len(sys.argv) > 1 and sys.argv[1] in ('-profile', '-prof'):
        import cProfile
        sys.argv.pop(1)
        cProfile.run('main()', sort='cumtime')
    else:
        main()