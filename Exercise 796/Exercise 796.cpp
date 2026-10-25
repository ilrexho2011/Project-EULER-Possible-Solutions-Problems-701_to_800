using state_t = std::array<uint32_t, 3>;
using map_t = std::unordered_map<state_t, double, boost::hash<state_t>>;

double a_grand_shuffle(uint32_t const decks, uint32_t const jokers) {
  std::array<map_t, 2> cache;
  auto & [current, next] = cache;
  // Initially all decks, ranks and suits are unseen
  current[{0, 0, 0}] = 1;

  auto total = decks*(52 + jokers);
  double expectation = 0;

  // Draw cards one by one until there are none left
  for (uint32_t i = 1, left = total; i <= total; ++i, --left) {
    for (auto const& [state, prob] : current) {
      if (prob != 0) {
        auto const& [d, r, s] = state;
        auto const p = prob/left;
        if (d < decks) {
          if (r < 13) {
            if (s < 4) 
              next[{d+1, r+1, s+1}] += p*(decks-d)*(13-r)*(4-s);
            if (s) 
              next[{d+1, r+1, s}] += p*(decks-d)*(13-r)*s;
          }
          if (r && s < 4)
            next[{d+1, r, s+1}] += p*(decks-d)*r*(4-s);
          next[{d+1, r, s}] += p*(decks-d)*(r*s + jokers);
        }
        if (d) {
          if (r < 13) {
            if (s < 4)
              next[{d, r+1, s+1}] += p*d*(13-r)*(4-s);
            if (s)              
              next[{d, r+1, s}] += p*d*(13-r)*s;
          }
          if (r && s < 4)
            next[{d, r, s+1}] += p*d*r*(4-s);
          next[{d, r, s}] += p*(left - (decks-d)*(52+jokers) - d*(52-r*s));
        }
      }
    }
    // accumulate the expectation
    expectation += next[{decks, 13, 4}] * i;
    next.erase({decks, 13, 4});
    current.clear();
    std::swap(current, next);
  }

  return expectation;
}
