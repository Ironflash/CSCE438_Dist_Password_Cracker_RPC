struct crackMessage{
	uint8_t* hash;
    int len;
};

program cracker {
        version 0 {
        	/*request uses to give server a hash to crack and gets the result returned to it*/
            string crack_password(crackMessage) = 1;

            /*worker uses to inform server that it is ready and gets a password chunk to crack in return*/
            string inform_available() = 2; 

            /*worker uses to deliver crack result*/
            void deliver_result(string) = 3; 
        } = 1;
} = 0x20000099;