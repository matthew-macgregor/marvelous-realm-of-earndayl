/* Game Main Entrypoint */
#include <stdio.h>
#include "colors.h"
#include "constants.h"

#define STRING_UTILS_IMPLEMENTATION
#include "string_utils.h"
// #define OUTPUT_IMPLEMENTATION
// #include "output.h"
#include "hero.h"
#include "input.h"

int main(void) {
	printf(CON_BLUE   "%s\n" CON_RESET, HL);
	printf(CON_YELLOW "%s\n" CON_RESET, "Welcome to the Marvelous Realm of Eärndayl");
	printf(CON_BLUE   "%s\n" CON_RESET, HL);

	hero_init();
	input_loop();
	return 0;
}
