struct networkMessage{
	uint8_t connid;
    uint8_t seqnum;
    uint8_t* payload;
};

program cracker {
        version 0 {
        	/*sends message to server and a response from the server is returned*/
            networkMessage send_message(networkMessage) = 1;
        } = 1;
} = 0x20000099;