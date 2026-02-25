const BIG_0 = BigInt(0);
const BIG_1 = BigInt(1);
const BIG_10 = BigInt(10);
const BIG_NEG_ONE = BigInt(-1);

// a / b
class Float {
    /**
     * @param {bigint | number | string} a 
     * @param {bigint | number | string} b 
     */
    constructor(a, b = BIG_1) {
        this.a = BigInt(a);
        this.b = BigInt(b);
    }
    /**
     * reduce bigint sizes
     * @param {number} maxSize 
     */
    normalize(maxSize) {
        const { a, b } = this;
        const c = BIG_10 ** BigInt(maxSize);
        if (a > c && b > c) {
            return new Float(a / c, b > c ? b / c : 1);
        }
        return new Float(a, b);
    }
    /**
     * Float.fromFloat('2.34')
     * new Float(234, 100)
     * @param {string} c 
     */
    static fromFloat(c) {
        const i = c.indexOf('.');
        if (i === -1) {
            // int
            return new Float(c, 1);
        }
        const prefix = c.slice(0, i);
        const suffix = c.slice(i + 1);
        return new Float(prefix + suffix, '1' + '0'.repeat(suffix.length));
    }
    /**
     * this + C
     * @param {Float} C 
     */
    add(C) {
        const { a, b } = this;
        // a / b + C.a / C.b
        // (a * C.b + C.a * b) / b * C.b
        return new Float(a * C.b + C.a * b, b * C.b);
    }
    /**
     * this - C
     * @param {Float} C 
     */
    sub(C) {
        const { a, b } = this;
        // a / b - C.a / C.b
        // (a * C.b - C.a * b) / b * C.b
        return new Float(a * C.b - C.a * b, b * C.b);
    }
    /**
     * this * C
     * @param {Float} C 
     */
    mul(C) {
        const { a, b } = this;
        // a / b * C.a / C.b
        // (a * C.a) / b * C.b
        return new Float(a * C.a, b * C.b);
    }
    toString(precision = 10) {
        const { a, b } = this;
        // precision = 2
        // a = 532
        // b = 100
        // a / b
        // a * 10**2 / b = a / b = 53200 / 100 = 532
        // 5.32
        const c = (a * BIG_10 ** BigInt(precision) / b).toString().padStart(precision + 1, '0');
        return c.slice(0, -precision) + '.' + c.slice(-precision);
    }
    sign() {
        const { a, b } = this;
        const ZERO = BIG_0;
        if (a === ZERO) return 0;
        return a * b > ZERO ? 1 : -1;
    }
    abs() {
        const { a, b } = this;
        const ZERO = BIG_0;
        const NEG_ONE = BIG_NEG_ONE;
        return new Float(a < ZERO ? a * NEG_ONE : a, b < ZERO ? b * NEG_ONE : b);
    }
    floor() {
        const { a, b } = this;
        return new Float(a / b, 1);
    }
}

const FLOAT_1 = new Float(1);
const FLOAT_0 = new Float(0);
const FLOAT_HALF = new Float(1, 2);

/**
 * 
 * @param {Float} b 
 */
const T_Float = (b, precision = 16) => {
    let c = b.floor().a + '.';
    while (c.length < precision) {
        const a = new Float(b.floor().a);
        b = b.sub(a).add(FLOAT_1).mul(a);
        c += b.floor().a.toString();
    }
    return c;
};
// search for f(x)=0
// a < b
// f(a) < 0
// f(b) > 0 
/**
 * 
 * @param {Float} a 
 * @param {Float} b 
 * @param {(b: Float) => Float} f 
 * @param {Float} precision 
 * @param {number} maxIters 
 */
function binarySearch_Float(a, b, f, precision = FLOAT_0, maxIters = 100, normalizeSize = 1000) {
    let fa = f(a),
        fb = f(b);
    for (let i = 0; i < maxIters; i++) {
        const c = a.add(b).mul(FLOAT_HALF).normalize(normalizeSize);
        console.time('f(c) ');
        const fc = f(c);
        console.timeEnd('f(c) ');
        if (fc.mul(fa).sign() > 0) {
            a = c;
            fa = fc;
        } else {
            b = c;
            fb = fc;
        }
        if (fc.abs().sub(precision).sign() <= 0) {
            console.warn('binarySearch found after', i, 'iters');
            return [a, b];
        }
    }
    console.warn('hit binarySearch maxIters');
    return [a, b];
}
const solve = (T_precision = 16, binarySearch_precision = 5, binarySearch_maxIters = 100, binarySearch_normalizeSize = 1000) => {
    return binarySearch_Float(new Float(2), new Float(3), b => Float.fromFloat(T_Float(b, T_precision)).sub(b), new Float(1, BIG_10 ** BigInt(binarySearch_precision)), binarySearch_maxIters, binarySearch_normalizeSize)[0].toString(100);
};

console.time('t');
const b = solve(27, 26, 100, 4000).slice(0, 50);
console.timeEnd('t');
console.log(b);
const bT = T_Float(Float.fromFloat(b), 50);
console.log(bT);
for (let i = b.length; i >= 0; i--) {
    if (b.slice(0, i) === bT.slice(0, i)) {
        console.log(b.slice(0, i), i);
        console.log('solution', b.slice(0, 26));
        if (i < 26) {
            console.warn('solution not complete');
        }
        break;
    }
}