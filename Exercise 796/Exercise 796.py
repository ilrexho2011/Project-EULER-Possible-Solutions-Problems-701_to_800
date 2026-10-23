class CardState(object):
    TOTAL_CARDS = 540
    MAX_DESIGNS = 10
    MAX_SUITS = 4
    MAX_RANKS = 13
    
    def __init__(self, card_count, designs, suits, ranks, joker_count):
        self.card_count = card_count
        self.designs = designs
        self.suits = suits
        self.ranks = ranks
        self.joker_count = joker_count
        
    @classmethod
    def start_state(cls):
        return cls(0, 0, 0, 0, 0)
    
    @classmethod
    def transitions(cls, state):
        card_count = state.card_count
        total_count = state.TOTAL_CARDS - card_count
        designs = state.designs
        suits = state.suits
        ranks = state.ranks
        joker_count = state.joker_count
        new_design_count = 54 * (state.MAX_DESIGNS - designs)
        new_suit_count = 130 * (state.MAX_SUITS - suits)
        new_rank_count = 40 * (state.MAX_RANKS - ranks)
        new_design_suit_intersection = 13 * (state.MAX_DESIGNS - designs) * (state.MAX_SUITS - suits)
        new_design_rank_intersection = 4 * (state.MAX_DESIGNS - designs) * (state.MAX_RANKS - ranks)
        new_suit_rank_intersection = 10 * (state.MAX_SUITS - suits) * (state.MAX_RANKS - ranks)
        new_design_suit_rank_intersection = (state.MAX_DESIGNS - designs) * (state.MAX_SUITS - suits) * (state.MAX_RANKS - ranks)
        new_joker_count = 2 * state.MAX_DESIGNS - joker_count
        new_joker_design_intersection = 2 * (state.MAX_DESIGNS - designs)
        # No change.
        t0_count = total_count - (new_design_count + new_suit_count + new_rank_count + new_joker_count - \
            new_design_suit_intersection - new_design_rank_intersection - new_suit_rank_intersection - \
            new_joker_design_intersection + new_design_suit_rank_intersection)
        # New design, no change to ranks, suits, or jokers.
        t1_count = new_design_count - new_joker_design_intersection - new_design_suit_intersection - \
            new_design_rank_intersection + new_design_suit_rank_intersection
        # New suit, no change to designs or ranks
        t2_count = new_suit_count - new_design_suit_intersection - new_suit_rank_intersection + \
            new_design_suit_rank_intersection
        # New rank, no change to designs or suits
        t3_count = new_rank_count - new_design_rank_intersection - new_suit_rank_intersection + \
            new_design_suit_rank_intersection
        # New design and rank, no change to suits
        t4_count = new_design_rank_intersection - new_design_suit_rank_intersection
        # New design and suit, no change to ranks
        t5_count = new_design_suit_intersection - new_design_suit_rank_intersection
        # New suit and rank, no change to designs
        t6_count = new_suit_rank_intersection - new_design_suit_rank_intersection
        # New suit, design, and rank
        t7_count = new_design_suit_rank_intersection
        # New joker, no change to designs
        t8_count = new_joker_count - new_joker_design_intersection
        # New joker, new design
        t9_count = new_joker_design_intersection
        assert \
            len(list(filter(lambda x: x < 0, [t0_count, t1_count, t2_count, t3_count, t4_count, t5_count, t6_count, t7_count, t8_count, t9_count]))) == 0, \
            f'{state}'
        assert(sum([t0_count, t1_count, t2_count, t3_count, t4_count, t5_count, t6_count, t7_count, t8_count, t9_count]) == total_count)
        for new_state, count in [
            (cls(card_count + 1, designs, suits, ranks, joker_count), t0_count),
            (cls(card_count + 1, designs + 1, suits, ranks, joker_count), t1_count),
            (cls(card_count + 1, designs, suits + 1, ranks, joker_count), t2_count),
            (cls(card_count + 1, designs, suits, ranks + 1, joker_count), t3_count),
            (cls(card_count + 1, designs + 1, suits, ranks + 1, joker_count), t4_count),
            (cls(card_count + 1, designs + 1, suits + 1, ranks, joker_count), t5_count),
            (cls(card_count + 1, designs, suits + 1, ranks + 1, joker_count), t6_count),
            (cls(card_count + 1, designs + 1, suits + 1, ranks + 1, joker_count), t7_count),
            (cls(card_count + 1, designs, suits, ranks, joker_count + 1), t8_count),
            (cls(card_count + 1, designs + 1, suits, ranks, joker_count + 1), t9_count),
        ]:
            if count > 0:
                yield new_state, count / total_count
                
    def __str__(self):
        return f'count: {self.card_count}, designs: {self.designs}, ranks: {self.ranks}, suits: {self.suits}, jokers: {self.joker_count}'
        
    def __hash__(self):
        return hash((self.card_count, self.designs, self.suits, self.ranks, self.joker_count))
    
    def __eq__(self, other):
        return self.card_count == other.card_count and \
            self.designs == other.designs and \
            self.suits == other.suits and \
            self.ranks == other.ranks and \
            self.joker_count == other.joker_count

def prob796():
    result = 0
    stack = [(CardState.start_state(), 1.0)]
    next_states = dict()
    while len(stack) > 0:
        #print(stack[0][0].card_count, len(stack), result)
        next_states.clear()
        for state, prob in stack:
            if state.designs == state.MAX_DESIGNS and state.suits == state.MAX_SUITS and state.ranks == state.MAX_RANKS:
                result += state.card_count * prob
                continue
            for next_state, next_prob in CardState.transitions(state):
                if next_state in next_states:
                    next_states[next_state] += prob * next_prob
                else:
                    next_states[next_state] = prob * next_prob
        stack.clear()
        stack = [(k, v) for k, v in next_states.items()]
    return result
