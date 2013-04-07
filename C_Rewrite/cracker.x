struct networkMessage{
	unsigned int connid;
    unsigned int seqnum;
    string payload<64>;
};

program CRACKER_PROG {
	version CRACKER_VERS {
		/*sends message to server and a response from the server is returned*/
		networkMessage SEND_MESSAGE(networkMessage) = 1;
		void callbackfn(int) = 2;   /* procedure number = 2 */
	} = 1;
} = 0x33301138;