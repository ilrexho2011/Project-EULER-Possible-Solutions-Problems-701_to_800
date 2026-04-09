#include <vector>
#include <set>
#include <iostream>

using namespace std;
using uxxint = __uint128_t;

const int X = 6;
const int Y = 5;

struct Piece {
  vector<int> squares;
  uxxint bitmapf, bitmapv;
  int xmin, xmax, ymin, ymax;
};


vector<Piece> pieces = {
  {},
  {{0}},        // 1x1 square, pink
  {{0, 1, 6}},  // L up, red
  {{0, 5, 6}},  // L down, green
  {{0, 6}},     // I
  {{0, 1}},     // _
  {{0, 1, 6, 7}}  // square
};


void InitPieces() {
  for (int i = 1; i < pieces.size(); ++i) {
    auto &piece = pieces[i];
    uxxint bitmap = 0;
    vector<int> xs, ys;
    for (int square : piece.squares) {
      bitmap |= 1 << (square*4);
      xs.push_back((square+2) % 6 - 2);
      ys.push_back(square / 6);
    }
    piece.bitmapf = bitmap * 15;
    piece.bitmapv = bitmap * i;
    piece.xmin = *min_element(xs.begin(), xs.end());
    piece.xmax = *max_element(xs.begin(), xs.end());
    piece.ymin = *min_element(ys.begin(), ys.end());
    piece.ymax = *max_element(ys.begin(), ys.end());
  }
}

void Explore(uxxint init) {
  set<uxxint> seen;
  vector<uxxint> toanalyze = {init};

  while (!toanalyze.empty()) {
    const uxxint base = toanalyze.back(); toanalyze.pop_back();
    uxxint copy = base;
    for (int y = 0; y < Y; ++y)
      for (int x = 0; x < X; ++x) {
        int bit = (y*6 + x)*4;
        int pi = (copy >> bit) & 0xf;
        if (!pi) continue;
        const Piece& piece = pieces[pi];
        copy &= ~uxxint(piece.bitmapf << bit);
        uxxint without = base & ~uxxint(piece.bitmapf << bit);

        // Try moving up.
        if (y + piece.ymin > 0 && ((piece.bitmapf << (bit - 4*6)) & without) == 0) {
          uxxint next = without | (piece.bitmapv << (bit - 4*6));
          if (seen.insert(next).second) toanalyze.push_back(next);
        }

        // Try moving down.
        if (y + piece.ymax < Y-1 && ((piece.bitmapf << (bit + 4*6)) & without) == 0) {
          uxxint next = without | (piece.bitmapv << (bit + 4*6));
          if (seen.insert(next).second) toanalyze.push_back(next);
        }

        // Try moving left.
        if (x + piece.xmin > 0 && ((piece.bitmapf << (bit - 4*1)) & without) == 0) {
          uxxint next = without | (piece.bitmapv << (bit - 4*1));
          if (seen.insert(next).second) toanalyze.push_back(next);
        }

        // Try moving right.
        if (x + piece.xmax < X-1 && ((piece.bitmapf << (bit + 4*1)) & without) == 0) {
          uxxint next = without | (piece.bitmapv << (bit + 4*1));
          if (seen.insert(next).second) toanalyze.push_back(next);
        }
      }
  }
  cout << seen.size() << endl;
}

uxxint Create(vector<int> vals) {
  uxxint bitmap = 0;
  for (int i = 0; i < vals.size(); ++i)
    bitmap |= uxxint(vals[i]) << (4*i);
  return bitmap;
}


int main() {
  InitPieces();
  Explore(Create({
        0, 2, 2, 3, 2, 2,
        0, 2, 3, 3, 2, 4,
        1, 1, 6, 6, 4, 4,
        1, 1, 6, 6, 4, 3,
        1, 1, 5, 5, 3, 3,
      }));
  return 0;
}