/* rpc.x - demonstration of NFS via rpc */


struct stuff {
        string name<64>;
        int val;
};

program NFS_PROGRM {
	version NFS_VERS {
		string TEST_FUNC(stuff)  = 1;   /* procedure number = 1 */
		void callbackfn(int)  = 2;   /* procedure number = 2 */
	} = 1;
} = 0x33348688;	
