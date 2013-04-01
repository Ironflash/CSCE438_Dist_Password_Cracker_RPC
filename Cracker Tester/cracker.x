struct networkMessage{
	uint8_t connid;
    uint8_t seqnum;
    uint8_t* payload;
};

program CRACKER_PROG {
        version CRACKER_VERS {
        	/*sends message to server and a response from the server is returned*/
            networkMessage send_message(networkMessage) = 1;
        } = 1;
} = 0x33301138;