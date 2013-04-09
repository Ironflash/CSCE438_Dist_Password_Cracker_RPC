struct networkMessage{
	unsigned int connid;
    unsigned int seqnum;
    string payload<64>;
};

program NFS_PROGRM {
	version NFS_VERS {
		void TEST_FUNC(networkMessage)  = 1;   /* procedure number = 1 */
		void callbackfn(int)  = 2;   /* procedure number = 2 */
	} = 1;
} = 0x33301138;
