/* strings.x - demonstration of working with strings via rpc */

struct networkMessage {
	string name<64>;
	int val;
	u_int seqnum;
	u_int* payload;
};

program CRACKER_PROG {
	version CRACKER_VERS {
		string send_message(networkMessage)  = 1;	/* procedure number = 1 */
	} = 1;
} = 0x33381138;	/* replace 88888 with some random numbers */
