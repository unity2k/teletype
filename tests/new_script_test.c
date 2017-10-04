#include "new_script_test.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  // ssize_t

#include "greatest/greatest.h"

#include "teletype.h"
#include "log.h"

error_t append_command(scene_state_t *ss, script_number_t script,
        const char *command) {
    char error_msg[TELE_ERROR_MSG_LENGTH];
    tele_command_t cmd;
    error_t ret = parse(command, &cmd, error_msg);
    ret = validate(&cmd, error_msg);
    uint8_t sl = ss_get_script_len(ss, script);
    ss_set_script_command(ss, script, sl, &cmd);
    ss_set_script_len(ss, script, sl + 1);
    return ret;
}


TEST test_script_trampoline() {
    scene_state_t ss;
    exec_state_t es;
    ss_init(&ss);
    
    append_command(&ss, 0, "X 1");
    append_command(&ss, 0, "SCRIPT 2");
    append_command(&ss, 1, "X 2");
    append_command(&ss, 1, "SCRIPT 3");
    append_command(&ss, 2, "X 3");

    run_script(&ss, 0);
    
    if (ss.variables.x != 3)
        FAIL();
    else
        PASS();
}

SUITE(script_suite) {
    RUN_TEST(test_script_trampoline);
}
