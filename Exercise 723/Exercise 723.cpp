const ld eps = 1e-10;

struct TPrecComp {
    bool operator()(ld a, ld b) const {
        if (fabs(a - b) < eps) return false;
        return a < b;
    }
};

ld norma(ld x) {
    x -= floor(x / (2 * PI)) * 2 * PI;
    if (x > 2 * PI - eps) x -= 2 * PI;
    return x;
}

i64 solve(map<int, int> m) {
    set<ld, TPrecComp> s = {0, PI / 2, PI, 3 * PI / 2};
    int pK = 4;
    for (auto w: m) {
        int p = w.fi, cnt = w.se;
        if (!cnt) continue;
        pK *= cnt + 1;
        vector<ld> ms;

        ld alpha;
        for1(x, p) {
            if (!isSquare(p - x * x)) continue;
            int y = sqrt(p - x * x) + 0.5;
            alpha = atan2(y, x);
            break;
        }

        forn(i, cnt + 1) ms.pb(norma(alpha * (cnt - 2 * i)));
        set<ld, TPrecComp> ns;
        for (ld a: s) for (ld b: ms) ns.insert(norma(a + b));
        s = ns;
    }

    vector<ld> vs(all(s));
    i64 K = vs.size();
    int hK = K / 2;
    i64 ans = 0;
    forn(i, hK) {
        ans += 1LL * (hK - 1) * (hK - i - 1);
        ans += 1LL * i * (hK - 1);
    }
    ans -= 1LL * hK * (hK - 1) / 2;

    map<ld, i64, TPrecComp> cnt;
    forn(p, K) {
        fore(q, p + 1, K - 1) {
            ld dif = vs[q] - vs[p];
            if (dif > PI - eps) break;
            ans += cnt[PI - dif];
            ld a1 = vs[p] - PI + PI - dif, a2 = vs[q] - PI - (PI - dif);
            bool case1 = s.count(vs[p] - PI) && a1 < vs[p] - eps && a1 > vs[p] - PI + eps && s.count(a1);
            bool case2 = s.count(vs[q] - PI) && a2 < vs[q] - PI - eps && s.count(a2);
            if (case1) --ans;
            if (case2) --ans;
            if (case1 && case2 && fabs(dif - PI / 2) < eps) ++ans;
        }
        forn(r, p) ++cnt[vs[p] - vs[r]];
    }
    return ans;
}

i64 totalSolve(vector<pii> v, int i, map<int, int> &m) {
    if (i == v.size()) return solve(m);
    i64 res = 0;
    forn(j, v[i].se + 1) {
        m[v[i].fi] = j;
        res += totalSolve(v, i + 1, m);
        m.erase(v[i].fi);
    }
    return res;
}