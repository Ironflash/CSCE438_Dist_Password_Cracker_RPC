struct networkMessage {
	unsigned int connid;
	unsigned int seqnum;
	string payload<64>;
};

program NFS_PROGRM {
	version NFS_VERS {
		string TEST_FUNC(networkMessage)  = 5;   /* procedure number = 1 */
		void callbackfn(int)  = 6;   /* procedure number = 1 */		
	} = 1;
} = 0x33341138;	
