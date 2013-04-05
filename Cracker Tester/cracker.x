struct networkMessage{
	unsigned int connid;
    unsigned int seqnum;
    string payload<64>;
};

program CRACKER_PROG {
	version CRACKER_VERS {
		/*sends message to server and a response from the server is returned*/
		string send_message(networkMessage) = 1;
	} = 1;
} = 0x33301138;