#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "minunit.h"
#include "onedriver.h"
#include "systemd.h"

// does systemd path escaping work correctly?
MU_TEST(test_systemd_path_escape) {
    char *escaped;
    systemd_path_escape("/home/test/yesYes", &escaped);
    mu_check(strcmp(escaped, "home-test-yesYes") == 0);
    free(escaped);

    systemd_path_escape("words@ test", &escaped);
    mu_check(strcmp(escaped, "words\\x40\\x20test") == 0);
    free(escaped);
}

// does systemd unit name templating work correctly?
MU_TEST(test_systemd_template_unit) {
    char *escaped;
    systemd_template_unit(ONEDRIVER_SERVICE_TEMPLATE, "this-is-a-test", &escaped);
    mu_check(strcmp(escaped, "onedriver@this-is-a-test.service") == 0);
    free(escaped);
}

// can we enable and disable systemd units? (and correctly check if the units are
// enabled/disabled?)
MU_TEST(test_systemd_unit_enabled) {
    char cwd[1024];
    getcwd(cwd, 1024);
    strcat(cwd, "/mount");

    char *cwd_escaped, *unit_name;
    systemd_path_escape(cwd, &cwd_escaped);
    systemd_template_unit(ONEDRIVER_SERVICE_TEMPLATE, cwd_escaped, &unit_name);
    free(cwd_escaped);

    // make sure things are disabled before test start
    mu_check(systemd_unit_set_enabled(unit_name, false));
    mu_check(!systemd_unit_is_enabled(unit_name));

    // actual test content
    mu_assert(systemd_unit_set_enabled(unit_name, true), "Could not enable unit.");
    mu_assert(systemd_unit_is_enabled(unit_name),
              "Unit could not detect unit as enabled.");
    mu_assert(systemd_unit_set_enabled(unit_name, false), "Could not disable unit.");
    mu_assert(!systemd_unit_is_enabled(unit_name),
              "Unit was still enabled after disabling.");

    free(unit_name);
}

// Can we enable and disable the onedriver service (and correctly check if the unit is
// active/stopped?). Do a few checks on the fs functions while things are mounted as well.
MU_TEST(test_systemd_unit_active) {
    char cwd[1024];
    getcwd(cwd, 1024);
    strcat(cwd, "/mount");

    char *cwd_escaped, *unit_name;
    systemd_path_escape(cwd, &cwd_escaped);
    systemd_template_unit(ONEDRIVER_SERVICE_TEMPLATE, cwd_escaped, &unit_name);
    free(cwd_escaped);

    // make extra sure things are off before we start
    mu_check(systemd_unit_set_active(unit_name, false));
    mu_check(!systemd_unit_is_active(unit_name));

    mu_assert(systemd_unit_set_active(unit_name, true), "Could not start unit.");
    fs_poll_until_avail((const char *)&cwd);
    mu_assert(systemd_unit_is_active(unit_name), "Did not detect unit as active");

    // test this function while we're at it
    char *account_name = fs_account_name((const char *)&cwd);
    mu_assert(account_name, "Could not determine account name.");
    // TODO actually check email is valid later
    free(account_name);

    mu_assert(systemd_unit_set_active(unit_name, false), "Could not stop unit.");
    mu_assert(!systemd_unit_is_active(unit_name), "Did not detect unit as stopped");

    free(unit_name);
}

MU_TEST_SUITE(systemd_tests) {
    MU_RUN_TEST(test_systemd_path_escape);
    MU_RUN_TEST(test_systemd_template_unit);
    MU_RUN_TEST(test_systemd_unit_enabled);
    MU_RUN_TEST(test_systemd_unit_active);
}

int main(int argc, char **argv) {
    MU_RUN_SUITE(systemd_tests);
    MU_REPORT();
    return MU_EXIT_CODE;
}
