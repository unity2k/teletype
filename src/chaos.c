#include "chaos.h"

// constants defining I/O ranges
static const int16_t chaos_value_min = -10000;
static const int16_t chaos_value_max = 10000;
static const int16_t chaos_param_min = 0;
static const int16_t chaos_param_max = 10000;
// fixed beta for henon map
static const float chaos_henon_b = 0.3;
// cellular automata parameters (1-d, binary)
static const int chaos_cell_count = 8;
static const int chaos_cell_max = 0xff;

static chaos_state_t chaos_state = {
  .ix = 5000,
  .ir = 5000,
  //  .algo = LOGISTIC
  // .algo = CUBIC
  // .algo = HENON
};

// scale integer state and param values to float, as appropriate for given algorithm
static void chaos_scale_values(chaos_state_t* state) {
  switch(state->algo) {
  case HENON:
    // for henon, x in [-1.5, 1.5], r in [1, 1.4]
    state->fx = state->ix / (float)chaos_value_max * 1.5;
    state->fr = 1.f + state->ir / (float)chaos_param_max * 0.4;
    break;
  case CELLULAR:
    // 1d binary CA takes binary state and rule
    if(state->ix > chaos_cell_max) { state->ix = chaos_cell_max; }
    if(state->ix < 0 ) { state->ix = 0; }
    // rule is 8 bits
    if(state->ir > 0xff) { state->ir = 0xff; }
    if(state->ir < 0 ) { state->ir = 0; }
    break;
  case LOGISTIC: // fall through
  case CUBIC: // fall through
  default:
    // for cubic / logistic, x in [-1, 1] and r in [3.2, 4)
    state->fx = state->ix / (float)chaos_value_max;
    state->fr = state->ir / (float)chaos_param_max * 0.7999 + 3.2;
    break;

  }
}

void chaos_set_val(int16_t val) {  
  chaos_state.ix = val;
  chaos_scale_values(&chaos_state);
}

static int16_t logistic_get_val() {
  chaos_state.fx =
    chaos_state.fx * chaos_state.fr * (1.f - chaos_state.fx);
  chaos_state.ix = chaos_state.fx * (float)chaos_value_max;
  return chaos_state.ix;  
}

static int16_t cubic_get_val() {
  float x3 = chaos_state.fx * chaos_state.fx * chaos_state.fx;    
  chaos_state.fx = chaos_state.fr * x3 + chaos_state.fx * (1.f - chaos_state.fr);
  chaos_state.ix = chaos_state.fx * (float)chaos_value_min;
  return chaos_state.ix;
}

static int16_t henon_get_val() {
  float x0_2 = chaos_state.fx0 * chaos_state.fx0;
  float x = 1.f - (x0_2 * chaos_state.fr) + (chaos_henon_b * chaos_state.fx1);
  // clamp to avoid blowup
  if(x < -1.5) { x = -1.5; }
  if(x > 1.5) { x = 1.5; }
  chaos_state.fx1 = chaos_state.fx0;
  chaos_state.fx0 = chaos_state.fx;
  chaos_state.fx = x;
  chaos_state.ix = x / 1.5 * (float)chaos_value_max;
  return chaos_state.ix;
}

static int16_t cellular_get_val() {
  uint8_t code = 0;
  (void)code;
  // TODO
  /*
    here's some old supercollider code.
    NB: this is wrapping behavior.
    the other alternative is to have endpoints fixed.

    var code = 0; // 8 bit input to rule
    code = code | (val[(i-1).wrap(0, n-1)] << 2);
    code = code | (v << 1);
    code = code | (val[(i+1).wrap(0, n-1)]);
    val[i] = (rule & (1 << code)) >> code;
   */
  return 0;
}


int16_t chaos_get_val() {
  switch(chaos_state.algo) {
  case LOGISTIC:
    return logistic_get_val();
  case CUBIC:
    return cubic_get_val();
  case HENON:
    return henon_get_val();
  case CELLULAR:
    return cellular_get_val();
  default:
    return 0;
  }
}

void chaos_set_r(int16_t r) {
  chaos_state.ir = r;
  chaos_scale_values(&chaos_state);
} 

int16_t chaos_get_r() {
  return chaos_state.ir;
}
