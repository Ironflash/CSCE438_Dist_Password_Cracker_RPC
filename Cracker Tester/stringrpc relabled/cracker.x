/* strings.x - demonstration of working with strings via rpc */

struct stuff {
	string name<64>;
	int val;
};

program TEST_PROG1 {
	version TEST_VERS1 {
		stuff TEST_FUNC1(stuff)  = 1;	/* procedure number = 1 */
	} = 1;
} = 0x33381138;	/* replace 88888 with some random numbers */

program TEST_PROG2 {
	version TEST_VERS2 {
		stuff TEST_FUNC2(stuff)  = 1;	/* procedure number = 1 */
	} = 1;
} = 0x33381139;	/* replace 88888 with some random numbers */
