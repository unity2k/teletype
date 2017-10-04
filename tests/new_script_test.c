#include "new_script_test.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // ssize_t

#include "greatest/greatest.h"

#include "teletype.h"
#include "log.h"

TEST test_script_trampoline() {
    scene_state_t ss;
    exec_state_t es;
    ss_init(&ss);
    es_init(&es);
    es_push(&es);
    es_set_script_number(&es, 0);

    
    
    tele_command_t cmd;
    char error_msg[TELE_ERROR_MSG_LENGTH];
    error_t error = parse("SCRIPT 2", &cmd, error_msg);
    error = validate(&cmd, error_msg);
    ss_set_script_command(&ss, 0, 0, &cmd);
    ss_set_script_len(&ss, 0, 1);
    error = parse("X 11", &cmd, error_msg);
    error = validate(&cmd, error_msg);
    ss_set_script_command(&ss, 1, 0, &cmd);
    ss_set_script_len(&ss, 1, 1);

    run_script(&ss, 0);
    
    if (ss.variables.x != 11)
        FAIL();
    else
        PASS();
}

SUITE(script_suite) {
    RUN_TEST(test_script_trampoline);
}
